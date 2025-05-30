/* cmd_sniffer example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "cmd_sniffer.h"

#include "argtable3/argtable3.h"
#include "cmd_pcap.h"
#include "esp_app_trace.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "sdkconfig.h"
#include <string.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>

#define SNIFFER_DEFAULT_CHANNEL (1)
#define SNIFFER_PAYLOAD_FCS_LEN (4)
#define SNIFFER_PROCESS_PACKET_TIMEOUT_MS (100)
#define SNIFFER_RX_FCS_ERR (0X41)
#define SNIFFER_DECIMAL_NUM (10)

static const char *TAG = "cmd_sniffer";

typedef struct {
  char *filter_name;
  uint32_t filter_val;
} wlan_filter_table_t;

typedef struct {
  void *payload;
  uint32_t length;
  uint32_t seconds;
  uint32_t microseconds;
} sniffer_packet_info_t;

static sniffer_runtime_t snf_rt = {0};
static wlan_filter_table_t wifi_filter_hash_table[SNIFFER_WLAN_FILTER_MAX] = {
    0};

static sniffer_cb_t sniffer_cb = NULL;
static sniffer_animation_cb_t sniffer_animation_start_cb = NULL;
static sniffer_animation_cb_t sniffer_animation_stop_cb = NULL;
static void (*out_of_mem_cb)();

void wifi_sniffer_register_cb(sniffer_cb_t callback, void *_out_of_mem_cb) {
  sniffer_cb = callback;
  out_of_mem_cb = _out_of_mem_cb;
}

void wifi_sniffer_register_animation_cbs(sniffer_animation_cb_t start_cb,
                                         sniffer_animation_cb_t stop_cb) {
  sniffer_animation_start_cb = start_cb;
  sniffer_animation_stop_cb = stop_cb;
}

static esp_err_t sniffer_stop(sniffer_runtime_t *sniffer);

static uint32_t hash_func(const char *str, uint32_t max_num) {
  uint32_t ret = 0;
  char *p = (char *)str;
  while (*p) {
    ret += *p;
    p++;
  }
  return ret % max_num;
}

static void create_wifi_filter_hashtable(void) {
  memset(wifi_filter_hash_table, 0, sizeof(wifi_filter_hash_table));
  char *wifi_filter_keys[SNIFFER_WLAN_FILTER_MAX] = {
      "mgmt", "data", "ctrl", "misc", "mpdu", "ampdu", "fcsfail"};
  uint32_t wifi_filter_values[SNIFFER_WLAN_FILTER_MAX] = {
      WIFI_PROMIS_FILTER_MASK_MGMT,      WIFI_PROMIS_FILTER_MASK_DATA,
      WIFI_PROMIS_FILTER_MASK_CTRL,      WIFI_PROMIS_FILTER_MASK_MISC,
      WIFI_PROMIS_FILTER_MASK_DATA_MPDU, WIFI_PROMIS_FILTER_MASK_DATA_AMPDU,
      WIFI_PROMIS_FILTER_MASK_FCSFAIL};
  for (int i = 0; i < SNIFFER_WLAN_FILTER_MAX; i++) {
    uint32_t idx = hash_func(wifi_filter_keys[i], SNIFFER_WLAN_FILTER_MAX);
    while (wifi_filter_hash_table[idx].filter_name) {
      idx++;
      if (idx >= SNIFFER_WLAN_FILTER_MAX) {
        idx = 0;
      }
    }
    wifi_filter_hash_table[idx].filter_name = wifi_filter_keys[i];
    wifi_filter_hash_table[idx].filter_val = wifi_filter_values[i];
  }
}

static uint32_t search_wifi_filter_hashtable(const char *key) {
  uint32_t len = strlen(key);
  uint32_t start_idx = hash_func(key, SNIFFER_WLAN_FILTER_MAX);
  uint32_t idx = start_idx;
  while (strncmp(wifi_filter_hash_table[idx].filter_name, key, len)) {
    idx++;
    if (idx >= SNIFFER_WLAN_FILTER_MAX) {
      idx = 0;
    }
    /* wrong key */
    if (idx == start_idx) {
      return 0;
    }
  }
  return wifi_filter_hash_table[idx].filter_val;
}

static void queue_packet(void *recv_packet,
                         sniffer_packet_info_t *packet_info) {
  /* Copy a packet from Link Layer driver and queue the copy to be processed by
   * sniffer task */
  void *packet_to_queue = malloc(packet_info->length);
  if (packet_to_queue) {
    memcpy(packet_to_queue, recv_packet, packet_info->length);
    packet_info->payload = packet_to_queue;
    if (snf_rt.work_queue) {
      /* send packet_info */
      if (xQueueSend(snf_rt.work_queue, packet_info,
                     pdMS_TO_TICKS(SNIFFER_PROCESS_PACKET_TIMEOUT_MS)) !=
          pdTRUE) {
        ESP_LOGE(TAG, "sniffer work queue full");
        free(packet_info->payload);
      }
    }
  } else {
    ESP_LOGE(TAG, "No enough memory for promiscuous packet");
  }
}

static void wifi_sniffer_cb(void *recv_buf, wifi_promiscuous_pkt_type_t type) {
  sniffer_packet_info_t packet_info;
  wifi_promiscuous_pkt_t *sniffer = (wifi_promiscuous_pkt_t *)recv_buf;
  /* prepare packet_info */
  packet_info.seconds = sniffer->rx_ctrl.timestamp / 1000000U;
  packet_info.microseconds = sniffer->rx_ctrl.timestamp % 1000000U;
  packet_info.length = sniffer->rx_ctrl.sig_len;

  /* For now, the sniffer only dumps the length of the MISC type frame */
  if (type != WIFI_PKT_MISC && !sniffer->rx_ctrl.rx_state) {
    packet_info.length -= SNIFFER_PAYLOAD_FCS_LEN;
    queue_packet(sniffer->payload, &packet_info);
  }
}

static void sniffer_task(void *parameters) {
  sniffer_packet_info_t packet_info;
  sniffer_runtime_t *sniffer = (sniffer_runtime_t *)parameters;
  bool force_exit = false;
  if (sniffer_animation_start_cb) {
    sniffer_animation_start_cb();
  }

  if (sniffer_cb) {
    sniffer_cb(sniffer);
  }

  while (sniffer->is_running) {
    // ESP_LOGI(TAG, "sniffer task running");
    if (sniffer->packets_to_sniff == 0) {
      sniffer_stop(sniffer);
      break;
    }
    /* receive packet info from queue */
    if (xQueueReceive(sniffer->work_queue, &packet_info,
                      pdMS_TO_TICKS(SNIFFER_PROCESS_PACKET_TIMEOUT_MS)) !=
        pdTRUE) {
      // ESP_LOGW(TAG, "receive packet info failed");
      continue;
    }
    if (packet_capture(packet_info.payload, packet_info.length,
                       packet_info.seconds,
                       packet_info.microseconds) != ESP_OK) {
      ESP_LOGW(TAG, "save captured packet failed");
      if (out_of_mem_cb) {
        xSemaphoreGive(sniffer->sem_task_over);
        force_exit = true;
        out_of_mem_cb();
      }
    }
    free(packet_info.payload);
    if (sniffer->packets_to_sniff > 0) {
      if (sniffer_cb) {
        sniffer_cb(sniffer);
      }
      sniffer->packets_to_sniff--;
      sniffer->sniffed_packets++;
      // TODO: Add a flag to make an animation when the sniffer is running
      ESP_LOGW(TAG, "%" PRIi32 " packages left to capture",
               sniffer->packets_to_sniff);
    }
  }
  /* notify that sniffer task is over */
  if (sniffer->packets_to_sniff != 0) {
    if (!force_exit) {
      xSemaphoreGive(sniffer->sem_task_over);
    }
  }
  if (sniffer_cb) {
    sniffer_cb(sniffer);
  }
  vTaskDelete(NULL);
}

static esp_err_t sniffer_stop(sniffer_runtime_t *sniffer) {
  sniffer->sniffed_packets = 0;
  if (sniffer_animation_stop_cb) {
    printf("sniffer_animation_stop_cb\n");
    sniffer_animation_stop_cb();
  }
  esp_err_t ret = ESP_OK;

  ESP_GOTO_ON_FALSE(sniffer->is_running, ESP_ERR_INVALID_STATE, err, TAG,
                    "sniffer is already stopped");

  switch (sniffer->interf) {
  case SNIFFER_INTF_WLAN:
    /* Disable wifi promiscuous mode */
    ESP_GOTO_ON_ERROR(esp_wifi_set_promiscuous(false), err, TAG,
                      "stop wifi promiscuous failed");
    break;
  case SNIFFER_INTF_ETH:
    // Ethernet support removed
  default:
    ESP_GOTO_ON_FALSE(false, ESP_ERR_INVALID_ARG, err, TAG,
                      "unsupported interface");
    break;
  }
  ESP_LOGI(TAG, "stop promiscuous ok");
  printf("sniffer->packets_to_sniff\n");

  /* stop sniffer local task */
  sniffer->is_running = false;
  /* wait for task over */
  if (sniffer->packets_to_sniff != 0) {
    printf("sniffer->packets_to_sniff: %ld \n", sniffer->packets_to_sniff);
    xSemaphoreTake(sniffer->sem_task_over, portMAX_DELAY);
  }
  printf("sniffer->xSemaphoreTake\n");

  vSemaphoreDelete(sniffer->sem_task_over);
  printf("sniffer->vSemaphoreDelete\n");
  sniffer->sem_task_over = NULL;
  /* make sure to free all resources in the left items */
  UBaseType_t left_items = uxQueueMessagesWaiting(sniffer->work_queue);
  printf("sniffer->uxQueueMessagesWaiting\n");

  sniffer_packet_info_t packet_info;
  while (left_items--) {
    xQueueReceive(sniffer->work_queue, &packet_info,
                  pdMS_TO_TICKS(SNIFFER_PROCESS_PACKET_TIMEOUT_MS));
    free(packet_info.payload);
  }
  printf("sniffer->packet_info\n");
  vQueueDelete(sniffer->work_queue);
  printf("sniffer->vQueueDelete\n");
  sniffer->work_queue = NULL;

  /* stop pcap session */
  sniff_packet_stop();
  printf("sniffer->sniff_packet_stop\n");
err:
  return ret;
}

static esp_err_t sniffer_start(sniffer_runtime_t *sniffer) {
  esp_err_t ret = ESP_OK;
  pcap_link_type_t link_type;
  wifi_promiscuous_filter_t wifi_filter;

  ESP_GOTO_ON_FALSE(!(sniffer->is_running), ESP_ERR_INVALID_STATE, err, TAG,
                    "sniffer is already running");

  switch (sniffer->interf) {
  case SNIFFER_INTF_WLAN:
    link_type = PCAP_LINK_TYPE_802_11;
    break;
  case SNIFFER_INTF_ETH:
    // Ethernet support removed
    break;
  default:
    ESP_GOTO_ON_FALSE(false, ESP_ERR_INVALID_ARG, err, TAG,
                      "unsupported interface");
    break;
  }

  /* init a pcap session */
  ESP_GOTO_ON_ERROR(sniff_packet_start(link_type), err, TAG,
                    "init pcap session failed");

  sniffer->is_running = true;
  sniffer->work_queue = xQueueCreate(CONFIG_SNIFFER_WORK_QUEUE_LEN,
                                     sizeof(sniffer_packet_info_t));
  ESP_GOTO_ON_FALSE(sniffer->work_queue, ESP_FAIL, err_queue, TAG,
                    "create work queue failed");
  sniffer->sem_task_over = xSemaphoreCreateBinary();
  ESP_GOTO_ON_FALSE(sniffer->sem_task_over, ESP_FAIL, err_sem, TAG,
                    "create work queue failed");
  ESP_GOTO_ON_FALSE(xTaskCreate(sniffer_task, "snifferT",
                                CONFIG_SNIFFER_TASK_STACK_SIZE, sniffer,
                                CONFIG_SNIFFER_TASK_PRIORITY, &sniffer->task),
                    ESP_FAIL, err_task, TAG, "create task failed");

  switch (sniffer->interf) {
  case SNIFFER_INTF_WLAN:
    /* Start WiFi Promiscuous Mode */
    wifi_filter.filter_mask = sniffer->filter;
    esp_wifi_set_promiscuous_filter(&wifi_filter);
    esp_wifi_set_promiscuous_rx_cb(wifi_sniffer_cb);
    ESP_GOTO_ON_ERROR(esp_wifi_set_promiscuous(true), err_start, TAG,
                      "create work queue failed");
    esp_wifi_set_channel(sniffer->channel, WIFI_SECOND_CHAN_NONE);
    ESP_LOGI(TAG, "start WiFi promiscuous ok");
    break;
  case SNIFFER_INTF_ETH:
    // Ethernet support removed
  default:
    break;
  }
  return ret;
err_start:
  vTaskDelete(sniffer->task);
  sniffer->task = NULL;
err_task:
  vSemaphoreDelete(sniffer->sem_task_over);
  sniffer->sem_task_over = NULL;
err_sem:
  vQueueDelete(sniffer->work_queue);
  sniffer->work_queue = NULL;
err_queue:
  sniffer->is_running = false;
err:
  out_of_mem_cb();
  return ret;
}

static struct {
  struct arg_str *interface;
  struct arg_str *filter;
  struct arg_int *channel;
  struct arg_lit *stop;
  struct arg_int *number;
  struct arg_end *end;
} sniffer_args;

int do_sniffer_cmd(int argc, char **argv) {
  int nerrors = arg_parse(argc, argv, (void **)&sniffer_args);

  if (nerrors != 0) {
    arg_print_errors(stderr, sniffer_args.end, argv[0]);
    return 0;
  }

  /* Check whether or not to stop sniffer: "--stop" option */
  if (sniffer_args.stop->count) {
    /* stop sniffer */
    sniffer_stop(&snf_rt);
    return 0;
  }

  /* Check interface: "-i" option */
  if (sniffer_args.interface->count) {
    if (!strncmp(sniffer_args.interface->sval[0], "wlan", 4)) {
      snf_rt.interf = SNIFFER_INTF_WLAN;
    } else {
      ESP_LOGE(TAG, "interface %s not found", sniffer_args.interface->sval[0]);
      return 1;
    }
  } else {
    snf_rt.interf = SNIFFER_INTF_WLAN;
    ESP_LOGW(TAG, "sniffing interface set to wlan by default");
  }

  /* Check channel: "-c" option */
  switch (snf_rt.interf) {
  case SNIFFER_INTF_WLAN:
    snf_rt.channel = SNIFFER_DEFAULT_CHANNEL;
    if (sniffer_args.channel->count) {
      snf_rt.channel = sniffer_args.channel->ival[0];
    }
    break;
  case SNIFFER_INTF_ETH:
    // Ethernet support removed
    break;
  default:
    break;
  }

  /* Check filter setting: "-F" option */
  switch (snf_rt.interf) {
  case SNIFFER_INTF_WLAN:
    if (sniffer_args.filter->count) {
      snf_rt.filter = 0;
      for (int i = 0; i < sniffer_args.filter->count; i++) {
        snf_rt.filter +=
            search_wifi_filter_hashtable(sniffer_args.filter->sval[i]);
      }
      /* When filter conditions are all wrong */
      if (snf_rt.filter == 0) {
        snf_rt.filter = WIFI_PROMIS_FILTER_MASK_ALL;
      }
    } else {
      snf_rt.filter = WIFI_PROMIS_FILTER_MASK_ALL;
    }
    break;
  case SNIFFER_INTF_ETH:
    // Ethernet support removed
  default:
    break;
  }

  /* Check the number of captured packages: "-n" option */
  snf_rt.packets_to_sniff = -1;
  if (sniffer_args.number->count) {
    snf_rt.packets_to_sniff = sniffer_args.number->ival[0];
    ESP_LOGI(TAG, "%" PRIi32 " packages will be captured",
             snf_rt.packets_to_sniff);
  }

  /* start sniffer */
  sniffer_start(&snf_rt);
  return 0;
}

void register_sniffer_cmd(void) {
#if !defined(CONFIG_CMD_SNIFFER_DEBUG)
  esp_log_level_set(TAG, ESP_LOG_NONE);
#endif

  sniffer_args.number = arg_int0("n", "number", "<num>",
                                 "the number of the packets to be captured");
  sniffer_args.interface = arg_str0("i", "interface", "<wlan|eth0|eth1|...>",
                                    "which interface to capture packet");
  sniffer_args.filter =
      arg_strn("F", "filter", "<mgmt|data|ctrl|misc|mpdu|ampdu|fcsfail>", 0, 7,
               "filter parameters");
  sniffer_args.channel =
      arg_int0("c", "channel", "<channel>", "communication channel to use");
  sniffer_args.stop = arg_lit0(NULL, "stop", "stop running sniffer");
  sniffer_args.end = arg_end(1);

  create_wifi_filter_hashtable();
}
