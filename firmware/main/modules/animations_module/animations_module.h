#ifndef _ANIMATIONS_MODULE_H_
#define _ANIMATIONS_MODULE_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#include "animation_t.h"

typedef struct {
  animation_t *animation;
  uint8_t x;
  uint8_t y;
  uint16_t current_frame;
  TaskHandle_t task_handle;
  void (*pre_draw_cb)();
  void (*pos_draw_cb)();
  void (*exit_cb)();
  bool invert;
  volatile bool loop;
  volatile bool _is_runing;
  volatile bool _is_paused;
} animations_module_ctx_t;

#endif // ANIMATIONS_MODULE_H_