#include "menus.h"

#include "menus_icons.h"
#include "tama_scenes.h"

menu_t menus[] = { //////////////////////////////////
    {.display_name = "Must Not See This",
     .menu_idx = MENU_MAIN,
     .parent_idx = -1,
     .last_selected_submenu = 0,
     .on_enter_cb = NULL,
     .on_exit_cb = NULL,
     .is_visible = false},
    {.display_name = "Wifi",
     .menu_idx = MENU_WIFI,
     .parent_idx = MENU_MAIN,
     .icon = &menu_icon_wifi,
     .last_selected_submenu = 0,
     .on_enter_cb = NULL,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Bluetooth",
     .menu_idx = MENU_BLUETOOTH,
     .parent_idx = MENU_MAIN,
     .icon = &menu_icon_bt,
     .last_selected_submenu = 0,
     .on_enter_cb = NULL,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Tamagotchi",
     .menu_idx = MENU_TAMAGOTCHI,
     .parent_idx = MENU_MAIN,
     .icon = &menu_icon_tama,
     .last_selected_submenu = 0,
     .on_enter_cb = tama_scenes_main,
     .on_exit_cb = NULL,
     .is_visible = true}};

uint8_t get_menus_count() { return sizeof(menus) / sizeof(menu_t); }