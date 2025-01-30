#ifndef _NEOPIXEL_CTRL_H_
#define _NEOPIXEL_CTRL_H_

#include <stdio.h>

#include "knob_rect.h"
#include "neopixels_t.h"

typedef struct {
  neopixel_t neopixel;
  knob_rect_t *r;
  knob_rect_t *g;
  knob_rect_t *b;
  uint8_t current_idx;
} neopixel_ctrl_t;

#endif // _NEOPIXEL_CTRL_H_