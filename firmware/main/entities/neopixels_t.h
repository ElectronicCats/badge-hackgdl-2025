#ifndef _NEOPIXELS_T_H_
#define _NEOPIXELS_T_H_

#include <stdio.h>

#include "rgb_color_t.h"

typedef struct {
  uint8_t pin;
  uint8_t count;
  uint8_t *pixel_buf;
} neopixels_t;

typedef struct {
  uint8_t idx;
  rgb_color_t color;
} neopixel_t;

#endif // _NEOPIXELS_T_H_
