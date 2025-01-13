#include "general_notification.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

#include "menus_module.h"
#include "oled_screen.h"

general_notification_ctx_t *notification_ctx = NULL;

static void draw_notification() {
  oled_screen_clear();
  oled_screen_display_card_border();
  int page = 2;
  oled_screen_display_text_center(notification_ctx->head, page,
                                  OLED_DISPLAY_NORMAL);
  page++;
  if (strlen(notification_ctx->body) > MAX_LINE_CHAR) {
    oled_screen_display_text_splited(notification_ctx->body, &page,
                                     OLED_DISPLAY_NORMAL);
    oled_screen_display_show();
    return;
  }
  oled_screen_display_text_center(notification_ctx->body, page,
                                  OLED_DISPLAY_NORMAL);
  oled_screen_display_show();
}

static void free_ctx() {
  free(notification_ctx);
  notification_ctx = NULL;
}

void general_notification(general_notification_ctx_t ctx) {
  if (notification_ctx) {
    free_ctx();
  }
  notification_ctx = calloc(1, sizeof(general_notification_ctx_t));
  notification_ctx->head = ctx.head;
  notification_ctx->body = ctx.body;
  notification_ctx->on_start_cb = ctx.on_start_cb;
  notification_ctx->on_end_cb = ctx.on_end_cb;
  notification_ctx->duration_ms = ctx.duration_ms;

  oled_screen_get_last_buffer();
  menus_module_disable_input();
  if (notification_ctx->on_start_cb) {
    notification_ctx->on_start_cb();
  }
  draw_notification();
  vTaskDelay(pdMS_TO_TICKS(notification_ctx->duration_ms));
  menus_module_enable_input();
  oled_screen_set_last_buffer();
  oled_screen_display_show();
  void (*exit_cb)() = notification_ctx->on_end_cb;
  free_ctx();
  if (exit_cb) {
    exit_cb();
  }
}