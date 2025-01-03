#include "tama_input.h"

#include "hw.h"

#include "keyboard_module.h"

static void input_cb(uint8_t button, uint8_t event) {
  if (event != BUTTON_PRESS_DOWN && event != BUTTON_PRESS_UP) {
    return;
  }

  switch (button) {
  case BUTTON_BACK:
    break;
  case BUTTON_LEFT:
    hw_set_button(BTN_LEFT, event == BUTTON_PRESS_DOWN ? BTN_STATE_PRESSED
                                                       : BTN_STATE_RELEASED);
    break;
  case BUTTON_MIDDLE:
    hw_set_button(BTN_MIDDLE, event == BUTTON_PRESS_DOWN ? BTN_STATE_PRESSED
                                                         : BTN_STATE_RELEASED);
    break;
  case BUTTON_RIGHT:
    hw_set_button(BTN_RIGHT, event == BUTTON_PRESS_DOWN ? BTN_STATE_PRESSED
                                                        : BTN_STATE_RELEASED);
    break;
  }
}

void tama_input_begin() {
  keyboard_module_begin();
  keyboard_module_set_input_callback(input_cb);
}
