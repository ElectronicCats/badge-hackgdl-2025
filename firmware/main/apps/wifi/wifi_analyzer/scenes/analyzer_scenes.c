#include "analyzer_scenes.h"

#include "general_radio_selection.h"
#include "general_screens.h"
#include "general_scrolling_text.h"
#include "general_submenu.h"
#include "menus_module.h"
#include "wifi_analyzer.h"
#include "wifi_sniffer.h"

static analyzer_scenes_e current_scene;

void analyzer_scenes_main_menu();
void analyzer_scenes_channel();
void analyzer_scenes_summary(char **list, uint16_t list_size);

analyzer_scenes_e analyzer_get_current_scene() { return current_scene; }
static uint8_t last_main_selection = 0;

//////////////////////////   MAIN MENU   ///////////////////////////////
static enum {
  ANALYZER_START_OPTION,
  ANALYZER_SETTINGS_OPTION,
  ANALYZER_HELP_OPTION
} analyzer_main_options_e;

char *analizer_main_options[] = {"Start", "Settings", "Help"};

static void main_menu_selection_handler(uint8_t selection) {
  last_main_selection = selection;
  switch (selection) {
  case ANALYZER_START_OPTION:
    wifi_analyzer_run();
    break;
  case ANALYZER_SETTINGS_OPTION:
    analyzer_scenes_channel();
    break;
  case ANALYZER_HELP_OPTION:
    wifi_analyzer_help();
    break;
  default:
    break;
  }
}

static void main_menu_exit_handler() { menus_module_restart(); }

void analyzer_scenes_main_menu() {
  current_scene = ANALYZER_MAIN_SCENE;
  general_submenu_menu_t main_menu = {0};
  main_menu.title = "Wifi Analyzer";
  main_menu.options = analizer_main_options;
  main_menu.options_count = sizeof(analizer_main_options) / sizeof(char *);
  main_menu.selected_option = last_main_selection;
  main_menu.select_cb = main_menu_selection_handler;
  main_menu.exit_cb = main_menu_exit_handler;
  general_submenu(main_menu);
  wifi_analyzer_begin();
}

//////////////////////////   CHANNEL MENU   ///////////////////////////////
static const char *channel_options[] = {
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14",
};
static void set_channel(uint8_t selected_item) {
  wifi_sniffer_set_channel(selected_item + 1);
}
void analyzer_scenes_channel() {
  general_radio_selection_menu_t channel = {0};
  channel.banner = "Choose Channel",
  channel.current_option = wifi_sniffer_get_channel() - 1;
  channel.options = channel_options;
  channel.options_count = sizeof(channel_options) / sizeof(char *);
  channel.select_cb = set_channel;
  channel.exit_cb = analyzer_scenes_main_menu;
  channel.style = RADIO_SELECTION_OLD_STYLE;
  general_radio_selection(channel);
}

//////////////////////////   HELP MENU   ///////////////////////////////
static const char *wifi_analizer_help = {
    "This tool     "
    "allows you to "
    "analyze the   "
    "WiFi networks "
    "around you.   "
    "You can select"
    "the channel in"
    "settings menu.",
};

void wifi_analyzer_help() {
  general_scrolling_text_ctx help = {0};
  memset(&help, 0, sizeof(help));
  help.banner = "Analyzer Help";
  help.scroll_type = GENERAL_SCROLLING_TEXT_CLAMPED;
  help.window_type = GENERAL_SCROLLING_TEXT_WINDOW;
  help.text = wifi_analizer_help;
  help.exit_cb = analyzer_scenes_main_menu;
  general_scrolling_text(help);
}

//////////////////////////   SUMMARY MENU   ///////////////////////////////
void analyzer_scenes_summary(char **list, uint16_t list_size) {
  general_scrolling_text_ctx summary = {0};
  memset(&summary, 0, sizeof(summary));
  summary.banner = "Information";
  summary.scroll_type = GENERAL_SCROLLING_TEXT_INFINITE;
  summary.window_type = GENERAL_SCROLLING_TEXT_WINDOW;
  summary.text_arr = list;
  summary.text_len = list_size;
  summary.exit_cb = wifi_module_summary_exit_cb;
  general_scrolling_text_array(summary);
}