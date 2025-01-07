#include "tama_display.h"

#include "bitmaps.h"
#include "hw.h"

#include "display_module.h"

static bool_t matrix_buffer[LCD_HEIGHT][LCD_WIDTH / 8] = {{0}};
static bool_t icon_buffer[ICON_NUM] = {0};
static u8g2_t *u8g2;

static void drawTriangle(uint8_t x, uint8_t y) {
  u8g2_DrawLine(u8g2, x + 1, y + 1, x + 5, y + 1);
  u8g2_DrawLine(u8g2, x + 2, y + 2, x + 4, y + 2);
  u8g2_DrawLine(u8g2, x + 3, y + 3, x + 3, y + 3);
}

static void drawTamaSelection(uint8_t y) {
  uint8_t i;
  for (i = 0; i < 7; i++) {
    if (icon_buffer[i]) {
      drawTriangle(i * 16 + 5, y);
    }
    u8g2_DrawXBMP(u8g2, i * 16 + 4, y + 6, 16, 9, bitmaps + i * 18);
  }
  if (icon_buffer[7]) {
    drawTriangle(7 * 16 + 5, y);
  }
  u8g2_DrawXBMP(u8g2, 7 * 16 + 4, y + 6, 16, 9, bitmaps + 7 * 18);
}

static void drawTamaRow(uint8_t tamaLCD_y, uint8_t ActualLCD_y, uint8_t thick) {
  uint8_t i;
  for (i = 0; i < LCD_WIDTH; i++) {
    uint8_t mask = 0b10000000;
    mask = mask >> (i % 8);
    if ((matrix_buffer[tamaLCD_y][i / 8] & mask) != 0) {
      u8g2_DrawBox(u8g2, 3 * i + 16, ActualLCD_y, 2, thick);
    }
  }
}

static void displayTama() {
  uint8_t j;
  u8g2_ClearBuffer(u8g2);
  drawTamaSelection(49);
  for (j = 0; j < LCD_HEIGHT; j++) {
    drawTamaRow(j, 3 * j, 2);
  }
  u8g2_SendBuffer(u8g2);
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
  // display_module_begin();
  u8g2 = display_module_get_u8g2();
}