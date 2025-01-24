#include "tama_anim.h"

#include "animation_t.h"
#include "animations_module.h"
#include "oled_screen.h"
#include "tama_bmps.h"

const uint16_t pinata_order[] = {9, 10, 11};
const uint32_t pinata_duration_ms[] = {200, 200, 200};

const animation_t pinata_animation = {
    .bitmaps = pinata_bitmaps,
    .bitmaps_len = sizeof(pinata_bitmaps) / sizeof(bitmap_t),
    .order = pinata_order,
    .duration_ms = pinata_duration_ms,
    .frames_len = MIN(sizeof(pinata_order) / sizeof(uint16_t),
                      sizeof(pinata_duration_ms) / sizeof(uint32_t))};

void tama_anim_start() {
  animations_module_ctx_t animation = {0};
  animation.animation = &pinata_animation;
  animation.loop = true;

  animations_module_run(animation);
}