/*
 * ArduinoGotchi - A real Tamagotchi emulator for Arduino ESP32
 *
 * Copyright (C) 2022 Gary Kwok - Arduino Uno Implementation
 * Copyright (C) 2022 Marcel Ochsendorf - ESP32 Plattform Support
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include "hw.h"
#include "tamalib.h"

#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "tama_buzzer.h"
#include "tama_display.h"
#include "tama_friends.h"
#include "tama_input.h"
#include "tama_scenes.h"
#include "tama_state.h"

#include "preferences.h"

#define SPEED 1
#define SPEED_MEM "speed"

#define TAMA_DISPLAY_FREQ 1000000
#define TAMA_DISPLAY_FRAMERATE 10
#define AUTOSAVE_INTERVAL_S 10

static uint8_t speed;
volatile bool is_running = false;

void tama_app_set_speed(uint8_t _speed);

/**** TamaLib Specific Variables ****/
static cpu_state_t cpuState;
static unsigned long lastSaveTimestamp = 0;
/************************************/

static void hal_halt(void);
static void hal_log(log_level_t level, char *buff, ...);
static void hal_sleep_until(timestamp_t ts);
static timestamp_t hal_get_timestamp(void);
static void hal_update_screen();

static hal_t hal = {
    .halt = &hal_halt,
    .log = &hal_log,
    .sleep_until = &hal_sleep_until,
    .get_timestamp = &hal_get_timestamp,
    .update_screen = &hal_update_screen,
    .set_lcd_matrix = &hal_set_lcd_matrix,
    .set_lcd_icon = &hal_set_lcd_icon,
    .set_frequency = &hal_set_frequency,
    .play_frequency = &hal_play_frequency,
    .handler = &hal_handler,
};

static void hal_halt(void) { printf("Halt!\n"); }

static void hal_log(log_level_t level, char *buff, ...) { printf("buff\n"); }

static void hal_sleep_until(timestamp_t ts) {
  //  printf("Sleep\n");
}

static timestamp_t hal_get_timestamp(void) { return (esp_timer_get_time()); }

static void hal_update_screen() {
  if (!is_running) {
    return;
  }
  tama_display_update_screen();
}

static void dumpStateToSerial() {
  uint16_t i, count = 0;
  cpu_get_state(&cpuState);
  u4_t *memTemp = cpuState.memory;
  uint8_t *cpuS = (uint8_t *)&cpuState;
  printf("\nstatic const uint8_t hardcodedState[] = {\n");
  for (i = 0; i < sizeof(cpu_state_t); i++, count++) {
    printf("0x%02X,", cpuS[i]);
    if ((count % 16) == 15)
      printf("\n");
  }
  for (i = 0; i < MEMORY_SIZE; i++, count++) {
    printf("0x%02X,", memTemp[i]);
    if ((count % 16) == 15)
      printf("\n");
  }
  printf("};\n");
}

static void tamalib_begin() {
  tama_input_begin();
  tama_display_begin();
  tama_buzzer_begin();

  tamalib_register_hal(&hal);
  tamalib_set_framerate(TAMA_DISPLAY_FRAMERATE);
  tamalib_init(TAMA_DISPLAY_FREQ);

  tama_state_begin();
  tama_state_load(&cpuState);

  tama_app_set_speed(preferences_get_uchar(SPEED_MEM, SPEED));
  // dumpStateToSerial();
}

static void tama_app_task() {
  is_running = true;
  uint8_t i = 0;
  while (is_running) {
    tamalib_mainloop_step_by_step();
    if (esp_timer_get_time() - lastSaveTimestamp >
        AUTOSAVE_INTERVAL_S * 1000000) {
      tama_state_save(&cpuState);
      lastSaveTimestamp = esp_timer_get_time();
    }
    if (!(++i % (6 * speed))) {
      i = 0;
      vTaskDelay(1);
    }
  }
  tama_state_save(&cpuState);
  vTaskDelete(NULL);
}

void tama_app_begin() {
  tamalib_begin();
  tama_friends_begin();
  xTaskCreatePinnedToCore(tama_app_task, "app_task", 4096, NULL, 15, NULL, 0);
}

void tama_app_exit() {
  is_running = false;
  vTaskDelay(pdMS_TO_TICKS(20));
  tama_scenes_main();
}

void tama_app_set_speed(uint8_t _speed) { speed = _speed; }