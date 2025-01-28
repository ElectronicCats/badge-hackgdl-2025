#include "about.h"

#include "menus_module.h"

#include "general_notification.h"
#include "general_scrolling_text.h"
#include "general_submenu.h"

void about_main();

static void show_notification(const char *head, const char *body);
static void show_scrolling_text(const char *banner, char **text_arr,
                                uint16_t len);

static uint8_t last_main_selection = 0;

static char *credits_text[] = {
    "Developed by",   "Electronic Cats", "",
    "With love from", "Mexico...",       "",
    "Thanks",         "-Roberto:",       "  Otrebor671",
    "Thanks",         "-Carlos Adonai:", "  Donuts Diaz",
    "Thanks",         "-JeanCristophe:", "  jcrona",
    "and Electronic", "Cats team",
};

static char *legal_text[] = {
    "The user",        "assumes all",     "responsibility",   "for the use of",
    "this badge and",  "agrees to use",   "it legally and",   "ethically,",
    "avoiding any",    "activities that", "may cause harm,",  "interference,",
    "or unauthorized", "access to",       "systems or data.",
};

static enum {
  ABOUT_VERSION,
  ABOUT_LICENSE,
  ABOUT_CREDITS,
  ABOUT_LEGAL,
} about_options_e;

static const char *about_options[] = {"Version", "License", "Credits", "Legal"};

static void about_main_handler(uint8_t selection) {
  last_main_selection = selection;
  switch (selection) {
  case ABOUT_VERSION:
    show_notification("Version", "v1.1.0.0");
    break;
  case ABOUT_LICENSE:
    show_notification("License", "GNU GPL 3.0");
    break;
  case ABOUT_CREDITS:
    show_scrolling_text("Creditos", credits_text,
                        sizeof(credits_text) / sizeof(char *));
    break;
  case ABOUT_LEGAL:
    show_scrolling_text("Legal", legal_text,
                        sizeof(legal_text) / sizeof(char *));
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

static void show_notification(const char *head, const char *body) {
  general_notification_ctx_t notification = {0};
  notification.head = head;
  notification.body = body;
  notification.on_exit = about_main;

  general_notification_handler(notification);
}

static void show_scrolling_text(const char *banner, char **text_arr,
                                uint16_t len) {
  general_scrolling_text_ctx scrolling_info = {0};
  scrolling_info.banner = banner;
  scrolling_info.text_arr = text_arr;
  scrolling_info.text_len = len;
  scrolling_info.window_type = GENERAL_SCROLLING_TEXT_WINDOW;
  scrolling_info.scroll_type = GENERAL_SCROLLING_TEXT_CLAMPED;
  scrolling_info.exit_cb = about_main;

  general_scrolling_text_array(scrolling_info);
}
