#include "menus.h"

#include "tama_app.h"

menu_t menus[] = { //////////////////////////////////
    {.display_name = "Must Not See This",
     .menu_idx = MENU_MAIN,
     .parent_idx = -1,
     .last_selected_submenu = 0,
     .on_enter_cb = NULL,
     .on_exit_cb = NULL,
     .is_visible = false},
    {.display_name = "Applications",
     .menu_idx = MENU_APPLICATIONS,
     .parent_idx = MENU_MAIN,
     .last_selected_submenu = 0,
     .on_enter_cb = NULL,
     .on_exit_cb = NULL,
     .is_visible = true},
    {.display_name = "Tamagotchi",
     .menu_idx = MENU_TAMAGOTCHI,
     .parent_idx = MENU_MAIN,
     .last_selected_submenu = 0,
     .on_enter_cb = tama_app_begin,
     .on_exit_cb = NULL,
     .is_visible = true}};

uint8_t get_menus_count() { return sizeof(menus) / sizeof(menu_t); }