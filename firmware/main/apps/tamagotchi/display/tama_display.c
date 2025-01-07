#include "tama_display.h"

#include "bitmaps.h"
#include "hw.h"

#include "oled_screen.h"

static bool_t matrix_buffer[LCD_HEIGHT][LCD_WIDTH / 8] = {{0}};
static bool_t icon_buffer[ICON_NUM] = {0};

static void drawTriangle(uint8_t x, uint8_t y) {
  oled_screen_draw_hline(x + 1, y + 1, 5, OLED_DISPLAY_NORMAL);
  oled_screen_draw_hline(x + 2, y + 2, 3, OLED_DISPLAY_NORMAL);
  oled_screen_draw_hline(x + 3, y + 3, 1, OLED_DISPLAY_NORMAL);
}

static void drawTamaSelection(uint8_t y) {
  uint8_t i;
  for (i = 0; i < 7; i++) {
    if (icon_buffer[i]) {
      drawTriangle(i * 16 + 5, y);
    }
  }
  if (icon_buffer[7]) {
    drawTriangle(7 * 16 + 5, y);
  }
}

static void drawTamaRow(uint8_t tamaLCD_y, uint8_t ActualLCD_y) {
  uint8_t i;
  for (i = 0; i < LCD_WIDTH; i++) {
    uint8_t mask = 0b10000000;
    mask = mask >> (i % 8);
    if ((matrix_buffer[tamaLCD_y][i / 8] & mask) != 0) {
      oled_screen_draw_box(3 * i + 16, ActualLCD_y, 2, 2, OLED_DISPLAY_NORMAL);
    }
  }
}
static void displayTama() {
  oled_screen_clear_buffer();
  drawTamaSelection(49);
  for (uint8_t j = 0; j < LCD_HEIGHT; j++) {
    drawTamaRow(j, 3 * j);
  }
  oled_screen_display_show();
  vTaskDelay(60);
}

void hal_set_lcd_matrix(uint8_t x, uint8_t y, bool val) {
  uint8_t mask;
  if (val) {
    mask = 0b10000000 >> (x % 8);
    matrix_buffer[y][x / 8] = matrix_buffer[y][x / 8] | mask;
  } else {
    mask = 0b01111111;
    for (uint8_t i = 0; i < (x % 8); i++) {
      mask = (mask >> 1) | 0b10000000;
    }
    matrix_buffer[y][x / 8] = matrix_buffer[y][x / 8] & mask;
  }
}

void hal_set_lcd_icon(uint8_t icon, bool val) { icon_buffer[icon] = val; }

void tama_display_update_screen() { displayTama(); }

void tama_display_begin() {
  // oled_screen_begin();
}