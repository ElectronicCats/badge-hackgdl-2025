#include "tama_scenes.h"

#include "general_knob.h"
#include "general_radio_selection.h"
#include "general_submenu.h"
#include "menus_module.h"
#include "preferences.h"
#include "tama_app.h"
#include "tama_state.h"

#define SPEED 1
#define SPEED_MEM "speed"

void tama_scenes_main();
void tama_scenes_speed();

static uint8_t last_main_selection = 0;

////////////////////////////////////// MAIN MENU
/////////////////////////////////////////
typedef enum {
  MAIN_MENU_PLAY,
  MAIN_MENU_SPEED,
  MAIN_MENU_ERASE,
} main_menu_options_t;

static const char *main_menu_options[] = {
    "Play",
    "Speed",
    "Erase",
};

static void tama_scenes_handler(uint8_t option) {
  last_main_selection = option;
  switch (option) {
  case MAIN_MENU_PLAY:
    tama_app_begin();
    break;
  case MAIN_MENU_SPEED:
    tama_scenes_speed();
    break;
  case MAIN_MENU_ERASE:
    tama_state_erase();
    break;
  }
}

void tama_scenes_main() {
  general_submenu_menu_t main_menu = {0};
  main_menu.title = "Tamagotchi App";
  main_menu.options = main_menu_options;
  main_menu.options_count = sizeof(main_menu_options) / sizeof(char *);
  main_menu.selected_option = last_main_selection;
  main_menu.select_cb = tama_scenes_handler;
  main_menu.exit_cb = menus_module_exit_app;
  general_submenu(main_menu);
}

////////////////////////////////////// Speed Selection
/////////////////////////////////////////

static void speed_handler(uint8_t speed) {
  preferences_put_uchar(SPEED_MEM, speed);
  tama_app_set_speed(speed);
}

void tama_scenes_speed() {
  general_knob_ctx_t speed_menu = {0};
  speed_menu.help_lbl = "Emulation speed";
  speed_menu.max = 10;
  speed_menu.min = 1;
  speed_menu.step = 1;
  speed_menu.value = preferences_get_uchar(SPEED_MEM, SPEED);
  speed_menu.prefix = "x";
  speed_menu.var_lbl = "Speed";
  speed_menu.units_lbl = "";
  speed_menu.value_handler = speed_handler;
  speed_menu.exit_cb = tama_scenes_main;
  general_knob(speed_menu);
}