#include "general_scrolling_text.h"

#include "bitmaps_general.h"
#include "keyboard_module.h"
#include "menus_module.h"
#include "oled_screen.h"

#include <string.h>

#define LINES_PER_PAGE 3

general_scrolling_text_ctx *scroll_text_ctx;

#define MAX_LINE_LENGTH 14

static char **split_text(const char *text, uint16_t *num_lines) {
  int len = strlen(text);
  int lines_count = 1;
  int line_len = 0;

  for (int i = 0; i < len; ++i) {
    if (text[i] == '\n') {
      lines_count++;
      line_len = 0;
    } else {
      line_len++;
      if (line_len > MAX_LINE_LENGTH) {
        lines_count++;
        line_len = 1;
      }
    }
  }

  char **lines = (char **)malloc(lines_count * sizeof(char *));
  if (!lines) {
    return NULL;
  }

  int current_line = 0;
  int start = 0;
  line_len = 0;

  for (int i = 0; i <= len; ++i) {
    if (!line_len && text[i] == ' ') {
      start++;
      continue;
    }
    if (text[i] == '\n' || text[i] == '\0') {
      lines[current_line] = (char *)malloc((line_len + 1));
      if (!lines[current_line]) {
        return NULL;
      }

      strncpy(lines[current_line], &text[start], line_len);
      lines[current_line][line_len] = '\0';

      current_line++;
      start = i + 1;
      line_len = 0;
    } else {
      line_len++;
    }

    if (line_len >= MAX_LINE_LENGTH) {
      lines[current_line] = (char *)malloc((line_len + 1));
      if (!lines[current_line]) {
        return NULL;
      }

      strncpy(lines[current_line], &text[start], line_len);
      lines[current_line][line_len] = '\0';

      current_line++;
      start = i + 1;
      line_len = 0;
    }
  }

  *num_lines = lines_count;
  return lines;
}

static void scroll_text_draw_window() {
  if (scroll_text_ctx == NULL) {
    return;
  }
  oled_screen_clear_buffer();
  if (scroll_text_ctx->banner) {
    oled_screen_display_text_center(scroll_text_ctx->banner, 0,
                                    OLED_DISPLAY_NORMAL);
  }

  oled_screen_display_card_border();

  char str[17];
  snprintf(str, sizeof(str), "%d/%d", scroll_text_ctx->current_idx + 1,
           scroll_text_ctx->pages);
  uint8_t arrows_x = 62 - strlen(str) * 4;
  oled_screen_draw_box(arrows_x - 8, 56, strlen(str) * 8 + 16, 8,
                       OLED_DISPLAY_INVERT);
  oled_screen_display_text_center(str, 7, OLED_DISPLAY_NORMAL);

  if (!(scroll_text_ctx->scroll_type == GENERAL_SCROLLING_TEXT_CLAMPED &&
        !scroll_text_ctx->current_idx)) {
    oled_screen_display_bitmap(simple_left_arrow_bmp, arrows_x - 8, 56, 8, 8,
                               OLED_DISPLAY_NORMAL);
  }
  if (!(scroll_text_ctx->scroll_type == GENERAL_SCROLLING_TEXT_CLAMPED &&
        scroll_text_ctx->current_idx >= scroll_text_ctx->pages - 1)) {
    oled_screen_display_bitmap(simple_right_arrow_bmp,
                               arrows_x + strlen(str) * 8, 56, 8, 8,
                               OLED_DISPLAY_NORMAL);
  }

  uint8_t start_idx = scroll_text_ctx->current_idx * LINES_PER_PAGE;
  uint8_t page = 0;
  for (uint16_t i = start_idx;
       i < start_idx + LINES_PER_PAGE && i < scroll_text_ctx->text_len;
       i++, page++) {
    oled_screen_display_text(scroll_text_ctx->text_arr[i], 2, 3 + page,
                             OLED_DISPLAY_NORMAL);
  }
  oled_screen_display_show();
}

static void scroll_text_draw_full_screen() {}

static void scroll_text_draw() {
  if (scroll_text_ctx->window_type == GENERAL_SCROLLING_TEXT_FULL_SCREEN) {
    scroll_text_draw_full_screen();
  } else {
    scroll_text_draw_window();
  }
}

static void scroll_text_ctx_free() {
  if (scroll_text_ctx) {
    if (scroll_text_ctx->text) {
      free(scroll_text_ctx->text_arr);
    }
    free(scroll_text_ctx);
    scroll_text_ctx = NULL;
  }
}

static void button_up_handler() {
  if (scroll_text_ctx->current_idx == 0) {
    if (scroll_text_ctx->scroll_type == GENERAL_SCROLLING_TEXT_INFINITE) {
      scroll_text_ctx->current_idx = scroll_text_ctx->pages - 1;
    } else {
      return;
    }
  } else {
    scroll_text_ctx->current_idx--;
  }
  scroll_text_draw();
}

static void button_down_handler() {
  if (scroll_text_ctx->current_idx >= scroll_text_ctx->pages - 1) {
    if (scroll_text_ctx->finish_cb) {
      scroll_text_ctx->finish_cb();
    }
    if (scroll_text_ctx->scroll_type == GENERAL_SCROLLING_TEXT_INFINITE) {
      scroll_text_ctx->current_idx = 0;
    } else {
      return;
    }
  } else {
    scroll_text_ctx->current_idx++;
  }
  scroll_text_draw();
}

static void scrolling_text_input_cb(uint8_t button_name, uint8_t button_event) {
  if (button_event != BUTTON_PRESS_DOWN) {
    return;
  }
  switch (button_name) {
  case BUTTON_BACK:
    void (*exit_cb)() = scroll_text_ctx->exit_cb;
    scroll_text_ctx_free();
    if (exit_cb) {
      exit_cb();
    }
    break;
  case BUTTON_MIDDLE:
    if (scroll_text_ctx->select_cb) {
      scroll_text_ctx->select_cb();
    }
    break;
  case BUTTON_LEFT:
    button_up_handler();
    break;
  case BUTTON_RIGHT:
    button_down_handler();
    break;
  default:
    break;
  }
}

void general_scrolling_text(general_scrolling_text_ctx ctx) {
  scroll_text_ctx_free();
  scroll_text_ctx = calloc(1, sizeof(general_scrolling_text_ctx));
  scroll_text_ctx->banner = ctx.banner;
  scroll_text_ctx->text = ctx.text;
  scroll_text_ctx->window_type = ctx.window_type;
  scroll_text_ctx->scroll_type = ctx.scroll_type;
  scroll_text_ctx->exit_cb = ctx.exit_cb;
  scroll_text_ctx->finish_cb = ctx.finish_cb;
  scroll_text_ctx->text_arr =
      split_text(scroll_text_ctx->text, &scroll_text_ctx->text_len);

  uint8_t pages = scroll_text_ctx->text_len / LINES_PER_PAGE;
  pages += pages * LINES_PER_PAGE < scroll_text_ctx->text_len;
  scroll_text_ctx->pages = pages;

  menus_module_set_app_state(true, scrolling_text_input_cb);
  scroll_text_draw();
}

void general_scrolling_text_array(general_scrolling_text_ctx ctx) {
  scroll_text_ctx_free();
  scroll_text_ctx = calloc(1, sizeof(general_scrolling_text_ctx));
  scroll_text_ctx->banner = ctx.banner;
  scroll_text_ctx->window_type = ctx.window_type;
  scroll_text_ctx->scroll_type = ctx.scroll_type;
  scroll_text_ctx->exit_cb = ctx.exit_cb;
  scroll_text_ctx->finish_cb = ctx.finish_cb;
  scroll_text_ctx->text_arr = ctx.text_arr;
  scroll_text_ctx->text_len = ctx.text_len;

  uint8_t pages = scroll_text_ctx->text_len / LINES_PER_PAGE;
  pages += pages * LINES_PER_PAGE < scroll_text_ctx->text_len;
  scroll_text_ctx->pages = pages;

  menus_module_set_app_state(true, scrolling_text_input_cb);
  scroll_text_draw();
}

uint16_t general_scrolling_text_get_current_idx() {
  return scroll_text_ctx->current_idx;
}