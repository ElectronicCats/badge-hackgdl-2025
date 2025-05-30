#include "tama_anim.h"

#include <string.h>

#include "animation_t.h"
#include "animations_module.h"
#include "oled_screen.h"

#include "bitmaps.h"
#include "get_random.h"
#include "tama_bmps.h"
#include "tama_scan.h"

static char friend_detected_name[13];

static void draw_tama_ui(const char *str) {
  if (!str) {
    return;
  }
  for (uint8_t i = 0; i < 8; i++) {
    oled_screen_buffer_bitmap(bitmaps + i * 18, i / 4 ? 109 : -9,
                              i % 4 * 16 + 6, 16, 9, OLED_DISPLAY_NORMAL);
  }
  oled_screen_draw_rect(12, 8, 103, 55, OLED_DISPLAY_NORMAL);
  oled_screen_draw_box(12, 0, 103, 8, OLED_DISPLAY_NORMAL);
  oled_screen_display_text_center(str, 0, OLED_DISPLAY_INVERT);
  if (str[11] == 'x') {
    oled_screen_draw_vline(100, 1, 7, OLED_DISPLAY_INVERT);
  }
}

const uint16_t pinata_splash_order[] = {9, 10, 11};
const uint32_t pinata_splash_durations_ms[] = {200, 200, 200};

const animation_t pinata_splash_animation = {
    .bitmaps = pinata_bitmaps,
    .bitmaps_len = sizeof(pinata_bitmaps) / sizeof(bitmap_t),
    .order = pinata_splash_order,
    .duration_ms = pinata_splash_durations_ms,
    .frames_len = sizeof(pinata_splash_order) / sizeof(uint16_t)};

static void pos_splash_screen_draw() { draw_tama_ui("Loading... x"); }

static void time_bar_task() {
  for (uint8_t i = 0; i < 100; i++) {
    oled_screen_draw_loading_bar(i + 1, 15, 55, 97, 5, OLED_DISPLAY_NORMAL);
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
    .frames_len = sizeof(pinata_scan_order) / sizeof(uint16_t)};

static void pos_scan_draw() { draw_tama_ui("Scanning.. x"); }

void tama_anim_scan() {
  animations_module_ctx_t animation = {0};
  animation.animation = &pinata_scan_animation;
  animation.loop = true;
  animation.pos_draw_cb = pos_scan_draw;

  animations_module_run(animation);
}

////////////////////// NEW FRIEND ANIM /////////////////////
const uint16_t pinata_new_friend_order[] = {0, 5, 0, 5, 0, 6, 0, 6,
                                            0, 6, 0, 6, 0, 5, 0, 5};
const uint32_t pinata_new_friend_durations_ms[] = {
    600, 600, 600, 600, 600, 50, 80, 50, 80, 50, 80, 50, 80, 600, 600, 600};

const animation_t pinata_new_friend_animation = {
    .bitmaps = pinata_bitmaps,
    .bitmaps_len = sizeof(pinata_bitmaps) / sizeof(bitmap_t),
    .order = pinata_new_friend_order,
    .duration_ms = pinata_new_friend_durations_ms,
    .frames_len = sizeof(pinata_new_friend_order) / sizeof(uint16_t)};

static void pos_new_friend_draw() { draw_tama_ui("New Friend x"); }

static void new_friend_anim_shake() {
  for (uint8_t i = 0; i < 15; i++) {
    animations_module_set_pos(get_random_uint8() % 10, get_random_uint8() % 10);
    vTaskDelay(pdMS_TO_TICKS(pinata_new_friend_durations_ms[i]));
  }
  vTaskDelete(NULL);
}

static void new_friend_exit_cb() {}

void tama_anim_new_friend(void (*new_friend_exit)()) {
  animations_module_ctx_t animation = {0};
  animation.animation = &pinata_new_friend_animation;
  animation.loop = false;
  animation.pos_draw_cb = pos_new_friend_draw;
  animation.exit_cb = new_friend_exit;

  xTaskCreate(new_friend_anim_shake, "shake_task", 2048, NULL, 3, NULL);
  animations_module_run(animation);
}

////////////////////// FRIEND ANIM /////////////////////
const uint16_t pinata_friend_order[] = {0, 5, 0, 5, 0};
const uint32_t pinata_friend_durations_ms[] = {600, 600, 600, 600, 600};

const animation_t pinata_friend_animation = {
    .bitmaps = pinata_bitmaps,
    .bitmaps_len = sizeof(pinata_bitmaps) / sizeof(bitmap_t),
    .order = pinata_friend_order,
    .duration_ms = pinata_friend_durations_ms,
    .frames_len = sizeof(pinata_friend_order) / sizeof(uint16_t)};

static void pos_friend_draw() { draw_tama_ui(friend_detected_name); }

void tama_anim_friend(char *name, void (*friend_exit)()) {
  strncpy(friend_detected_name, name, 13);

  animations_module_ctx_t animation = {0};
  animation.animation = &pinata_friend_animation;
  animation.loop = false;
  animation.pos_draw_cb = pos_friend_draw;
  animation.exit_cb = friend_exit;

  animations_module_run(animation);
}

void tama_anim_stop_any() { animations_module_delete(); }