#include "tama_anim.h"

#include "animation_t.h"
#include "animations_module.h"
#include "oled_screen.h"

#include "bitmaps.h"
#include "tama_bmps.h"

static void draw_tama_ui(const char *str) {
  for (uint8_t i = 0; i < 8; i++) {
    oled_screen_buffer_bitmap(bitmaps + i * 18, i / 4 ? 109 : -9,
                              i % 4 * 16 + 6, 16, 9, OLED_DISPLAY_NORMAL);
  }
  oled_screen_draw_rect(12, 8, 103, 55, OLED_DISPLAY_NORMAL);
  oled_screen_draw_box(12, 0, 103, 8, OLED_DISPLAY_NORMAL);
  oled_screen_display_text_center(str, 0, OLED_DISPLAY_INVERT);
  oled_screen_draw_vline(100, 1, 7, OLED_DISPLAY_INVERT);
}

const uint16_t pinata_splash_order[] = {9, 10, 11};
const uint32_t pinata_splash_durations_ms[] = {200, 200, 200};

const animation_t pinata_splash_animation = {
    .bitmaps = pinata_bitmaps,
    .bitmaps_len = sizeof(pinata_bitmaps) / sizeof(bitmap_t),
    .order = pinata_splash_order,
    .duration_ms = pinata_splash_durations_ms,
    .frames_len = MIN(sizeof(pinata_splash_order) / sizeof(uint16_t),
                      sizeof(pinata_splash_durations_ms) / sizeof(uint32_t))};

static void pos_splash_screen_draw() { draw_tama_ui("Loading... x"); }

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
  animation.animation = &pinata_splash_animation;
  animation.loop = true;
  animation.pos_draw_cb = pos_splash_screen_draw;

  animations_module_run(animation);
  xTaskCreate(time_bar_task, "time_bar_task", 2048, NULL, 3, NULL);
  vTaskDelay(pdMS_TO_TICKS(3100));
  animations_module_stop();
}

////////////////////// SCANNING ANIM /////////////////////
const uint16_t pinata_scan_order[] = {1, 2, 1, 2, 3, 4, 3, 4};
const uint32_t pinata_scan_durations_ms[] = {1000, 600, 400,  1200,
                                             700,  900, 1200, 600};

const animation_t pinata_scan_animation = {
    .bitmaps = pinata_bitmaps,
    .bitmaps_len = sizeof(pinata_bitmaps) / sizeof(bitmap_t),
    .order = pinata_scan_order,
    .duration_ms = pinata_scan_durations_ms,
    .frames_len = MIN(sizeof(pinata_scan_order) / sizeof(uint16_t),
                      sizeof(pinata_scan_durations_ms) / sizeof(uint32_t))};

static void pos_scan_draw() { draw_tama_ui("Scanning.. x"); }

void tama_anim_scan() {
  animations_module_ctx_t animation = {0};
  animation.animation = &pinata_scan_animation;
  animation.loop = true;
  animation.pos_draw_cb = pos_scan_draw;

  animations_module_run(animation);
  // xTaskCreate(time_bar_task, "time_bar_task", 2048, NULL, 3, NULL);
  vTaskDelay(pdMS_TO_TICKS(100000));
  animations_module_stop();
}