#include "tama_now.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

#include "espnow_conn.h"

#include "tama_display.h"
#include "tama_friends.h"

#define RSSI -50
#define ADV_RATE_MS 5000

#define MAGIC *(uint8_t *)msg->data
#define CMD *(uint8_t *)((msg->data) + 1)

static TaskHandle_t adv_task_handler = NULL;
static bool is_running = false;
static bool is_active = false;

static void adv_res_send(uint8_t *mac, char *nickname);

/////////////////////////////////////////////////////////////

static void adv_req_send() {
  tama_now_adv_req_cmd_t cmd;
  cmd.magic = TAMA_NOW_MAGIC;
  cmd.cmd = ADV_REQ_CMD;

  espnow_conn_send(espnow_conn_broadcast_mac, &cmd,
                   sizeof(tama_now_adv_req_cmd_t));
}

static void adv_req_handler(espnow_conn_rx_data_t *msg) {
  if (msg->len != sizeof(tama_now_adv_req_cmd_t)) {
    printf("CMD: %d | LEN: %d -> MISSMATCH ERR\n",
           sizeof(tama_now_adv_req_cmd_t), msg->len);
    return;
  }

  char *nickname = tama_display_get_nickname();
  adv_res_send(msg->rx_info->src_addr, nickname);
}

/////////////////////////////////////////////////////////////

static void adv_res_send(uint8_t *mac, char *nickname) {
  tama_now_adv_res_cmd_t cmd;
  cmd.magic = TAMA_NOW_MAGIC;
  cmd.cmd = ADV_RES_CMD;
  memcpy(cmd.nickname, nickname, 13);

  espnow_conn_send(mac, &cmd, sizeof(tama_now_adv_res_cmd_t));
}

static void adv_res_handler(espnow_conn_rx_data_t *msg) {
  if (msg->len != sizeof(tama_now_adv_res_cmd_t)) {
    printf("CMD: %d | LEN: %d -> MISSMATCH ERR\n",
           sizeof(tama_now_adv_res_cmd_t), msg->len);
    return;
  }

  tama_now_adv_res_cmd_t *cmd = (tama_now_adv_res_cmd_t *)msg->data;
  tama_friends_add(cmd->nickname, msg->rx_info->src_addr);
}

/////////////////////////////////////////////////////////////

static void cmd_handler(espnow_conn_rx_data_t *msg) {
  if (!is_active) {
    return;
  }
  if (MAGIC != TAMA_NOW_MAGIC || msg->rx_info->rx_ctrl->rssi < RSSI) {
    return;
  }
  switch (CMD) {
  case ADV_REQ_CMD:
    adv_req_handler(msg);
    break;
  case ADV_RES_CMD:
    adv_res_handler(msg);
    break;
  default:
    break;
  }
}

/////////////////////////////////////////////////////////////

static void adv_task() {
  while (1) {
    adv_req_send();
    vTaskDelay(pdMS_TO_TICKS(ADV_RATE_MS));
  }
}

void tama_now_begin() {
  if (!is_running) {
    is_running = true;
    espnow_conn_begin();
  }
  is_active = true;
  espnow_conn_set_rx_cb(cmd_handler);

  xTaskCreate(adv_task, "adv_task", 2048, NULL, 5, &adv_task_handler);
}

void tama_now_deinit() {
  vTaskDelete(adv_task_handler);
  is_active = false;
  // espnow_conn_set_rx_cb(NULL);
  // espnow_conn_deinit();
}