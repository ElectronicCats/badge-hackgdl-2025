#include <stdio.h>

#include "buzzer.h"
#include "keyboard_module.h"
#include "menus_module.h"
#include "oled_screen.h"
#include "preferences.h"

#define PIN_BUZZER GPIO_NUM_2

void app_main(void) {
  buzzer_begin(PIN_BUZZER);
  preferences_begin();
  keyboard_module_begin();
  oled_screen_begin();
  menus_module_begin();
}
