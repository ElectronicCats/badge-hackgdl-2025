#include "espnow_conn.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_mac.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#define TAG "ESPNOW_CONN"

#define MAC_SIZE 6
#define ESPNOW_PMK "cat1234567890123"

static espnow_conn_rx_cb_t espnow_conn_rx_cb = NULL;
static uint8_t broadcast_mac[MAC_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// static void dump_mac(uint8_t *mac, const char *str)
// {
//   printf("%s: %02X:%02X:%02X:%02X:%02X:%02X\n", str, mac[0], mac[1], mac[2],
//   mac[3], mac[4], mac[5]);
// }

static void wifi_init(void) {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));
}

static void espnow_recv_cb(const esp_now_recv_info_t *recv_info,
                           const uint8_t *data, int data_len) {
  if (!recv_info || !data || data_len <= 0) {
    ESP_LOGE(TAG, "Receive callback argument error");
    return;
  }

  uint8_t *mac_addr = recv_info->src_addr;
  uint8_t *des_addr = recv_info->des_addr;

  // dump_mac(des_addr, "DES MAC");
  // dump_mac(mac_addr, "SRC MAC");

  espnow_conn_rx_data_t msg = {0};
  msg.rx_info = recv_info;
  msg.data = data;
  msg.len = data_len;

  if (espnow_conn_rx_cb) {
    espnow_conn_rx_cb(&msg);
  }

  // printf("COUNT: %d\n", *(uint8_t *)msg.data);
}

void espnow_conn_send(uint8_t *addr, void *data, size_t len) {
  esp_err_t err = esp_now_send(addr, data, len);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "ERR: %s\n", esp_err_to_name(err));
  }
}

void espnow_conn_add_peer(uint8_t *mac) {
  esp_now_peer_info_t peer = {0};
  peer.channel = 1;
  peer.ifidx = ESP_IF_WIFI_AP;
  peer.encrypt = false;
  memcpy(peer.peer_addr, mac, MAC_SIZE);
  ESP_ERROR_CHECK(esp_now_add_peer(&peer));
}

static esp_err_t espnow_init() {
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_recv_cb));
  ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)ESPNOW_PMK));

  espnow_conn_add_peer(broadcast_mac);
  return ESP_OK;
}

void espnow_conn_deinit() {
  esp_now_deinit();
  // esp_wifi_deinit();
}

// static void sender_task()
// {
//   uint8_t x = 0;
//   while (1)
//   {
//     espnow_conn_send(dest_mac, &x, 1);
//     x++;
//     vTaskDelay(pdMS_TO_TICKS(500));
//   }
// }

void espnow_conn_begin() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  wifi_init();
  espnow_init();

  vTaskDelay(pdMS_TO_TICKS(3000));
  espnow_conn_deinit();
  // xTaskCreate(sender_task, "task", 2048, NULL, 10, NULL);
}

void espnow_conn_set_rx_cb(espnow_conn_rx_cb_t cb) { espnow_conn_rx_cb = cb; }