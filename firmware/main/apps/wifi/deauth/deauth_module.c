#include "animations_task.h"
#include "captive_portal.h"
#include "deauth_screens.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include <stdint.h>
#include <string.h>
// #include "led_events.h"
#include "deauth_scenes.h"
#include "menus_module.h"
#include "neopixels_events.h"
#include "wifi_attacks.h"
#include "wifi_controller.h"
#include "wifi_scanner.h"

#define DEFAULT_SCAN_LIST_SIZE CONFIG_SCAN_MAX_AP
#define SCAN_RETRIES 2

typedef enum {
  DEAUTH_STATE_IDLE = 0,
  DEAUTH_STATE_MENU,
  DEAUTH_STATE_RUNNING,
  DEAUTH_STATE_STOP,
  DEAUTH_STATE_MAX
} deauth_state_t;

typedef struct {
  deauth_state_t state;
  wifi_config_t wifi_config;
} wifi_module_t;

typedef struct {
  uint16_t count;
  wifi_ap_record_t records[CONFIG_SCAN_MAX_AP];
} scanned_ap_records_t;

static wifi_module_t current_wifi_state;
static scanned_ap_records_t *ap_records;
static uint16_t current_item = 0;
static menu_stadistics_t menu_stadistics = {0};

// static void deauth_module_cb_event(uint8_t button_name, uint8_t
// button_event); static void deauth_module_cb_event_select_ap(uint8_t
// button_name,
//                                              uint8_t button_event);
// static void deauth_module_cb_event_attacks(uint8_t button_name,
//                                            uint8_t button_event);
static void deauth_module_cb_event_run(uint8_t button_name,
                                       uint8_t button_event);
// static void deauth_module_cb_event_captive_portal(uint8_t button_name,
//                                                   uint8_t button_event);

static void scanning_task();
static void deauth_run_scan_task();
static void deauth_handle_attacks();

static void scanning_task() {
  neopixel_events_run_event(neopixel_scanning_event);
  menus_module_disable_input();
  uint8_t scan_count = 0;
  while (ap_records->count < (DEFAULT_SCAN_LIST_SIZE / 2) &&
         scan_count < SCAN_RETRIES) {
    wifi_scanner_module_scan();
    vTaskDelay(20 / portTICK_PERIOD_MS);
    scan_count++;
  }
  ap_records = wifi_scanner_get_ap_records();
  menu_stadistics.count = ap_records->count;
  animations_task_stop();

  ESP_LOGI("deauth", "Scanning done: %d", ap_records->count);
  // deauth_display_menu(current_item, menu_stadistics);
  current_wifi_state.state = DEAUTH_STATE_MENU;
  neopixel_events_stop_event();
  menus_module_enable_input();
  deauth_scenes_main_menu();
  // led_control_stop();
  vTaskDelete(NULL);
}

static void deauth_run_scan_task() {
  ap_records = wifi_scanner_get_ap_records();
  menu_stadistics.count = ap_records->count;
  while (ap_records->count < (DEFAULT_SCAN_LIST_SIZE / 2)) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

static void deauth_handle_attacks() {
  wifi_attacks_module_stop();
  // led_control_run_effect(led_control_wifi_attacking);
  neopixel_events_run_event(neopixel_scanning_event);

  switch (menu_stadistics.attack) {
  case BROADCAST:
  case ROGUE_AP:
  case COMBINED:
    ESP_LOGI("deauth", "Attack: %d", menu_stadistics.attack);
    deauth_display_attaking_text();
    animations_task_run(&deauth_display_attacking_animation, 200, NULL);
    ESP_LOGI("deauth", "Attack: %d", menu_stadistics.attack);
    wifi_attack_handle_attacks(
        menu_stadistics.attack,
        &ap_records->records[menu_stadistics.selected_ap]);
    ESP_LOGI("deauth", "Attack: %d", menu_stadistics.attack);
    menus_module_set_app_state(true, deauth_module_cb_event_run);
    current_item = 0;
    break;
  case CAPTIVEPORTAL:
    current_item = 0;
    // menus_module_set_app_state(true, deauth_module_cb_event_captive_portal);
    // deauth_display_captive_portals(current_item, menu_stadistics);
    deauth_scenes_portal_selection();
    break;
  default:
    break;
  }
}

void deauth_module_begin() {
  deauth_clear_screen();
  deauth_display_scanning_text();
  animations_task_run(&deauth_display_scanning, 200, NULL);

  current_wifi_state.state = DEAUTH_STATE_IDLE;

  memset(&current_wifi_state.wifi_config, 0, sizeof(wifi_config_t));
  current_wifi_state.wifi_config = wifi_driver_access_point_begin();

  ap_records = wifi_scanner_get_ap_records();
  if (ap_records == NULL) {
    ESP_LOGE("deauth", "Failed to get ap records");
    return;
  }
  ap_records->count = 0;

  xTaskCreate(scanning_task, "wifi_scan", 4096, NULL, 5, NULL);
  deauth_run_scan_task();

  // menus_module_set_app_state(true, deauth_module_cb_event);

  // menu_stadistics.attack = 99;

  // led_control_run_effect(led_control_wifi_scanning);
}

void deauth_module_scan() {
  current_wifi_state.state = DEAUTH_STATE_IDLE;
  wifi_scanner_clear_ap_records();
  deauth_clear_screen();
  deauth_display_scanning_text();
  animations_task_run(&deauth_display_scanning, 200, NULL);
  xTaskCreate(scanning_task, "wifi_scan", 4096, NULL, 5, NULL);
  deauth_run_scan_task();
  // menus_module_set_app_state(true, deauth_module_cb_event);
}

static char *ap_names[20] = {0};

void deauth_module_select_ap() {
  // menus_module_set_app_state(true, deauth_module_cb_event_select_ap);
  // deauth_display_scanned_ap(ap_records->records, ap_records->count,
  //                           current_item);
  for (uint8_t i = 0; i < ap_records->count; i++) {
    ap_names[i] = (char *)ap_records->records[i].ssid;
  }
  deauth_scenes_ap_selection(ap_names, ap_records->count,
                             menu_stadistics.selected_ap);
}
void deauth_module_select_attack() {
  // menus_module_set_app_state(true, deauth_module_cb_event_attacks);
  // deauth_display_attacks(current_item, menu_stadistics);
  deauth_scenes_attack_selection(menu_stadistics.attack);
}
void deauth_module_run() {
  if (ap_records->records[menu_stadistics.selected_ap].bssid[0] == 0) {
    deauth_display_warning_not_ap_selected();
    vTaskDelay(1500 / portTICK_PERIOD_MS);
    // deauth_display_menu(current_item, menu_stadistics);
    deauth_scenes_main_menu();
    return;
  }
  if (menu_stadistics.attack == 99) {
    deauth_display_warning_not_attack_selected();
    vTaskDelay(1500 / portTICK_PERIOD_MS);
    // deauth_display_menu(current_item, menu_stadistics);
    deauth_scenes_main_menu();
    return;
  }
  deauth_clear_screen();
  deauth_handle_attacks();
}

void deauth_module_exit() {
  wifi_scanner_clear_ap_records();
  menus_module_restart();
  // led_control_stop();
}

// static void deauth_module_cb_event(uint8_t button_name, uint8_t button_event)
// {
//   if (button_event != BUTTON_PRESS_DOWN)
//   {
//     return;
//   }
//   if (current_wifi_state.state == DEAUTH_STATE_IDLE)
//   {
//     return;
//   }

//   switch (button_name)
//   {
//   case BUTTON_MIDDLE:
//     current_item = current_item-- == 0 ? MENUCOUNT - 1 : current_item;
//     deauth_display_menu(current_item, menu_stadistics);
//     break;
//   case BUTTON_BACK:
//     current_item = ++current_item > MENUCOUNT - 1 ? 0 : current_item;
//     deauth_display_menu(current_item, menu_stadistics);
//     break;
//   case BUTTON_RIGHT:
//     switch (current_item)
//     {
//     case SCAN:
//       current_wifi_state.state = DEAUTH_STATE_IDLE;
//       wifi_scanner_clear_ap_records();
//       deauth_clear_screen();
//       deauth_display_scanning_text();
//       animations_task_run(&deauth_display_scanning, 200, NULL);
//       xTaskCreate(scanning_task, "wifi_scan", 4096, NULL, 5, NULL);
//       deauth_run_scan_task();
//       menus_module_set_app_state(true, deauth_module_cb_event);
//       current_item = 0;
//       break;
//     case SELECT:
//       current_item = 0;
//       menus_module_set_app_state(true, deauth_module_cb_event_select_ap);
//       deauth_display_scanned_ap(ap_records->records, ap_records->count,
//                                 current_item);
//       break;
//     case DEAUTH:
//       current_item = 0;
//       menus_module_set_app_state(true, deauth_module_cb_event_attacks);
//       deauth_display_attacks(current_item, menu_stadistics);
//       break;
//     case RUN:
//       if (menu_stadistics.selected_ap.bssid[0] == 0)
//       {
//         deauth_display_warning_not_ap_selected();
//         vTaskDelay(1500 / portTICK_PERIOD_MS);
//         deauth_display_menu(current_item, menu_stadistics);
//         break;
//       }
//       if (menu_stadistics.attack == 99)
//       {
//         deauth_display_warning_not_attack_selected();
//         vTaskDelay(1500 / portTICK_PERIOD_MS);
//         deauth_display_menu(current_item, menu_stadistics);
//         break;
//       }
//       deauth_clear_screen();
//       deauth_handle_attacks();
//       break;
//     default:
//       current_item = 0;
//       break;
//     }
//     break;
//   case BUTTON_LEFT:
//     wifi_scanner_clear_ap_records();
//     printf("Exit deauth: %d\n", current_item);
//     menus_module_restart();
//     // led_control_stop();
//     break;
//   default:
//     break;
//   }
// }

void deauth_module_set_ap(uint8_t selection) {
  menu_stadistics.selected_ap = selection;
  // menus_module_set_app_state(true, deauth_module_cb_event);
  // deauth_display_menu(current_item, menu_stadistics);
}

// static void deauth_module_cb_event_select_ap(uint8_t button_name,
//                                              uint8_t button_event)
// {
//   if (button_event != BUTTON_PRESS_DOWN)
//   {
//     return;
//   }
//   switch (button_name)
//   {
//   case BUTTON_MIDDLE:
//     current_item = current_item-- == 0 ? ap_records->count - 1 :
//     current_item; deauth_display_scanned_ap(ap_records->records,
//     ap_records->count,
//                               current_item);
//     break;
//   case BUTTON_BACK:
//     current_item = ++current_item > ap_records->count - 1 ? 0 : current_item;
//     deauth_display_scanned_ap(ap_records->records, ap_records->count,
//                               current_item);
//     break;
//   case BUTTON_RIGHT:
//     menu_stadistics.selected_ap = ap_records->records[current_item];
//     menus_module_set_app_state(true, deauth_module_cb_event);
//     deauth_display_menu(current_item, menu_stadistics);
//     current_item = 0;
//     break;
//   case BUTTON_LEFT:
//     current_item = 0;
//     menus_module_set_app_state(true, deauth_module_cb_event);
//     deauth_display_menu(current_item, menu_stadistics);
//     break;
//   default:
//     break;
//   }
// }

void deauth_module_set_attack(uint8_t selection) {
  menu_stadistics.attack = selection;
  // menus_module_set_app_state(true, deauth_module_cb_event);
  // deauth_display_menu(current_item, menu_stadistics);
}

// static void deauth_module_cb_event_attacks(uint8_t button_name,
//                                            uint8_t button_event)
// {
//   if (button_event != BUTTON_PRESS_DOWN)
//   {
//     return;
//   }
//   switch (button_name)
//   {
//   case BUTTON_MIDDLE:
//     current_item = current_item-- == 0 ? ATTACKSCOUNT - 1 : current_item;
//     deauth_display_attacks(current_item, menu_stadistics);
//     break;
//   case BUTTON_BACK:
//     current_item = ++current_item > ATTACKSCOUNT - 1 ? 0 : current_item;
//     deauth_display_attacks(current_item, menu_stadistics);
//     break;
//   case BUTTON_RIGHT:
//     menu_stadistics.attack = current_item;
//     menus_module_set_app_state(true, deauth_module_cb_event);

//     current_item = 0;
//     deauth_display_menu(current_item, menu_stadistics);
//     break;
//   case BUTTON_LEFT:
//     current_item = 0;
//     menus_module_set_app_state(true, deauth_module_cb_event);
//     deauth_display_menu(current_item, menu_stadistics);
//     break;
//   default:
//     break;
//   }
// }

void deauth_module_run_exit() {
  // captive_portal_stop();
  // led_control_stop();
  neopixel_events_stop_event();
  animations_task_stop();
  wifi_attacks_module_stop();
  // menus_module_set_app_state(true, deauth_module_cb_event);
  deauth_display_menu(current_item, menu_stadistics);
  deauth_scenes_main_menu();
}
static void deauth_module_cb_event_run(uint8_t button_name,
                                       uint8_t button_event) {
  if (button_event != BUTTON_PRESS_DOWN) {
    return;
  }
  switch (button_name) {
  case BUTTON_BACK:
    // captive_portal_stop();
    // led_control_stop();
    neopixel_events_stop_event();
    animations_task_stop();
    wifi_attacks_module_stop();
    // menus_module_set_app_state(true, deauth_module_cb_event);
    // deauth_display_menu(current_item, menu_stadistics);
    deauth_scenes_main_menu();
    break;
  default:
    break;
  }
}

void deauth_module_set_captive_portal_settings(uint8_t selection) {
  // led_control_run_effect(led_control_wifi_scanning);
  neopixel_events_run_event(neopixel_scanning_event);
  captive_portal_set_portal(selection);
  captive_portal_set_config_ssid(
      ap_records->records[menu_stadistics.selected_ap]);
  captive_portal_register_cb(deauth_display_captive_portal_creds);
  deauth_display_captive_waiting();
  xTaskCreate(captive_portal_begin, "captive_portal_start", 4096, NULL, 5,
              NULL);
  menus_module_set_app_state(true, deauth_module_cb_event_run);
}

// static void deauth_module_cb_event_captive_portal(uint8_t button_name,
//                                                   uint8_t button_event)
// {
//   if (button_event != BUTTON_PRESS_DOWN)
//   {
//     return;
//   }
//   switch (button_name)
//   {
//   case BUTTON_MIDDLE:
//     current_item = current_item-- == 0 ? CAPTIVEPORTALCOUNT - 1 :
//     current_item; deauth_display_captive_portals(current_item,
//     menu_stadistics); break;
//   case BUTTON_BACK:
//     current_item = ++current_item > CAPTIVEPORTALCOUNT - 1 ? 0 :
//     current_item; deauth_display_captive_portals(current_item,
//     menu_stadistics); break;
//   case BUTTON_LEFT:
//     current_item = 0;
//     // led_control_stop();
//     captive_portal_stop();
//     menus_module_set_app_state(true, deauth_module_cb_event);
//     deauth_display_menu(current_item, menu_stadistics);
//     break;
//   case BUTTON_RIGHT:
//     // led_control_run_effect(led_control_wifi_scanning);
//     captive_portal_set_portal(current_item);
//     captive_portal_set_config_ssid(menu_stadistics.selected_ap);
//     captive_portal_register_cb(deauth_display_captive_portal_creds);
//     deauth_display_captive_waiting();
//     xTaskCreate(captive_portal_begin, "captive_portal_start", 4096, NULL, 5,
//                 NULL);
//     menus_module_set_app_state(true, deauth_module_cb_event_run);
//     current_item = 0;
//     break;
//   default:
//     break;
//   }
// }