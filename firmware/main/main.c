#include <stdio.h>

#include "buzzer.h"
#include "display_module.h"
#include "keyboard_module.h"
#include "menus_module.h"
#include "preferences.h"
#include "tama_app.h"

#define PIN_BUZZER GPIO_NUM_2

void app_main(void) {
  buzzer_begin(PIN_BUZZER);
  preferences_begin();
  display_module_begin();
  keyboard_module_begin();
  menus_module_begin();
  // tama_app_begin();
}
