#include "general_submenu.h"

#include "bitmaps_general.h"
#include "menus_module.h"
#include "oled_screen.h"

#ifdef CONFIG_RESOLUTION_128X64
#define MAX_OPTIONS_NUM 8
#else // CONFIG_RESOLUTION_128X32
#define MAX_OPTIONS_NUM 4
#endif

static general_submenu_menu_t *general_radio_selection_ctx;

static void list_submenu_options() {
  general_submenu_menu_t *ctx = general_radio_selection_ctx;
  oled_screen_clear_buffer();
  oled_screen_draw_box(0, 0, 128, 8, OLED_DISPLAY_NORMAL);
  oled_screen_display_text_center(ctx->title, 0, OLED_DISPLAY_INVERT);
  char str[16];
  snprintf(str, sizeof(str), "<%d/%d>", ctx->selected_option + 1,
           ctx->options_count);
  oled_screen_display_text_center(str, 6, OLED_DISPLAY_NORMAL);
  oled_screen_display_text_center(ctx->options[ctx->selected_option], 7,
                                  OLED_DISPLAY_NORMAL);
  oled_screen_display_show();
}

static void input_cb(uint8_t button_name, uint8_t button_event) {
  if (button_event != BUTTON_PRESS_DOWN) {
    return;
  }
  switch (button_name) {
  case BUTTON_BACK:
    void (*exit_cb)() = general_radio_selection_ctx->exit_cb;
    free(general_radio_selection_ctx);
    if (exit_cb) {
      exit_cb();
    }
    break;
  case BUTTON_MIDDLE:
    void (*select_cb)() = general_radio_selection_ctx->select_cb;
    if (select_cb) {
      select_cb(general_radio_selection_ctx->selected_option);
    }
    break;
  case BUTTON_LEFT:
    general_radio_selection_ctx->selected_option =
        general_radio_selection_ctx->selected_option == 0
            ? general_radio_selection_ctx->options_count - 1
            : general_radio_selection_ctx->selected_option - 1;
    list_submenu_options();
    break;
  case BUTTON_RIGHT:
    general_radio_selection_ctx->selected_option =
        ++general_radio_selection_ctx->selected_option <
                general_radio_selection_ctx->options_count
            ? general_radio_selection_ctx->selected_option
            : 0;
    list_submenu_options();
    break;
  default:
    break;
  }
}

void general_submenu(general_submenu_menu_t radio_selection_menu) {
  general_radio_selection_ctx = calloc(1, sizeof(general_submenu_menu_t));
  general_radio_selection_ctx->title = radio_selection_menu.title;
  general_radio_selection_ctx->options = radio_selection_menu.options;
  general_radio_selection_ctx->options_count =
      radio_selection_menu.options_count;
  general_radio_selection_ctx->select_cb = radio_selection_menu.select_cb;
  general_radio_selection_ctx->exit_cb = radio_selection_menu.exit_cb;
  menus_module_set_app_state(true, input_cb);
  list_submenu_options();
}