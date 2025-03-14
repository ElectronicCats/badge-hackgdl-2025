#include "tama_scenes.h"

#include "general_knob.h"
#include "general_radio_selection.h"
#include "general_scrolling_text.h"
#include "general_submenu.h"
#include "menus_module.h"
#include "preferences.h"

#include "tama_anim.h"
#include "tama_app.h"
#include "tama_display.h"
#include "tama_scan.h"
#include "tama_state.h"

#define SPEED 1
#define SPEED_MEM "speed"

void tama_scenes_main();
void tama_scenes_speed();
void tama_scenes_friends();
void tama_scenes_friends_list(tama_friends_ctx_t *ctx);
void tama_scenes_help();

static void tama_scenes_erase();
static void show_help(const char *banner, char **help, uint16_t len);

static uint8_t last_main_selection = 0;
static uint8_t last_help_selection = 0;
static uint8_t last_friends_selection = 0;

///////////////////////////// MAIN MENU /////////////////////////////
typedef enum {
  MAIN_MENU_PLAY,
  MAIN_MENU_SPEED,
  MAIN_MENU_FRIENDS,
  MAIN_MENU_ERASE,
  MAIN_MENU_HELP,
} main_menu_options_t;

static const char *main_menu_options[] = {
    "Play", "Speed", "Friends", "Erase", "Help",
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
  case MAIN_MENU_FRIENDS:
    tama_scenes_friends();
    break;
  case MAIN_MENU_ERASE:
    tama_scenes_erase();
    break;
  case MAIN_MENU_HELP:
    tama_scenes_help();
    break;
  }
}

void tama_scenes_main() {
  tama_friends_begin();

  general_submenu_menu_t main_menu = {0};
  main_menu.title = "Tamagotchi App";
  main_menu.options = main_menu_options;
  main_menu.options_count = sizeof(main_menu_options) / sizeof(char *);
  main_menu.selected_option = last_main_selection;
  main_menu.select_cb = tama_scenes_handler;
  main_menu.exit_cb = menus_module_restart;
  general_submenu(main_menu);
}

//////////////////////// Speed Selection ////////////////////////

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

///////////////////////////// ERASE MENU /////////////////////////////
typedef enum {
  ERASE_MENU_CANCEL,
  ERASE_MENU_ERASE,
} erase_menu_options_t;

static const char *erase_menu_options[] = {
    "Cancel",
    "Erase",
};

static void erase_handler(uint8_t option) {
  switch (option) {
  case ERASE_MENU_ERASE:
    tama_state_erase();
  case ERASE_MENU_CANCEL:
    tama_scenes_main();
    break;
  }
}

static void tama_scenes_erase() {
  general_submenu_menu_t erase_menu = {0};
  erase_menu.title = "Are you shure?";
  erase_menu.options = erase_menu_options;
  erase_menu.options_count = sizeof(erase_menu_options) / sizeof(char *);
  erase_menu.selected_option = 0;
  erase_menu.select_cb = erase_handler;
  erase_menu.exit_cb = tama_scenes_main;
  general_submenu(erase_menu);
}

///////////////////////////// FRIENDS MENU /////////////////////////////
typedef enum {
  FRIENDS_SCAN,
  FRIENDS_LIST,
} friends_menu_options_t;

static const char *friends_menu_options[] = {
    "Scan",
    "List",
};

static void friends_menu_handler(uint8_t option) {
  last_friends_selection = option;
  switch (option) {
  case FRIENDS_SCAN:
    tama_display_load_nickname();
    tama_scan_begin();
    break;
  case FRIENDS_LIST:
    tama_friends_show_list();
    break;
  }
}

void tama_scenes_friends() {
  general_submenu_menu_t friends_menu = {0};
  friends_menu.title = "Tamagotchi App";
  friends_menu.options = friends_menu_options;
  friends_menu.options_count = sizeof(friends_menu_options) / sizeof(char *);
  friends_menu.selected_option = last_friends_selection;
  friends_menu.select_cb = friends_menu_handler;
  friends_menu.exit_cb = tama_scenes_main;
  general_submenu(friends_menu);
}

//////////////////////// Friends List ////////////////////////

char **friends_list;

static void friends_list_exit() {
  free(friends_list);
  tama_scenes_friends();
}

void tama_scenes_friends_list(tama_friends_ctx_t *ctx) {
  general_scrolling_text_ctx list = {0};

  static char list_banner[20];
  uint16_t friends_count = tama_friends_get_count();
  snprintf(list_banner, sizeof(list_banner), "Friends: %03d", friends_count);

  friends_list = calloc(friends_count, sizeof(char *));
  for (uint16_t i = 0; i < friends_count; i++) {
    friends_list[i] = ctx->friends_str[i];
  }

  list.banner = list_banner;
  list.text_arr = friends_list;
  list.text_len = friends_count;
  list.scroll_type = GENERAL_SCROLLING_TEXT_CLAMPED;
  list.window_type = GENERAL_SCROLLING_TEXT_WINDOW;
  list.exit_cb = friends_list_exit;

  general_scrolling_text_array(list);
}

///////////////////////////// HELP MENU /////////////////////////////
typedef enum {
  HELP_MENU_TAMAGOTCHI,
  HELP_MENU_NICKNAME,
  HELP_MENU_FRIENDS,
} help_menu_options_t;

static const char *help_menu_options[] = {
    "Tamagotchi",
    "Nickname",
    "Friends",
};

static const char *tamagotchi_help[] = {
    "Its a classic", "Tamagotchi P1", "emulator.",    "Thanks to:",
    "-jcrona",       "-Otrebor671",   "To activate",  "your pet,",
    "configure the", "clock by",      "pressing the", "middle button"};
static const char *nickname_help[] = {
    "Connect to your", "computer, open", "the serial port", "and change your",
    "pet's nickname",  "by using the",   "next command:",   "`tama_nickname'"};
static const char *friends_help[] = {
    "Open the scan", "menu at:",    "FRIENDS->SCAN,",
    "Get close to",  "other pets",  "to increase",
    "your friends",  "list & your", "friends count"};

static void help_menu_handler(uint8_t option) {
  last_help_selection = option;
  switch (option) {
  case HELP_MENU_TAMAGOTCHI:
    show_help("Tamagotchi Help", tamagotchi_help,
              sizeof(tamagotchi_help) / sizeof(char *));
    break;
  case HELP_MENU_NICKNAME:
    show_help("NickName Help", nickname_help,
              sizeof(nickname_help) / sizeof(char *));
    break;
  case HELP_MENU_FRIENDS:
    show_help("Friends Help", friends_help,
              sizeof(friends_help) / sizeof(char *));
    break;
  }
}

void tama_scenes_help() {
  general_submenu_menu_t help_menu = {0};
  help_menu.title = "Tamagotchi Help";
  help_menu.options = help_menu_options;
  help_menu.options_count = sizeof(help_menu_options) / sizeof(char *);
  help_menu.selected_option = last_help_selection;
  help_menu.select_cb = help_menu_handler;
  help_menu.exit_cb = tama_scenes_main;
  general_submenu(help_menu);
}

///////////////////////////// HELP ITEM /////////////////////////////

static void show_help(const char *banner, char **help, uint16_t len) {
  general_scrolling_text_ctx list = {0};

  list.banner = banner;
  list.text_arr = help;
  list.text_len = len;
  list.scroll_type = GENERAL_SCROLLING_TEXT_CLAMPED;
  list.window_type = GENERAL_SCROLLING_TEXT_WINDOW;
  list.exit_cb = tama_scenes_help;

  general_scrolling_text_array(list);
}