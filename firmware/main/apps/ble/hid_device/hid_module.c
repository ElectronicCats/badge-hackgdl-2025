#include "hid_module.h"

#include "esp_log.h"
#include "esp_mac.h"

#include "ble_hidd_main.h"
#include "general_notification.h"
#include "hid_scenes.h"
#include "hid_screens.h"
#include "menus_module.h"

// static uint16_t current_item = 0;

// static void hid_module_cb_event(uint8_t button_name, uint8_t button_event);
static void hid_module_cb_event_volumen(uint8_t button_name,
                                        uint8_t button_event);
static void hid_module_cb_connection_handler(bool connection);

static void hid_module_cb_connection_handler(bool connection) {
  hid_module_display_device_connection(connection);
  if (!connection) {
    // esp_restart();
  }
}

void hid_module_control_vol_up() {
  ble_hid_volume_up(true);
  ble_hid_volume_up(false);
  hid_module_display_notify_volumen_up();
}
void hid_module_control_vol_down() {
  ble_hid_volume_down(true);
  ble_hid_volume_down(false);
  hid_module_display_notify_volumen_down();
}
void hid_module_control_play_pause() {
  ble_hid_play(true);
  ble_hid_play(false);
  hid_module_display_notify_play_pause();
}

void hid_module_exit() { menus_module_restart(); }

void hid_module_name() {
  char *hid_name[20];
  ble_hid_get_device_name(&hid_name);
  hid_scenes_notification_handler("Device Name", &hid_name);
  // general_screen_display_card_information_handler(
  //     "Device Name", &hid_name, hid_module_reset_menu, hid_module_cb_event);
}
void hid_module_mac() {
  uint8_t hid_mac[8] = {0};
  esp_read_mac(hid_mac, ESP_MAC_BT);
  char mac_address[20];
  sprintf(mac_address, "%02X:%02X:%02X:%02X", hid_mac[2], hid_mac[3],
          hid_mac[4], hid_mac[5]);
  hid_scenes_notification_handler("Device MAC", &mac_address);
  // general_screen_display_card_information_handler(
  //     "Device MAC", &mac_address, hid_module_reset_menu,
  //     hid_module_cb_event);
}
void hid_module_start() {
  // hid_module_register_menu(GENERAL_TREE_APP_SUBMENU);
  hid_module_display_device_pairing();
  ble_hid_register_callback(hid_module_cb_connection_handler);
  ble_hid_begin();
  // menus_module_set_app_state(true, hid_module_cb_event_volumen);
}

void hid_module_begin() {
  // hid_module_register_menu(GENERAL_TREE_APP_MENU);
  // hid_module_display_menu(current_item);
  // menus_module_set_app_state(true, hid_module_cb_event);
  hid_scenes_main_menu();
}
