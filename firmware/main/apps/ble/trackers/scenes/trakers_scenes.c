#include "trakers_scenes.h"

#include "general_notification.h"
#include "general_scrolling_text.h"
#include "general_submenu.h"
#include "menus_module.h"
#include "trackers_module.h"
#include "trackers_screens.h"

void trakers_scenes_main_menu();
void trakers_scenes_list_menu(char **trakers, uint8_t count);
void trakers_scenes_no_devices_found();
void trakers_scenes_searching();
void trakers_scenes_device_found();
void traker_scenes_traker_details(char **traker_details);

////////////////   MAIN MENU   /////////////////
typedef enum {
  MAIN_MENU_SCAN,
  MAIN_MENU_LIST,
} main_menu_option_t;

static char *main_menu_options[] = {"Scan", "List"};

void set_scan_text(char *text) { main_menu_options[0] = text; }

static void main_menu_handler(uint8_t selection) {
  switch (selection) {
  case MAIN_MENU_SCAN:
    trakers_module_scan();
    break;
  case MAIN_MENU_LIST:
    trakers_screens_list_trakers();
    break;
  default:
    break;
  }
}

static void main_menu_exit() { menus_module_restart(); }

void trakers_scenes_main_menu() {
  general_submenu_menu_t main_menu = {0};
  memset(&main_menu, 0, sizeof(general_submenu_menu_t));
  main_menu.title = "Trakers Scan";
  main_menu.options = main_menu_options;
  main_menu.options_count = sizeof(main_menu_options) / sizeof(char *);
  main_menu.select_cb = main_menu_handler;
  main_menu.exit_cb = main_menu_exit;

  general_submenu(main_menu);
}

////////////////   TRAKERS LIST MENU   /////////////////
static void list_selection_handler(uint8_t selection) {
  trakers_module_traker_details(selection);
}
void trakers_scenes_list_menu(char **trakers, uint8_t count) {
  general_submenu_menu_t list = {0};
  list.title = "Trakers List";
  list.options = trakers;
  list.options_count = count;
  list.select_cb = list_selection_handler;
  list.exit_cb = trakers_scenes_main_menu;

  general_submenu(list);
}

void trakers_scenes_no_devices_found() {
  general_notification_ctx_t no_found = {0};
  no_found.head = "Information";
  no_found.body = "Nothing to show";
  no_found.on_exit = trakers_scenes_main_menu;

  general_notification_handler(no_found);
}

////////////////   Searching Devices   /////////////////
void trakers_scenes_searching() {
  general_notification_ctx_t searching = {0};
  searching.head = "Searching";
  searching.body = "Devices";
  searching.on_exit = trakers_scenes_main_menu;

  general_notification_handler(searching);
}

////////////////   Device Found   /////////////////
void trakers_scenes_device_found(char *device_name) {
  general_notification_ctx_t found = {0};
  found.head = "Device found";
  found.body = device_name;
  found.on_exit = trakers_scenes_main_menu;

  general_notification_handler(found);
}

////////////////   TRAKER DETAILS MENU   /////////////////
void traker_scenes_traker_details(char **traker_details) {
  general_scrolling_text_ctx details = {0};
  details.banner = "Traker Details";
  details.text_arr = traker_details;
  details.text_len = 4;
  details.scroll_type = GENERAL_SCROLLING_TEXT_CLAMPED;
  details.window_type = GENERAL_SCROLLING_TEXT_WINDOW;
  details.exit_cb = trakers_module_traker_details_exit;

  general_scrolling_text_array(details);
}