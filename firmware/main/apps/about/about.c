#include "about.h"

#include "menus_module.h"

#include "general_scrolling_text.h"
#include "general_submenu.h"

void about_main();

static uint8_t last_main_selection = 0;

static enum {
  ABOUT_VERSION,
  ABOUT_LICENSE,
  ABOUT_CREDITS,
} about_options_e;

static const char *about_options[] = {
    "Version",
    "License",
    "Credits",
};

static void about_main_handler(uint8_t selection) {
  last_main_selection = selection;
  switch (selection) {
  case ABOUT_VERSION:
    break;
  case ABOUT_LICENSE:
    break;
  case ABOUT_CREDITS:
    break;
  default:
    break;
  }
}

void about_main() {
  general_submenu_menu_t about_menu = {0};
  about_menu.title = "About";
  about_menu.options = about_options;
  about_menu.options_count = sizeof(about_options) / sizeof(char *);
  about_menu.selected_option = last_main_selection;
  about_menu.exit_cb = menus_module_exit_app;
  about_menu.select_cb = about_main_handler;

  general_submenu(about_menu);
}