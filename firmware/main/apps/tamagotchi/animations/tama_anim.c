#include "tama_anim.h"

#include "animation_t.h"
#include "animations_module.h"
#include "oled_screen.h"

#include "bitmaps.h"
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

static void pos_splash_screen_draw() {
  for (uint8_t i = 0; i < 8; i++) {
    oled_screen_buffer_bitmap(bitmaps + i * 18, i / 4 ? 109 : -9,
                              i % 4 * 16 + 6, 16, 9, OLED_DISPLAY_NORMAL);
  }
  oled_screen_draw_rect(12, 8, 103, 55, OLED_DISPLAY_NORMAL);
  oled_screen_draw_box(12, 0, 103, 8, OLED_DISPLAY_NORMAL);
  oled_screen_draw_rect(15, 55, 97, 5, OLED_DISPLAY_NORMAL);
  oled_screen_display_text_center("Loading... x", 0, OLED_DISPLAY_INVERT);
  oled_screen_draw_vline(100, 1, 7, OLED_DISPLAY_INVERT);
}

static void time_bar_task() {
  for (uint8_t i = 0; i < 100; i++) {
    oled_screen_draw_loading_bar(i + 1, 15, 55, 97, 5);
    oled_screen_display_show();
  }

  // TickType_t start_time = xTaskGetTickCount();
  // oled_screen_draw_loading_bar(100, 15, 55, 97, 5);
  // oled_screen_display_show();
  // TickType_t end_time = xTaskGetTickCount();
  // uint32_t elapsed_time = (end_time - start_time) * portTICK_PERIOD_MS;
  // printf("Tiempo de ejecución: %lu ms\n", elapsed_time);

  vTaskDelete(NULL);
}

void tama_anim_splah_screen() {
  animations_module_ctx_t animation = {0};
  animation.animation = &pinata_animation;
  animation.loop = true;
  animation.pos_draw_cb = pos_splash_screen_draw;

  animations_module_run(animation);
  xTaskCreate(time_bar_task, "time_bar_task", 2048, NULL, 3, NULL);
  vTaskDelay(pdMS_TO_TICKS(3100));
  animations_module_stop();
}