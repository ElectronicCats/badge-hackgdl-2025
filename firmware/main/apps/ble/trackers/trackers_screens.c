#include "trackers_screens.h"
#include "animations_task.h"
#include "bitmaps_general.h"
#include "freertos/FreeRTOS.h"
#include "general_screens.h"
#include <stdint.h>
#include <string.h>
// #include "led_events.h"
#include "oled_screen.h"
#include "trakers_scenes.h"

static char *list_trackers[20];

void trakers_screens_list_trakers() {
  uint8_t count = 0;
  while (count < 20) {
    if (!list_trackers[count]) {
      break;
    }
    printf("Roberto Tiene La Culpa\n");
    count++;
  }
  if (count) {
    trakers_scenes_list_menu(list_trackers, count);
    return;
  }
  trakers_scenes_no_devices_found();
}

void module_add_tracker_to_list(char *tracker_name) {
  uint8_t count = 0;
  while (count < 20) {
    if (!list_trackers[count]) {
      break;
    }
    printf("Roberto Tiene La Culpa\n");
    count++;
  }
  list_trackers[count] = tracker_name;
}

void module_update_scan_state(bool scanning) {
  set_scan_text(scanning ? "Scanning" : "Scan");
}

void module_update_tracker_name(char *tracker_name, uint16_t index) {
  list_trackers[index] = tracker_name;
}

void module_display_scanning() {
  // led_control_run_effect(led_control_pulse_leds);
  trakers_scenes_searching();
}

void module_display_device_detected(char *device_name) {
  // led_control_run_effect(led_control_pulse_leds);
  trakers_scenes_device_found(device_name);
}