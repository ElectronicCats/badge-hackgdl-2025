#include "general_knob.h"

#include "menus_module.h"
#include "oled_screen.h"
#include <string.h>

static general_knob_ctx_t *knob_ctx = NULL;

static void knob_exit() {
  if (knob_ctx->exit_cb) {
    knob_ctx->exit_cb();
    free(knob_ctx);
  }
}

static void knob_draw_cb() {
  oled_screen_clear_buffer();

  oled_screen_draw_box(0, 0, 128, 8, OLED_DISPLAY_NORMAL);
  oled_screen_display_text_center(knob_ctx->help_lbl, 0, OLED_DISPLAY_INVERT);

  char str[20];
  sprintf(str, "Min:%d|Max:%d", knob_ctx->min, knob_ctx->max);
  oled_screen_display_text_center(str, 2, OLED_DISPLAY_NORMAL);

  sprintf(str, "%s: <%s%d%s>", knob_ctx->var_lbl, knob_ctx->prefix,
          knob_ctx->value, knob_ctx->units_lbl);
  oled_screen_display_text_center(str, 4, OLED_DISPLAY_INVERT);

  oled_screen_display_show();
}

static void knob_input_cb(uint8_t button_name, uint8_t button_event) {
  if (button_event != BUTTON_PRESS_DOWN) {
    return;
  }

  switch (button_name) {
  case BUTTON_MIDDLE:
  case BUTTON_BACK:
    knob_exit();
    break;
  case BUTTON_RIGHT:
    knob_ctx->value += knob_ctx->step;
    if (knob_ctx->value > knob_ctx->max) {
      knob_ctx->value = knob_ctx->min;
    }
    knob_draw_cb();
    if (knob_ctx->value_handler) {
      knob_ctx->value_handler(knob_ctx->value);
    }
    break;
  case BUTTON_LEFT:
    knob_ctx->value -= knob_ctx->step;
    if (knob_ctx->value < knob_ctx->min) {
      knob_ctx->value = knob_ctx->max;
    }
    knob_draw_cb();
    if (knob_ctx->value_handler) {
      knob_ctx->value_handler(knob_ctx->value);
    }
    break;
  default:
    break;
  }
}

void general_knob(general_knob_ctx_t ctx) {
  knob_ctx = calloc(1, sizeof(general_knob_ctx_t));

  knob_ctx->min = ctx.min;
  knob_ctx->max = ctx.max;
  knob_ctx->step = ctx.step;
  knob_ctx->prefix = ctx.prefix;
  knob_ctx->value = ctx.value;
  knob_ctx->var_lbl = ctx.var_lbl;
  knob_ctx->units_lbl = ctx.units_lbl;
  knob_ctx->help_lbl = ctx.help_lbl;
  knob_ctx->value_handler = ctx.value_handler;
  knob_ctx->exit_cb = ctx.exit_cb;

  menus_module_set_app_state(true, knob_input_cb);
  knob_draw_cb();
}