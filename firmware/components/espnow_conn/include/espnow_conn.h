#pragma once

#include "esp_err.h"
#include "espnow.h"
#include <stdint.h>

typedef struct {
  uint8_t *src_addr;
  void *data;
  size_t data_size;
  wifi_pkt_rx_ctrl_t *rx_ctrl;
} espnow_conn_rx_data_t;

typedef void (*espnow_conn_rx_cb_t)(espnow_conn_rx_data_t *msg);

void espnow_conn_begin();

void espnow_conn_deinit();

void espnow_conn_send(uint8_t *addr, void *data, size_t data_size);

void espnow_conn_set_rx_cb(espnow_conn_rx_cb_t cb);
