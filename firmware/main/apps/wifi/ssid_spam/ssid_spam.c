#include "ssid_spam.h"

#include "esp_event.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "string.h"

#include "animations_task.h"
#include "keyboard_module.h"
#include "menus_module.h"
#include "ssid_spam_screens.h"

esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len,
                            bool en_sys_seq);

static uint8_t beacon_raw[] = {
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xba, 0xde,
    0xaf, 0xfe, 0x00, 0x06, 0xba, 0xde, 0xaf, 0xfe, 0x00, 0x06, 0x00, 0x00,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x64, 0x00, 0x31, 0x04,
    0x00, 0x00, 0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24,
    0x03, 0x01, 0x01, 0x05, 0x04, 0x01, 0x02, 0x00, 0x00,
};

static char *ssids[] = {
    "01 Never gonna give you up",  "02 Never gonna let you down",
    "03 Never gonna run around",   "04 and desert you",
    "05 Never gonna make you cry", "06 Never gonna say goodbye",
    "07 Never gonna tell a lie",   "08 and hurt you"};

#define BEACON_SSID_OFFSET 38
#define SRCADDR_OFFSET 10
#define BSSID_OFFSET 16
#define SEQNUM_OFFSET 22
#define TOTAL_LINES (sizeof(ssids) / sizeof(char *))

static void spam_task(void *pvParameter) {
  uint8_t line = 0;

  uint16_t seqnum[TOTAL_LINES] = {0};

  for (;;) {
    vTaskDelay(100 / TOTAL_LINES / portTICK_PERIOD_MS);

    uint8_t beacon[200];
    memcpy(beacon, beacon_raw, BEACON_SSID_OFFSET - 1);
    beacon[BEACON_SSID_OFFSET - 1] = strlen(ssids[line]);
    memcpy(&beacon[BEACON_SSID_OFFSET], ssids[line], strlen(ssids[line]));
    memcpy(&beacon[BEACON_SSID_OFFSET + strlen(ssids[line])],
           &beacon_raw[BEACON_SSID_OFFSET],
           sizeof(beacon_raw) - BEACON_SSID_OFFSET);

    beacon[SRCADDR_OFFSET + 5] = line;
    beacon[BSSID_OFFSET + 5] = line;

    beacon[SEQNUM_OFFSET] = (seqnum[line] & 0x0f) << 4;
    beacon[SEQNUM_OFFSET + 1] = (seqnum[line] & 0xff0) >> 4;
    seqnum[line]++;
    if (seqnum[line] > 0xfff)
      seqnum[line] = 0;

    esp_wifi_80211_tx(WIFI_IF_AP, beacon,
                      sizeof(beacon_raw) + strlen(ssids[line]), false);

    if (++line >= TOTAL_LINES)
      line = 0;
  }
}

static void ssid_spam_init() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_ap();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  wifi_config_t ap_config = {.ap = {.ssid = "esp32-beaconspam",
                                    .ssid_len = 0,
                                    .password = "dummypassword",
                                    .channel = 1,
                                    .authmode = WIFI_AUTH_WPA2_PSK,
                                    .ssid_hidden = 1,
                                    .max_connection = 4,
                                    .beacon_interval = 60000}};

  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

  xTaskCreate(&spam_task, "spam_task", 4096, NULL, 5, NULL);
}

static void ssid_spam_input_cb(uint8_t button_name, uint8_t button_event) {
  if (button_event != BUTTON_PRESS_DOWN) {
    return;
  }
  switch (button_name) {
  case BUTTON_BACK:
    menus_module_restart();
    break;
  case BUTTON_RIGHT:
    break;
  case BUTTON_MIDDLE:
    break;
  case BUTTON_LEFT:
    break;
  default:
    break;
  }
}

void ssid_spam_begin() {
  ssid_spam_screens_running();
  animations_task_run(ssid_spam_animation, 200, NULL);
  ssid_spam_init();
  menus_module_set_app_state(true, ssid_spam_input_cb);
}
