#include "deauth_scenes.h"

#include "deauth_module.h"
#include "general_radio_selection.h"
#include "general_submenu.h"
#include "menus_module.h"

void deauth_scenes_main_menu();
void deauth_scenes_ap_selection(char **ap_names, uint8_t ap_count,
                                uint8_t selected);
void deauth_scenes_attack_selection(uint16_t selected);
void deauth_scenes_portal_selection();

static uint8_t last_main_selection = 0;

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
  last_main_selection = selection;
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
  main_menu.selected_option = last_main_selection;
  main_menu.select_cb = main_menu_handler;
  main_menu.exit_cb = menus_module_restart;

  general_submenu(main_menu);
}

/////////////// AP SELECTION ///////////
void deauth_scenes_ap_selection(char **ap_names, uint8_t ap_count,
                                uint8_t selected) {
  general_radio_selection_menu_t ap = {0};
  ap.banner = "Select AP";
  ap.options = ap_names;
  ap.options_count = ap_count;
  ap.current_option = selected;
  ap.select_cb = deauth_module_set_ap;
  ap.exit_cb = deauth_scenes_main_menu;

  general_radio_selection(ap);
}

/////////////// ATTACK SELECTION ///////////
static const char *deauth_attacks[] = {"Broadcast", "Rogue AP", "Combined",
                                       "Captive Portal"};

void deauth_scenes_attack_selection(uint16_t selected) {
  general_radio_selection_menu_t attack = {0};
  attack.banner = "Select Attack";
  attack.options = deauth_attacks;
  attack.options_count = sizeof(deauth_attacks) / sizeof(char *);
  attack.current_option = selected;
  attack.select_cb = deauth_module_set_attack;
  attack.exit_cb = deauth_scenes_main_menu;

  general_radio_selection(attack);
}

/////////////// CAPTIVE PORTAL SELECTION ///////////
static const char *captive_portals[] = {"Google", "Wifi Pass"};

void deauth_scenes_portal_selection() {
  general_submenu_menu_t captive_portal = {0};
  captive_portal.title = "Captive Portal";
  captive_portal.options = captive_portals;
  captive_portal.options_count = sizeof(captive_portals) / sizeof(char *);
  captive_portal.select_cb = deauth_module_set_captive_portal_settings;
  captive_portal.exit_cb = deauth_scenes_main_menu;

  general_submenu(captive_portal);
}
