#pragma once

#include "esp_err.h"
#include "esp_now.h"
#include <stdint.h>

#define MAC_SIZE 6
uint8_t espnow_conn_broadcast_mac[MAC_SIZE] = {0xFF, 0xFF, 0xFF,
                                               0xFF, 0xFF, 0xFF};

typedef struct {
  esp_now_recv_info_t *rx_info;
  void *data;
  size_t len;
} espnow_conn_rx_data_t;

typedef void (*espnow_conn_rx_cb_t)(espnow_conn_rx_data_t *msg);

void espnow_conn_begin();

void espnow_conn_deinit();

void espnow_conn_send(uint8_t *addr, void *data, size_t len);

void espnow_conn_set_rx_cb(espnow_conn_rx_cb_t cb);
