#include "display_module.h"

#define PIN_SDA 6
#define PIN_SCL 7

#define ROTATION_0 U8G2_R0
#define ROTATION_180 U8G2_R2
#define ROTATION ROTATION_0

// static u8g2_t u8g2;

// u8g2_t *display_module_get_u8g2() { return &u8g2; }

// void display_module_begin() {
//   u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
//   u8g2_esp32_hal.bus.i2c.sda = PIN_SDA;
//   u8g2_esp32_hal.bus.i2c.scl = PIN_SCL;
//   u8g2_esp32_hal_init(u8g2_esp32_hal);
//   u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, ROTATION,
//   u8g2_esp32_i2c_byte_cb,
//                                         u8g2_esp32_gpio_and_delay_cb);
//   u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);
//   u8g2_InitDisplay(&u8g2);
//   u8g2_SetPowerSave(&u8g2, 0);
//   u8g2_ClearBuffer(&u8g2);
// }