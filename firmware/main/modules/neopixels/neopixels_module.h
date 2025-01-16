#pragma once

#include "esp_err.h"

#define MAX_LED_NUMBER 4
/**
 * @brief Initialize the neopixels module
 */
void neopixels_module_begin();

/**
 * @brief Deinitialize the neopixels module
 *
 * @return
 * - ESP_OK: Deinitialize successfully
 * - ESP_FAIL: Deinitialize failed because some other error occurred
 */
esp_err_t neopixels_module_end();

/**
 * @brief Set RGB for a specific pixel
 *
 * @param index: index of pixel to set
 * @param red: red part of color
 * @param green: green part of color
 * @param blue: blue part of color
 *
 * @return
 * - ESP_OK: Set RGB for a specific pixel successfully
 * - ESP_ERR_INVALID_ARG: Set RGB for a specific pixel failed because of
 *   invalid parameters
 * - ESP_FAIL: Set RGB for a specific pixel failed because other error
 *   occurred
 */
esp_err_t neopixels_set_pixel(uint32_t index, uint32_t red, uint32_t green,
                              uint32_t blue);

/**
 * @brief Set RGB for a group of pixels
 *
 * @param number_of_pixels: number of pixels to set
 * @param red: red part of color
 * @param green: green part of color
 * @param blue: blue part of color
 *
 * @return
 * - ESP_OK: Set RGB for a specific pixel successfully
 * - ESP_ERR_INVALID_ARG: Set RGB for a specific pixel failed because of
 *   invalid parameters
 * - ESP_FAIL: Set RGB for a specific pixel failed because other error
 *   occurred
 */
esp_err_t neopixels_set_pixels(uint32_t number_of_pixels, uint32_t red,
                               uint32_t green, uint32_t blue);

/**
 * @brief Refresh memory colors to LEDs
 *
 * @return
 * - ESP_OK: Refresh successfully
 * - ESP_FAIL: Refresh failed because some other error occurred
 *
 * @note:
 *      After updating the LED colors in the memory, a following invocation of
 * this API is needed to flush colors to strip.
 */
esp_err_t neopixels_refresh();
