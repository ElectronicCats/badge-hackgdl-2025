#include "hid_scenes.h"

#include "string.h"

#include "general_notification.h"
#include "general_submenu.h"
#include "hid_module.h"
#include "menus_module.h"'
#include "oled_screen.h"

void hid_scenes_main_menu();
void hid_scenes_control_menu(uint8_t selected);
void hid_scenes_notification(char *head, char *body, uint32_t duration_ms);
void hid_scenes_notification_handler(char *head, char *body);

static uint8_t last_main_selection = 0;

/////////////// MAIN MENU //////////////
typedef enum { NAME_OPTION, MAC_OPTION, START_OPTION } main_menu_options_e;

static const char *main_menu_options[] = {"Device name", "Device MAC", "Start"};

static void main_menu_cb(uint8_t selection) {
  last_main_selection = selection;
  switch (selection) {
  case NAME_OPTION:
    hid_module_name();
    break;
  case MAC_OPTION:
    hid_module_mac();
    break;
  case START_OPTION:
    hid_module_start();
    break;
  default:
    break;
  }
}

static void hid_exit_cb() { menus_module_restart(); }

void hid_scenes_main_menu() {
  general_submenu_menu_t main_menu = {0};
  memset(&main_menu, 0, sizeof(main_menu));
  main_menu.title = "HID device";
  main_menu.options = main_menu_options;
  main_menu.options_count = sizeof(main_menu_options) / sizeof(char *);
  main_menu.selected_option = last_main_selection;
  main_menu.select_cb = main_menu_cb;
  main_menu.exit_cb = hid_exit_cb;
  general_submenu(main_menu);
}

/////////////// CONTROL MENU //////////////

typedef enum {
  VOL_UP_OPTION,
  VOL_DOWN_OPTION,
  PLAY_PAUSE_OPTION
} control_options_e;

static const char *control_options[] = {"Volumen Up", "Volumen Down",
                                        "Play/Pause"};

static void control_cb(uint8_t selection) {
  switch (selection) {
  case VOL_UP_OPTION:
    hid_module_control_vol_up();
    break;
  case VOL_DOWN_OPTION:
    hid_module_control_vol_down();
    break;
  case PLAY_PAUSE_OPTION:
    hid_module_control_play_pause();
    break;
  default:
    break;
  }
}

static void control_exit_cb() { hid_scenes_main_menu(); }

void hid_scenes_control_menu(uint8_t selected) {
  general_submenu_menu_t control_menu = {0};
  memset(&control_menu, 0, sizeof(control_menu));
  control_menu.title = "Media Control";
  control_menu.options = control_options;
  control_menu.options_count = sizeof(control_options) / sizeof(char *);
  control_menu.select_cb = control_cb;
  control_menu.exit_cb = control_exit_cb;
  control_menu.selected_option = selected;
  general_submenu(control_menu);
}

/////////////// NOTIFICATION MENU //////////////
void hid_scenes_notification(char *head, char *body, uint32_t duration_ms) {
  general_notification_ctx_t notification;
  memset(&notification, 0, sizeof(notification));
  notification.head = head;
  notification.body = body;
  notification.duration_ms = duration_ms;

  oled_screen_get_last_buffer();
  general_notification(notification);
  oled_screen_set_last_buffer();
  oled_screen_display_show();
}

/////////////// NOTIFICATION HANDLER MENU //////////////
static void on_exit_cb() { hid_scenes_main_menu(); }

void hid_scenes_notification_handler(char *head, char *body) {
  general_notification_ctx_t notification = {0};
  memset(&notification, 0, sizeof(notification));
  notification.head = head;
  notification.body = body;
  notification.on_exit = on_exit_cb;
  general_notification_handler(notification);
}