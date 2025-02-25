#include "esp_gap_ble_api.h"
#include "general_screens.h"
#include <stdbool.h>
#include <stdint.h>
#ifndef ADV_SCAN_SCREENS_H
#define ADV_SCAN_SCREENS_H

char *evt_adv_type[5] = {"IND", "DIRECT_IND", "SCAN_IND", "NONCONN_IND",
                         "SCAN_RSP"};

void adv_scanner_module_register_menu(menu_tree_t menu);
void adv_scanner_clear_screen();
void adv_scanner_module_display_menu(uint16_t current_item);
void adv_scanner_display_record(esp_ble_gap_cb_param_t *record);
#endif // ADV_SCAN_SCREENS_H
