#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "oled_screen.h"

static const char *TAG = "OLED_DRIVER";
oled_driver_t dev;
SemaphoreHandle_t oled_mutex;

uint8_t *last_buffer = NULL;

void oled_screen_begin() {
#if !defined(CONFIG_OLED_MODULE_DEBUG)
  esp_log_level_set(TAG, ESP_LOG_NONE);
#endif

  oled_mutex = xSemaphoreCreateMutex();
#if CONFIG_I2C_INTERFACE
  ESP_LOGI(TAG, "INTERFACE is i2c");
  ESP_LOGI(TAG, "CONFIG_SDA_GPIO=%d", CONFIG_SDA_GPIO);
  ESP_LOGI(TAG, "CONFIG_SCL_GPIO=%d", CONFIG_SCL_GPIO);
  ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d", CONFIG_RESET_GPIO);
  i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE

#if CONFIG_SPI_INTERFACE
  ESP_LOGI(TAG, "INTERFACE is SPI");
  ESP_LOGI(TAG, "CONFIG_MOSI_GPIO=%d", CONFIG_MOSI_GPIO);
  ESP_LOGI(TAG, "CONFIG_SCLK_GPIO=%d", CONFIG_SCLK_GPIO);
  ESP_LOGI(TAG, "CONFIG_CS_GPIO=%d", CONFIG_CS_GPIO);
  ESP_LOGI(TAG, "CONFIG_DC_GPIO=%d", CONFIG_DC_GPIO);
  ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d", CONFIG_RESET_GPIO);
  spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO,
                  CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_SPI_INTERFACE

#if CONFIG_FLIP
  dev._flip = true;
  ESP_LOGW(TAG, "Flip upside down");
#endif

#if CONFIG_RESOLUTION_128X64
  ESP_LOGI(TAG, "PANEL: 128x64");
  oled_driver_init(&dev, 128, 64);
#elif CONFIG_RESOLUTION_128X32
  ESP_LOGI(TAG, "PANEL: 128x32");
  oled_driver_init(&dev, 128, 32);
#endif
}

void oled_screen_get_last_buffer() {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  last_buffer = (uint8_t *)malloc(dev._pages * dev._width * sizeof(uint8_t));
  oled_driver_get_buffer(&dev, last_buffer);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_set_last_buffer() {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  if (last_buffer != NULL) {
    oled_driver_set_buffer(&dev, last_buffer);
    oled_driver_show_buffer(&dev);
    free(last_buffer);
    last_buffer = NULL;
  }
  xSemaphoreGive(oled_mutex);
}

void oled_screen_clear() {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_clear_screen(&dev, OLED_DISPLAY_NORMAL);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_display_show() {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_show_buffer(&dev);
  xSemaphoreGive(oled_mutex);
}
void oled_screen_clear_buffer() {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_clear_buffer(&dev);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_display_text(char *text, int x, int page, bool invert) {
  if (text == NULL) {
    ESP_LOGE(TAG, "Text is NULL");
    return;
  }

  uint8_t _x = x + (strlen(text) * 8) > 128 ? 0 : x;
  if (_x != x) {
    ESP_LOGW(TAG, "Text %s is too long for the screen, x offset: %d", text, _x);
  }

  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_display_text(&dev, page, text, _x, invert);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_display_text_center(char *text, int page, bool invert) {
  if (text == NULL) {
    ESP_LOGE(TAG, "Text is NULL");
    return;
  }

  int text_length = strlen(text);
  if (text_length > MAX_LINE_CHAR) {
    ESP_LOGE(TAG, "Text too long to center");
    oled_screen_display_text(text, 0, page, invert);
    return;
  }

  uint8_t middle_x_coordinate = 128 / 2;
  uint8_t half_text_length_px = (text_length * 8 / 2);
  uint8_t x_offset = middle_x_coordinate - half_text_length_px;
  oled_screen_display_text(text, x_offset, page, invert);
}

void oled_screen_clear_line(int x, int page, bool invert) {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_clear_line(&dev, x, page, invert);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_buffer_bitmap(const uint8_t *bitmap, int x, int y, int width,
                               int height, bool invert) {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_bitmaps(&dev, x, y, bitmap, width, height, invert);
  xSemaphoreGive(oled_mutex);
}
void oled_screen_display_bitmap(const uint8_t *bitmap, int x, int y, int width,
                                int height, bool invert) {
  oled_screen_buffer_bitmap(bitmap, x, y, width, height, invert);
  oled_screen_display_show();
}

void oled_screen_draw_pixel(int x, int y, bool invert) {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_draw_pixel(&dev, x, y, invert);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_draw_rect(int x, int y, int width, int height, bool invert) {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_draw_rect(&dev, x, y, width, height, invert);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_draw_line(int x1, int y1, int x2, int y2, bool invert) {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_draw_line(&dev, x1, y2, x2, y2, invert);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_draw_vline(int x, int y, int height, bool invert) {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_draw_vline(&dev, x, y, height, invert);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_draw_hline(int x, int y, int width, bool invert) {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_draw_hline(&dev, x, y, width, invert);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_draw_box(int x, int y, int width, int height, bool invert) {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  while (height != 0) {
    oled_driver_draw_hline(&dev, x, y++, width, invert);
    height--;
  }
  xSemaphoreGive(oled_mutex);
}

/// @brief Display a box around the selected item
void oled_screen_display_selected_item_box() {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_draw_custom_box(&dev);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_display_card_border() {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_draw_modal_box(&dev, 0, 3);
  xSemaphoreGive(oled_mutex);
}

void oled_screen_display_text_splited(char *p_text, int *p_started_page,
                                      int invert) {
  if (strlen(p_text) > MAX_LINE_CHAR) {
    char temp[100];
    strncpy(temp, p_text, 100);

    char *token = strtok(temp, " ");
    char current_line[MAX_LINE_CHAR] = "";
    while (token != NULL) {
      if (strlen(current_line) + strlen(token) + 1 <= MAX_LINE_CHAR) {
        if (strlen(current_line) > 0) {
          strcat(current_line, " ");
        }
        strcat(current_line, token);
      } else {
        oled_screen_display_text(current_line, 3, *p_started_page, invert);
        (*p_started_page)++;
        strcpy(current_line, token);
      }
      token = strtok(NULL, " ");
    }
    if (strlen(current_line) > 0) {
      oled_screen_display_text(current_line, 3, *p_started_page, invert);
      (*p_started_page)++;
    }
  } else {
    oled_screen_display_text(p_text, 3, *p_started_page, invert);
    (*p_started_page)++;
  }
}

void oled_screen_draw_loading_bar(uint8_t value, uint8_t x, uint8_t y,
                                  uint8_t w, uint8_t h) {
  uint8_t widht = (uint32_t)value * (w - 4) / 100;
  oled_screen_draw_rect(x, y, w, h, OLED_DISPLAY_NORMAL);
  oled_screen_draw_box(x + 2, y + 2, widht, h - 4, OLED_DISPLAY_NORMAL);
}

void oled_screen_fade_out() {
  xSemaphoreTake(oled_mutex, portMAX_DELAY);
  oled_driver_fadeout(&dev);
  xSemaphoreGive(oled_mutex);
}

uint8_t oled_screen_get_pages() { return dev._pages; }
