#ifndef _KNOB_RECT_H_
#define _KNOB_RECT_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  uint16_t min;
  uint16_t max;
  uint16_t step;
  int16_t value;
} knob_params_config_t;

typedef struct {
  uint8_t x;
  uint8_t y;
  uint8_t width;
  uint8_t height;
} knob_transform_config_t;

typedef struct {
  knob_params_config_t params;
  knob_transform_config_t transform;
  bool focused;
} knob_rect_t;

knob_rect_t *knob_rect_alloc();
void knob_rect_free(knob_rect_t *knob);

knob_params_config_t knob_rect_build_params(uint16_t min, uint16_t max,
                                            uint16_t step, int16_t value);
knob_transform_config_t
knob_rect_build_transform(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

int16_t knob_rect_increase_step(knob_rect_t *knob);
int16_t knob_rect_decrease_step(knob_rect_t *knob);

void knob_rect_draw(knob_rect_t *knob);
void knob_rect_set_focus(knob_rect_t *knob, bool focused);

#endif // _KNOB_RECT_H_