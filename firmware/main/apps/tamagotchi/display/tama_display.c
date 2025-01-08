#include "tama_display.h"

#include "bitmaps.h"
#include "hw.h"

#include "oled_screen.h"

static uint8_t matrix_buffer[LCD_HEIGHT][LCD_WIDTH / 8] = {{0}};
static bool_t icon_buffer[ICON_NUM] = {0};

void print_matrix_buffer() {
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
    oled_screen_display_bitmap(bitmaps + i * 18, i * 16 - 5, y + 6, 16, 9,
                               OLED_DISPLAY_NORMAL);
  }
  if (icon_buffer[7]) {
    drawTriangle(7 * 16 + 5, y);
    oled_screen_display_bitmap(bitmaps + 7 * 18, 7 * 16 - 5, y + 6, 16, 9,
                               OLED_DISPLAY_NORMAL);
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
  for (uint8_t j = 0; j < LCD_HEIGHT; j++) {
    drawTamaRow(j, 3 * j);
  }
  drawTamaSelection(49);
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
}