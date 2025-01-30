#include "menus.h"

#include "about.h"
#include "adv_scan_module.h"
#include "analyzer_scenes.h"
#include "catdos_module.h"
#include "deauth_module.h"
#include "detector_scenes.h"
#include "hid_module.h"
#include "menus_icons.h"
#include "neopixel_app.h"
#include "spam_module.h"
#include "ssid_spam.h"
#include "tama_scenes.h"
#include "trackers_module.h"

menu_t menus[] = { //////////////////////////////////
    {.display_name = "Main Menu",
     .menu_idx = MENU_MAIN,
     .parent_idx = -1,
     .last_selected_submenu = 0,
     .on_enter_cb = NULL,
     .on_exit_cb = NULL,
     .is_visible = false},
    {.display_name = "Wifi Apps",
     .menu_idx = MENU_WIFI_APPS,
     .parent_idx = MENU_MAIN,
     .icon = &menu_icon_wifi,
     .last_selected_submenu = 0,
     .on_enter_cb = NULL,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Analyzer",
     .menu_idx = MENU_WIFI_ANALIZER,
     .parent_idx = MENU_WIFI_APPS,
     .icon = &menu_icon_analyzer,
     .entry_cmd = "analyzer",
     .last_selected_submenu = 0,
     .on_enter_cb = analyzer_scenes_main_menu,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Deauth",
     .menu_idx = MENU_WIFI_DEAUTH,
     .parent_idx = MENU_WIFI_APPS,
     .icon = &menu_icon_deauth,
     .entry_cmd = "deauth",
     .last_selected_submenu = 0,
     .on_enter_cb = deauth_module_begin,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Deauth Scan",
     .menu_idx = MENU_WIFI_DEAUTH_SCAN,
     .parent_idx = MENU_WIFI_APPS,
     .icon = &menu_icon_deauth_scan,
     .entry_cmd = "deauth_scan",
     .last_selected_submenu = 0,
     .on_enter_cb = detector_scenes_main_menu,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "DoS",
     .menu_idx = MENU_WIFI_DOS,
     .parent_idx = MENU_WIFI_APPS,
     .icon = &menu_icon_dos,
     .entry_cmd = "dos",
     .last_selected_submenu = 0,
     .on_enter_cb = catdos_module_begin,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "SSID Spammer",
     .menu_idx = MENU_WIFI_SSID_SPAM,
     .parent_idx = MENU_WIFI_APPS,
     .icon = &menu_icon_ssid_spam,
     .entry_cmd = "ssid_spam",
     .last_selected_submenu = 0,
     .on_enter_cb = ssid_spam_begin,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Bluetooth Apps",
     .menu_idx = MENU_BLUETOOTH_APPS,
     .parent_idx = MENU_MAIN,
     .icon = &menu_icon_bt,
     .last_selected_submenu = 0,
     .on_enter_cb = NULL,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Trakers scan",
     .menu_idx = MENU_BLUETOOTH_TRAKERS_SCAN,
     .parent_idx = MENU_BLUETOOTH_APPS,
     .icon = &menu_icon_trakers,
     .entry_cmd = "trakers_scan",
     .last_selected_submenu = 0,
     .on_enter_cb = trackers_module_begin,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Spam",
     .menu_idx = MENU_BLUETOOTH_SPAM,
     .parent_idx = MENU_BLUETOOTH_APPS,
     .icon = &menu_icon_spam,
     .entry_cmd = "spam",
     .last_selected_submenu = 0,
     .on_enter_cb = ble_module_begin,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "HID",
     .menu_idx = MENU_BLUETOOTH_HID,
     .parent_idx = MENU_BLUETOOTH_APPS,
     .icon = &menu_icon_hid,
     .entry_cmd = "hid",
     .last_selected_submenu = 0,
     .on_enter_cb = hid_module_begin,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "ADV Scanner",
     .menu_idx = MENU_BLUETOOTH_ADV,
     .parent_idx = MENU_BLUETOOTH_APPS,
     .icon = &menu_icon_adv_scanner,
     .last_selected_submenu = 0,
     .on_enter_cb = adv_scanner_module_begin,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Tamagotchi",
     .menu_idx = MENU_TAMAGOTCHI,
     .parent_idx = MENU_MAIN,
     .icon = &menu_icon_tama,
     .last_selected_submenu = 0,
     .on_enter_cb = tama_scenes_main,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Neopixel Control",
     .menu_idx = MENU_NEOPIXELS,
     .parent_idx = MENU_MAIN,
     .icon = NULL,
     .last_selected_submenu = 0,
     .on_enter_cb = neopixel_app,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "About",
     .menu_idx = MENU_ABOUT,
     .parent_idx = MENU_MAIN,
     .icon = &menu_icon_about,
     .last_selected_submenu = 0,
     .on_enter_cb = about_main,
     .on_exit_cb = NULL,
     .is_visible = true}};

uint8_t get_menus_count() { return sizeof(menus) / sizeof(menu_t); }