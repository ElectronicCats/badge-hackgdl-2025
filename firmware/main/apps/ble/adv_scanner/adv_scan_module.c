#include "adv_scan_module.h"

#include "esp_log.h"
#include "esp_mac.h"

#include "adv_scan_screens.h"
#include "adv_scanner_scenes.h"
#include "ble_scann.h"
#include "menus_module.h"
#include "preferences.h"

void adv_scanner_module_set_filter(uint8_t selection) {
  preferences_put_uchar(SCAN_FILTER_MEM, selection);
  set_filter_type(selection);
}

void adv_scanner_module_set_scan_type(uint8_t selection) {
  preferences_put_uchar(SCAN_TYPE_MEM, selection);
  set_scan_type(selection);
}

void adv_scanner_module_start() {
  adv_scanner_scenes_scanning_card("Scanning", "Scanning for devices");
  ble_scanner_register_cb(adv_scanner_display_record);
  ble_scanner_begin();
}
void adv_scanner_module_begin() { adv_scanner_scenes_main_menu(); }
