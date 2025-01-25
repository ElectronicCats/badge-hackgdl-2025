#include "tama_scan.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "menus_module.h"

#include "tama_anim.h"
#include "tama_friends.h"
#include "tama_now.h"
#include "tama_scenes.h"

void tama_scan_deinit();

static bool scanning = false;

static void on_new_friend_animation_end() {
  tama_anim_stop_any();
  vTaskDelay(pdMS_TO_TICKS(500));
  tama_anim_scan();
}

static void on_friend_detected(bool new) {
  tama_anim_stop_any();
  vTaskDelay(pdMS_TO_TICKS(500));
  tama_anim_new_friend(on_new_friend_animation_end);
}

static void tama_scan_input_cb(uint8_t button, uint8_t event) {
  if (button != BUTTON_BACK || event != BUTTON_PRESS_DOWN) {
    return;
  }

  tama_scan_deinit();
}

void tama_scan_begin() {
  scanning = true;
  menus_module_set_app_state(true, tama_scan_input_cb);
  tama_anim_scan();
  tama_now_begin();
  tama_friend_set_detected_cb(on_friend_detected);
}

void tama_scan_deinit() {
  tama_anim_stop_any();
  scanning = false;
  tama_now_deinit();
  tama_scenes_friends();
}