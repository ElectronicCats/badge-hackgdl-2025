#include "adv_scanner_scenes.h"

#include "string.h"

#include "adv_scan_module.h"
#include "general_notification.h"
#include "general_radio_selection.h"
#include "general_submenu.h"
#include "menus_module.h"
#include "preferences.h"

void adv_scanner_scenes_main_menu();
void adv_scanner_scenes_scan_type();
void adv_scanner_scenes_scan_filter();
void adv_scanner_scenes_scanning_card(char *head, char *body);

///////////////// MAIN MENU /////////////
static enum {
  MAIN_SCAN_TYPE_OPTION,
  MAIN_FILTER_OPTION,
  MAIN_START_OPTION
} main_menu_options_e;

static const char *main_menu_options[] = {"Scan Type", "ADV Filter", "Start"};

static void main_menu_handler(uint8_t selection) {
  switch (selection) {
  case MAIN_SCAN_TYPE_OPTION:
    adv_scanner_scenes_scan_type();
    break;
  case MAIN_FILTER_OPTION:
    adv_scanner_scenes_scan_filter();
    break;
  case MAIN_START_OPTION:
    adv_scanner_module_start();
    break;
  default:
    break;
  }
}

static void main_menu_exit_cb() { menus_module_restart(); }

void adv_scanner_scenes_main_menu() {
  general_submenu_menu_t main_menu;
  memset(&main_menu, 0, sizeof(main_menu));
  main_menu.title = "ADV Scanner";
  main_menu.options = main_menu_options;
  main_menu.options_count = sizeof(main_menu_options) / sizeof(char *);
  main_menu.select_cb = main_menu_handler;
  main_menu.exit_cb = main_menu_exit_cb;

  general_submenu(main_menu);
}

///////////////// SCAN TYPE MENU /////////////
static const char *scan_type_items[] = {"Active", "Passive"};

static void scan_type_handler(uint8_t selection) {
  adv_scanner_module_set_scan_type(selection);
}

void adv_scanner_scenes_scan_type() {
  general_radio_selection_menu_t scan_type;
  memset(&scan_type, 0, sizeof(general_radio_selection_menu_t));
  scan_type.banner = "Scan Type";
  scan_type.options = scan_type_items;
  scan_type.options_count = sizeof(scan_type_items) / sizeof(char *);
  scan_type.current_option = preferences_get_uchar(SCAN_TYPE_MEM, 0);
  scan_type.select_cb = scan_type_handler;
  scan_type.exit_cb = adv_scanner_scenes_main_menu;

  general_radio_selection(scan_type);
}

///////////////// SCAN FILTER MENU /////////////
static const char *scan_filter_items[] = {"Allow All", "Allow Only WLST",
                                          "Allow UND RPA", "Allow WLST & RPA"};

static void scan_filter_handler(uint8_t selection) {
  adv_scanner_module_set_filter(selection);
}

void adv_scanner_scenes_scan_filter() {

  general_radio_selection_menu_t scan_filter;
  memset(&scan_filter, 0, sizeof(general_radio_selection_menu_t));
  scan_filter.banner = "ADV Filter";
  scan_filter.options = scan_filter_items;
  scan_filter.options_count = sizeof(scan_filter_items) / sizeof(char *);
  scan_filter.current_option = preferences_get_uchar(SCAN_FILTER_MEM, 0);
  scan_filter.select_cb = scan_filter_handler;
  scan_filter.exit_cb = adv_scanner_scenes_main_menu;

  general_radio_selection(scan_filter);
}

///////////////// SCANNING CARD /////////////
void adv_scanner_scenes_scanning_card(char *head, char *body) {
  general_notification_ctx_t scanning_card;
  memset(&scanning_card, 0, sizeof(scanning_card));
  scanning_card.head = head;
  scanning_card.body = body;
  scanning_card.on_exit = adv_scanner_scenes_main_menu;
  general_notification(scanning_card);
}