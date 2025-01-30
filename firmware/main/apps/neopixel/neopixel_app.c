#include "neopixel_app.h"

#include "general_submenu.h"
#include "menus_module.h"

#include "neopixels_t.h"

static uint8_t neopixels_buffer[3 * 8] = {0};

typedef enum { NEOPIXEL_OPTION, PRESETS_OPTION = 3 } neopixel_options_e;

static void neopixel_main_handler(uint8_t selection) {
  switch (selection) {
  case PRESETS_OPTION:
    break;
  default:

    break;
  }
}

void neopixel_app() {
  general_submenu_menu_t neopixel_main;
  neopixel_main.title = "Nepixel Control";
  neopixel_main.options =
      (char *[]){"Neopixel 1", "Neopixel 2", "Neopixel 3", "Presets"};
  neopixel_main.options_count = 4;
  neopixel_main.select_cb = neopixel_main.exit_cb = menus_module_exit_app;

  general_submenu(neopixel_main);
}