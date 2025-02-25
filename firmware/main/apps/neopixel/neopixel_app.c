#include "neopixel_app.h"

#include "general_submenu.h"
#include "menus_module.h"

#include "neopixel_ctrl.h"
#include "neopixels_module.h"
#include "neopixels_t.h"

#define NEOPIXELS_COUNT 3

static uint8_t neopixels_buffer[3 * 8] = {0};
static uint8_t main_last_selection = 0;

void neopixel_app();

static void build_neopixel_ctr(uint8_t idx);
static void refresh_neopixels();

typedef enum { NEOPIXEL_OPTION, PRESETS_OPTION = 3 } neopixel_options_e;

static char *main_options[] = {"All", "Neopixel 1", "Neopixel 2", "Neopixel 3"};
static char *_main_options[] = {"Neopixel 1", "Neopixel 2", "Neopixel 3",
                                "All"};

static void neopixel_main_handler(uint8_t selection) {
  main_last_selection = selection;
  if (!selection) {
    selection = 4;
  }
  build_neopixel_ctr(selection - 1);
}

void neopixel_app() {
  refresh_neopixels();
  general_submenu_menu_t neopixel_main = {0};
  neopixel_main.title = "Neopixel Control";
  neopixel_main.options = main_options;
  neopixel_main.options_count = sizeof(main_options) / sizeof(char *);
  neopixel_main.selected_option = main_last_selection;
  neopixel_main.select_cb = neopixel_main_handler;
  neopixel_main.exit_cb = menus_module_exit_app;

  general_submenu(neopixel_main);
}

static void build_neopixel_ctr(uint8_t idx) {
  neopixel_ctrl_t npx_ctrl = {0};
  npx_ctrl.title = _main_options[idx];
  npx_ctrl.buf = neopixels_buffer;
  npx_ctrl.npx_idx = idx;
  npx_ctrl.exit_cb = neopixel_app;

  neopixel_ctrl(npx_ctrl);
}

static void refresh_neopixels() {
  for (uint8_t i = 0; i < NEOPIXELS_COUNT; i++) {
    neopixels_set_pixel(i, neopixels_buffer[i * 3], neopixels_buffer[i * 3 + 1],
                        neopixels_buffer[i * 3 + 2]);
  }
  neopixels_refresh();
}