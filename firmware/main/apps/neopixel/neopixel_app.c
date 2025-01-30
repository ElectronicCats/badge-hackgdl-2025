#include "neopixel_app.h"

#include "general_submenu.h"
#include "menus_module.h"

#include "neopixel_ctrl.h"
#include "neopixels_t.h"

static uint8_t neopixels_buffer[3 * 8] = {0};
static uint8_t main_last_selection = 0;

void neopixel_app();

static void build_neopixel_ctr(uint8_t idx) {
  neopixel_ctrl_t npx_ctrl = {0};
  npx_ctrl.buf = neopixels_buffer;
  npx_ctrl.npx_idx = idx;
  npx_ctrl.exit_cb = neopixel_app;

  neopixel_ctrl(npx_ctrl);
}

typedef enum { NEOPIXEL_OPTION, PRESETS_OPTION = 3 } neopixel_options_e;

static char *main_options[] = {"Neopixel 1", "Neopixel 2", "Neopixel 3",
                               "Presets"};

static void neopixel_main_handler(uint8_t selection) {
  main_last_selection = selection;
  switch (selection) {
  case PRESETS_OPTION:
    break;
  default:
    build_neopixel_ctr(selection);
    break;
  }
}

void neopixel_app() {
  general_submenu_menu_t neopixel_main = {0};
  neopixel_main.title = "Neopixel Control";
  neopixel_main.options = main_options;
  neopixel_main.options_count = sizeof(main_options) / sizeof(char *);
  neopixel_main.selected_option = main_last_selection;
  neopixel_main.select_cb = neopixel_main_handler;
  neopixel_main.exit_cb = menus_module_exit_app;

  general_submenu(neopixel_main);
}

uint8_t *neopixel_app_get_buffer() { return neopixels_buffer; }