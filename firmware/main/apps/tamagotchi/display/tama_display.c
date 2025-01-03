#include "tama_display.h"

#include "bitmaps.h"
#include "hw.h"

#include "u8g2_esp32_hal.h"
#include <u8g2.h>

#define PIN_SDA 6
#define PIN_SCL 7

#define ROTATION_0 U8G2_R0
#define ROTATION_180 U8G2_R2
#define ROTATION ROTATION_0

static bool_t matrix_buffer[LCD_HEIGHT][LCD_WIDTH / 8] = {{0}};
static bool_t icon_buffer[ICON_NUM] = {0};
static u8g2_t u8g2;

static void drawTriangle(uint8_t x, uint8_t y) {
  u8g2_DrawLine(&u8g2, x + 1, y + 1, x + 5, y + 1);
  u8g2_DrawLine(&u8g2, x + 2, y + 2, x + 4, y + 2);
  u8g2_DrawLine(&u8g2, x + 3, y + 3, x + 3, y + 3);
}

static void drawTamaSelection(uint8_t y) {
  uint8_t i;
  for (i = 0; i < 7; i++) {
    if (icon_buffer[i]) {
      drawTriangle(i * 16 + 5, y);
    }
    u8g2_DrawXBMP(&u8g2, i * 16 + 4, y + 6, 16, 9, bitmaps + i * 18);
  }
  if (icon_buffer[7]) {
    drawTriangle(7 * 16 + 5, y);
  }
  u8g2_DrawXBMP(&u8g2, 7 * 16 + 4, y + 6, 16, 9, bitmaps + 7 * 18);
}

static void drawTamaRow(uint8_t tamaLCD_y, uint8_t ActualLCD_y, uint8_t thick) {
  uint8_t i;
  for (i = 0; i < LCD_WIDTH; i++) {
    uint8_t mask = 0b10000000;
    mask = mask >> (i % 8);
    if ((matrix_buffer[tamaLCD_y][i / 8] & mask) != 0) {
      u8g2_DrawBox(&u8g2, i + i + i + 16, ActualLCD_y, 2, thick);
    }
  }
}

static void displayTama() {
  uint8_t j;
  u8g2_ClearBuffer(&u8g2);
  drawTamaSelection(49);
  for (j = 0; j < LCD_HEIGHT; j++) {
    if (j != 5)
      drawTamaRow(j, j + j + j, 2);
    if (j == 5) {
      drawTamaRow(j, j + j + j, 1);
      drawTamaRow(j, j + j + j + 1, 1);
    }
  }
  u8g2_SendBuffer(&u8g2);
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

void hal_update_screen() { displayTama(); }

void tama_display_begin() {
  u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
  u8g2_esp32_hal.bus.i2c.sda = PIN_SDA;
  u8g2_esp32_hal.bus.i2c.scl = PIN_SCL;
  u8g2_esp32_hal_init(u8g2_esp32_hal);
  u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, ROTATION, u8g2_esp32_i2c_byte_cb,
                                        u8g2_esp32_gpio_and_delay_cb);
  u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_ClearBuffer(&u8g2);
}