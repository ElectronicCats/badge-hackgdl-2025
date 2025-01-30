#include "knob_rect.h"

#include "oled_screen.h"

knob_rect_t *knob_rect_alloc(knob_params_config_t *params,
                             knob_transform_config_t *transform) {
  knob_rect_t *knob = calloc(1, sizeof(knob_rect_t));

  knob->params.min = params->min;
  knob->params.max = params->max;
  knob->params.step = params->step;
  knob->params.value = params->value;

  knob->transform.x = transform->x;
  knob->transform.y = transform->y;
  knob->transform.width = transform->width;
  knob->transform.height = transform->height;

  return knob;
}

knob_params_config_t knob_rect_build_params(uint16_t min, uint16_t max,
                                            uint16_t step, int16_t value) {
  knob_params_config_t params = {0};

  params.min = min;
  params.max = max;
  params.step = step;
  params.value = value;

  return params;
}

knob_transform_config_t
knob_rect_build_transform(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  knob_transform_config_t transform = {0};

  transform.x = x;
  transform.y = y;
  transform.width = width;
  transform.height = height;

  return transform;
}

int16_t knob_rect_increase_step(knob_rect_t *knob) {
  knob->params.value += knob->params.step;
  if (knob->params.value > knob->params.max) {
    knob->params.value = knob->params.min;
  }
  return knob->params.value;
}

int16_t knob_rect_decrease_step(knob_rect_t *knob) {
  knob->params.value -= knob->params.step;
  if (knob->params.value < knob->params.min) {
    knob->params.value = knob->params.max;
  }
  return knob->params.value;
}

void knob_rect_free(knob_rect_t *knob) { free(knob); }

void knob_rect_draw(knob_rect_t *knob) {
  uint8_t fill = (uint8_t)(((float)(knob->params.value - knob->params.min) /
                            (knob->params.max - knob->params.min)) *
                           100);

  oled_screen_draw_loading_bar(fill, knob->transform.x, knob->transform.y,
                               knob->transform.width, knob->transform.height,
                               knob->focused);
}

void knob_rect_set_focus(knob_rect_t *knob, bool focused) {
  knob->focused = focused;
  knob_rect_draw(knob);
}