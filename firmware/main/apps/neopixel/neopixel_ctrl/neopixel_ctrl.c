#include "neopixel_ctrl.h"

#include "knob_rect.h"
#include "menus_module.h"
#include "oled_screen.h"

#define COLORS_COUNT 3

static neopixel_ctrl_t *npx_ctx = NULL;

static void npx_ctx_free() {
  if (npx_ctx != NULL) {
    for (uint8_t i = 0; i < COLORS_COUNT; i++) {
      if (npx_ctx->_rgb[i]) {
        knob_rect_free(npx_ctx->_rgb[i]);
      }
    }
    free(npx_ctx);
    npx_ctx = NULL;
  }
}

static void npx_color_alloc() {
  for (uint8_t i = 0; i < COLORS_COUNT; i++) {
    int16_t value = npx_ctx->buf[npx_ctx->npx_idx * COLORS_COUNT + i];
    knob_params_config_t params = knob_rect_build_params(0, 255, 1, value);
    knob_transform_config_t transform =
        knob_rect_build_transform(0, 8 * i, 128, 8);
    npx_ctx->_rgb[i] = knob_rect_alloc(&params, &transform);
  }
}

static void draw_cb() {
  oled_screen_clear_buffer();
  for (uint8_t i = 0; i < COLORS_COUNT; i++) {
    knob_rect_draw(npx_ctx->_rgb[i]);
  }
  oled_screen_display_show();
}

static void input_cb(uint8_t button, uint8_t event) {
  if (event != BUTTON_PRESS_DOWN && event != BUTTON_LONG_PRESS_HOLD) {
    return;
  }

  switch (button) {
  case BUTTON_LEFT:
    knob_rect_decrease_step(npx_ctx->_rgb[npx_ctx->_color_idx]);
    draw_cb();
    break;
  case BUTTON_RIGHT:
    knob_rect_increase_step(npx_ctx->_rgb[npx_ctx->_color_idx]);
    draw_cb();
    break;
  case BUTTON_MIDDLE:
    npx_ctx->_color_idx = (npx_ctx->_color_idx + 1) % COLORS_COUNT;
    break;
  case BUTTON_BACK:
    void (*exit_cb)(void) = npx_ctx->exit_cb;
    npx_ctx_free();
    if (exit_cb != NULL) {
      exit_cb();
    }
    break;
  default:
    break;
  }
}

void neopixel_ctrl(neopixel_ctrl_t ctx) {
  npx_ctx_free();
  npx_ctx = malloc(sizeof(neopixel_ctrl_t));
  npx_ctx->_rgb = calloc(COLORS_COUNT, sizeof(knob_rect_t *));

  npx_ctx->buf = ctx.buf;
  npx_ctx->npx_idx = ctx.npx_idx;
  npx_ctx->_color_idx = ctx._color_idx;
  npx_ctx->exit_cb = ctx.exit_cb;

  npx_color_alloc();

  menus_module_set_app_state(true, input_cb);
  draw_cb();
}