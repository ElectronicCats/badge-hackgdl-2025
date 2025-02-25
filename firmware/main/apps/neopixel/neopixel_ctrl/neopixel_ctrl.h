#ifndef _NEOPIXEL_CTRL_H_
#define _NEOPIXEL_CTRL_H_

#include <stdio.h>

#include "knob_rect.h"
#include "neopixels_t.h"

typedef struct {
  const char *title;
  uint8_t npx_idx;
  uint8_t _color_idx;
  uint8_t *buf;
  knob_rect_t **_rgb;

  void (*exit_cb)(void);
} neopixel_ctrl_t;

void neopixel_ctrl(neopixel_ctrl_t ctx);

#endif // _NEOPIXEL_CTRL_H_