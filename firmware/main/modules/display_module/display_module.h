#ifndef _DISPLAY_MODULE_H_
#define _DISPLAY_MODULE_H_

#include <stdio.h>

#include "u8g2_esp32_hal.h"
#include <u8g2.h>

u8g2_t *display_module_get_u8g2();
void display_module_begin();

#endif