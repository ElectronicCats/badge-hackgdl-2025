#include "web_file_browser_module.h"

#include "flash_fs.h"
#include "keyboard_module.h"
#include "menus_module.h"
#include "oled_screen.h"
#include "preferences.h"
// #include "sd_card.h"
#include "web_file_browser.h"
#include "web_file_browser_screens.h"
#include "wifi_app.h"

static void web_file_browser_input_cb(uint8_t button_name,
                                      uint8_t button_event);
static void web_file_browser_module_exit();

void web_file_browser_module_begin() {
  oled_screen_clear();
  menus_module_set_app_state(true, web_file_browser_input_cb);
  if (flash_fs_mount() == ESP_OK) {
    bool wifi_connected = preferences_get_bool("wifi_connected", false);
    if (!wifi_connected) {
      wifi_ap_init();
    }
    // wifi_ap_init();
    web_file_browser_set_show_event_cb(web_file_browse_show_event_handler);
    web_file_browser_begin();
  } else {
    oled_screen_display_text_center("Flash File", 3, OLED_DISPLAY_NORMAL);
    oled_screen_display_text_center("System Failed", 4, OLED_DISPLAY_NORMAL);
    vTaskDelay(pdMS_TO_TICKS(2000));
    web_file_browser_module_exit();
  }
}
void web_file_browser_module_exit() { menus_module_restart(); }
static void web_file_browser_input_cb(uint8_t button_name,
                                      uint8_t button_event) {
  if (button_event != BUTTON_PRESS_DOWN) {
    return;
  }
  switch (button_name) {
  case BUTTON_BACK:
    web_file_browser_module_exit();
    break;
  default:
    break;
  }
}