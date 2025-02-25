/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include <stdio.h>

#include "neopixels_module.h"

static const char *TAG = "neopixels_module";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to
   blink, or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_GPIO_NEOPIXEL

static led_strip_handle_t led_strip;

void neopixels_module_begin(void) {
  ESP_LOGI(TAG, "Initializing neopixels module");
  /* LED strip initialization with the GPIO and pixels number*/
  led_strip_config_t strip_config = {
      .strip_gpio_num = BLINK_GPIO,
      .max_leds = MAX_LED_NUMBER,
  };
  led_strip_rmt_config_t rmt_config = {
      .resolution_hz = 10 * 1000 * 1000, // 10MHz
      .flags.with_dma = false,
  };
  ESP_ERROR_CHECK(
      led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
  /* Set all LED off to clear all pixels */
  led_strip_clear(led_strip);
}

esp_err_t neopixels_module_end(void) {
  led_strip_clear(led_strip);
  return led_strip_del(led_strip);
}

esp_err_t neopixels_set_pixel(uint32_t index, uint32_t red, uint32_t green,
                              uint32_t blue) {
  return led_strip_set_pixel(led_strip, index, red, green, blue);
}

esp_err_t neopixels_set_pixels(uint32_t number_of_pixels, uint32_t red,
                               uint32_t green, uint32_t blue) {
  esp_err_t err = ESP_OK;
  for (int i = 0; i < number_of_pixels; i++) {
    err = led_strip_set_pixel(led_strip, i, red, green, blue);
  }
  return err;
}

esp_err_t neopixels_refresh() { return led_strip_refresh(led_strip); }
