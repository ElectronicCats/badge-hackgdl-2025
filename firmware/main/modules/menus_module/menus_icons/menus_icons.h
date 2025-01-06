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