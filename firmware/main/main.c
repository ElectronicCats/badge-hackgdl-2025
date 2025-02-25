#include <stdio.h>

#include "buzzer.h"
#include "cat_console.h"
#include "keyboard_module.h"
#include "menus_module.h"
#include "neopixels_module.h"
#include "oled_screen.h"
#include "preferences.h"

#define PIN_BUZZER CONFIG_GPIO_BUZZER

void app_main(void) {
  buzzer_begin(PIN_BUZZER);
  neopixels_module_begin();
  preferences_begin();
  keyboard_module_begin();
  oled_screen_begin();
  menus_module_begin();
  cat_console_begin(); // Contains a while(true) loop, it must be at the end
}
