#include "deauth_scenes.h"

#include "deauth_module.h"
#include "general_radio_selection.h"
#include "general_submenu.h"
#include "menus_module.h"

void deauth_scenes_main_menu();
void deauth_scenes_ap_selection();
void deauth_scenes_attack_selection();

/////////////// MAIN MENU ///////////
typedef enum {
  SCAN_OPTION,
  AP_OPTION,
  ATTACK_OPTION,
  RUN_OPTION
} main_menu_options_e;

static const char *main_menu_options[] = {
    "SCAN",
    "SELECT AP",
    "ATTACK",
    "RUN",
};

static void main_menu_handler(uint8_t selection) {
  switch (selection) {
  case SCAN_OPTION:
    deauth_module_scan();
    break;
  case AP_OPTION:
    deauth_module_select_ap();
    break;
  case ATTACK_OPTION:
    deauth_module_select_attack();
    break;
  case RUN_OPTION:
    deauth_module_run();
    break;
  default:
    break;
  }
}

void deauth_scenes_main_menu() {
  general_submenu_menu_t main_menu = {0};
  main_menu.title = "DEAUTH";
  main_menu.options = main_menu_options;
  main_menu.options_count = sizeof(main_menu_options) / sizeof(char *);
  main_menu.select_cb = main_menu_handler;
  main_menu.exit_cb = menus_module_restart;

  general_submenu(main_menu);
}

/////////////// AP SELECTION ///////////
void deauth_scenes_ap_selection() {
  general_radio_selection_menu_t ap = {0};
  ap.banner = "Select AP";
  ap.options = NULL;
  ap.options_count = NULL;
  ap.current_option = 0;
  ap.select_cb = NULL;

  general_radio_selection(ap);
}

/////////////// ATTACK SELECTION ///////////
void deauth_scenes_attack_selection() {
  general_radio_selection_menu_t attack = {0};
  attack.banner = "Select Attack";
  attack.options = NULL;
  attack.options_count = NULL;
  attack.current_option = 0;
  attack.select_cb = NULL;

  general_radio_selection(attack);
}
