#include "tama_input.h"

#include "hw.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "keyboard_module.h"
#include "menus_module.h"
#include "tama_app.h"

typedef struct {
  uint8_t button;
  uint8_t event;
} ButtonEvent_t;

#define QUEUE_SIZE 10
QueueHandle_t xQueue;

static void input_cb(uint8_t button, uint8_t event) {
  ButtonEvent_t eventToSend;
  if (event != BUTTON_PRESS_DOWN && event != BUTTON_PRESS_UP) {
    return;
  }
  if (button == BUTTON_BACK && event == BUTTON_PRESS_DOWN) {
    tama_app_exit();
    return;
  }
  eventToSend.button = button;
  eventToSend.event = event;
  xQueueSend(xQueue, &eventToSend, 0);
}

int hal_handler() {
  ButtonEvent_t receivedEvent;
  if (xQueueReceive(xQueue, &receivedEvent, 0) == pdPASS) {
    uint8_t button = receivedEvent.button;
    uint8_t event = receivedEvent.event;
    hw_set_button(button, event == BUTTON_PRESS_DOWN ? BTN_STATE_PRESSED
                                                     : BTN_STATE_RELEASED);
  }
  return 0;
}

void tama_input_begin() {
  xQueue = xQueueCreate(QUEUE_SIZE, sizeof(ButtonEvent_t));
  keyboard_module_begin();
  keyboard_module_set_input_callback(input_cb);
  // menus_module_set_app_state(true, input_cb);
}
