#include "espnow_conn.h"

#include <stdio.h>

#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "espnow_storage.h"
#include "espnow_utils.h"

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 4, 0)
#include "esp_mac.h"
#endif

static const char *TAG = "espnow_conn";

espnow_conn_rx_cb_t badge_connect_recv_cb = NULL;

esp_err_t badge_connect_recv(uint8_t *src_addr, void *payload,
                             size_t payload_size, wifi_pkt_rx_ctrl_t *rx_ctrl);

void espnow_conn_begin() {
  espnow_config_t espnow_config = ESPNOW_INIT_CONFIG_DEFAULT();
  espnow_init(&espnow_config);
  espnow_set_config_for_data_type(ESPNOW_DATA_TYPE_DATA, true,
                                  badge_connect_recv);
}

void espnow_conn_deinit() {
  espnow_deinit();
  esp_wifi_stop();
  esp_wifi_deinit();
  ESP_LOGI(TAG, "Badge connect module deinitialized");
}

void espnow_conn_send(uint8_t *addr, void *data, size_t data_size) {
  uint8_t *payload = (uint8_t *)malloc(data_size);
  if (!payload) {
    ESP_LOGE(TAG, "Failed to allocate memory for data");
    return;
  }

  memcpy(payload, data, data_size);

  espnow_frame_head_t frame_head = {
      .retransmit_count = 5,
      .broadcast = true,
  };

  esp_err_t ret = espnow_send(ESPNOW_DATA_TYPE_DATA, addr, payload, data_size,
                              &frame_head, portMAX_DELAY);
  free(payload);
}

esp_err_t badge_connect_recv(uint8_t *src_addr, void *payload,
                             size_t payload_size, wifi_pkt_rx_ctrl_t *rx_ctrl) {
  ESP_PARAM_CHECK(src_addr);
  ESP_PARAM_CHECK(payload);
  ESP_PARAM_CHECK(payload_size);
  ESP_PARAM_CHECK(rx_ctrl);

  char *data = malloc(payload_size);
  if (!data) {
    ESP_LOGE(TAG, "Failed to allocate memory for received data");
    return ESP_FAIL;
  }

  memcpy(data, payload, payload_size);

  espnow_conn_rx_data_t msg = {
      .src_addr = src_addr,
      .data = (void *)data,
      .data_size = payload_size,
      .rx_ctrl = rx_ctrl,
  };

  if (badge_connect_recv_cb) {
    badge_connect_recv_cb(&msg);
  }
  free(data);
  return ESP_OK;
}

void espnow_conn_set_rx_cb(espnow_conn_rx_cb_t cb) {
  badge_connect_recv_cb = cb;
}
