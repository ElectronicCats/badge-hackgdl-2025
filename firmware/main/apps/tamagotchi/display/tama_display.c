#include "tama_display.h"

#include "bitmaps.h"
#include "hw.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

#include "oled_screen.h"
#include "preferences.h"
#include "tama_friends.h"

#define NICKNAME_MEM "nickname"
#define MAGIC_NICKNAME_MEM "mnickname"
#define MAGIC_NICKNAME 12

static char nickname[TAMA_DISPLAY_MAX_NICKNAME_LEN] = "NickName";
static char header_str[20];

static uint8_t matrix_buffer[LCD_HEIGHT][LCD_WIDTH / 8] = {{0}};
static bool_t icon_buffer[ICON_NUM] = {0};

static bool frame = false;
static uint16_t friends = 0;

static void print_matrix_buffer() {
  for (uint8_t y = 0; y < 16; y++) {
    for (uint8_t byte_index = 0; byte_index < 4; byte_index++) {
      uint8_t byte = matrix_buffer[y][byte_index];
      for (uint8_t bit_index = 0; bit_index < 8; bit_index++) {
        if (byte & (0x80 >> bit_index)) {
          printf("1");
        } else {
          printf("0");
        }
      }
      printf(" ");
    }
    printf("\n");
  }
}

static void header_task() {
  while (1) {
    if (!strcmp(header_str, nickname) && tama_friends_get_ctx()) {
      snprintf(header_str, sizeof(header_str), "Find %03d/300",
               tama_friends_get_count());
      vTaskDelay(pdMS_TO_TICKS(3000));
    } else {
      strncpy(header_str, nickname, sizeof(header_str));
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
  }
}

static void drawTriangle(uint8_t x, uint8_t y) {
  oled_screen_draw_hline(x + 1, y + 2, 5, OLED_DISPLAY_NORMAL);
  oled_screen_draw_hline(x + 2, y + 3, 3, OLED_DISPLAY_NORMAL);
  oled_screen_draw_hline(x + 3, y + 4, 1, OLED_DISPLAY_NORMAL);
}

static void drawTamaSelection() {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    if (icon_buffer[i]) {
      drawTriangle(i / 4 ? 119 : 1, i % 4 * 16);
    }
    if (i == 7 && !icon_buffer[7]) {
      return;
    }
    oled_screen_buffer_bitmap(bitmaps + i * 18, i / 4 ? 109 : -9,
                              i % 4 * 16 + 6, 16, 9, OLED_DISPLAY_NORMAL);
  }
}

static void drawTamaRow(uint8_t tamaLCD_y, uint8_t ActualLCD_y) {
  uint8_t i;
  for (i = 0; i < LCD_WIDTH; i++) {
    uint8_t mask = 0b10000000;
    mask = mask >> (i % 8);
    if ((matrix_buffer[tamaLCD_y][i / 8] & mask) != 0) {
      oled_screen_draw_box(3 * i + 16, ActualLCD_y + 12, 2, 2,
                           OLED_DISPLAY_NORMAL);
    }
  }
}

static void displayTama() {
  oled_screen_clear_buffer();
  drawTamaSelection();
  oled_screen_draw_rect(12, 8, 103, 55, OLED_DISPLAY_NORMAL);
  for (uint8_t j = 0; j < LCD_HEIGHT; j++) {
    drawTamaRow(j, 3 * j);
  }
  oled_screen_draw_box(12, 0, 103, 8, OLED_DISPLAY_NORMAL);
  oled_screen_display_text_center(header_str, 0, OLED_DISPLAY_INVERT);
  oled_screen_display_show();
  // vTaskDelay(65);
}

void hal_set_lcd_matrix(uint8_t x, uint8_t y, bool val) {
  uint8_t mask;
  if (val) {
    mask = 0b10000000 >> (x % 8);
    matrix_buffer[y][x / 8] |= mask;
  } else {
    mask = ~(0b10000000 >> (x % 8));
    matrix_buffer[y][x / 8] &= mask;
  }
  if (y == 15 && x == 31) {
    displayTama();
    // print_matrix_buffer();
  }
}

void hal_set_lcd_icon(uint8_t icon, bool val) { icon_buffer[icon] = val; }

void tama_display_update_screen() {
  // displayTama();
}

void tama_display_begin() {
  // oled_screen_begin();
  if (preferences_get_uchar(MAGIC_NICKNAME_MEM, 0) == MAGIC_NICKNAME) {
    preferences_get_bytes(NICKNAME_MEM, nickname,
                          TAMA_DISPLAY_MAX_NICKNAME_LEN);
  }
  strncpy(header_str, nickname, sizeof(header_str));

  xTaskCreate(header_task, "header_task", 2048, NULL, 1, NULL);
}

void tama_display_set_nickname(char *new_nickname) {
  strncpy(nickname, new_nickname, TAMA_DISPLAY_MAX_NICKNAME_LEN - 1);
  preferences_put_bytes(NICKNAME_MEM, nickname, TAMA_DISPLAY_MAX_NICKNAME_LEN);
  preferences_put_uchar(MAGIC_NICKNAME_MEM, MAGIC_NICKNAME);
}

char *tama_display_get_nickname() { return nickname; }