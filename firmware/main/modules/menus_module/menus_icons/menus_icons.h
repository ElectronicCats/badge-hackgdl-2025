#pragma once

#include <stdio.h>

#include "menus_bitmaps.h"

typedef struct {
  const unsigned char *icon;
  uint8_t width;
  uint8_t height;
} menu_icon_t;

extern menu_icon_t menu_icon_wifi;
extern menu_icon_t menu_icon_bt;
extern menu_icon_t menu_icon_tama;
extern menu_icon_t menu_icon_about;

extern menu_icon_t menu_icon_analyzer;
extern menu_icon_t menu_icon_deauth;
extern menu_icon_t menu_icon_deauth_scan;
extern menu_icon_t menu_icon_dos;
extern menu_icon_t menu_icon_ssid_spam;

extern menu_icon_t menu_icon_trakers;
extern menu_icon_t menu_icon_spam;
extern menu_icon_t menu_icon_hid;
extern menu_icon_t menu_icon_adv_scanner;