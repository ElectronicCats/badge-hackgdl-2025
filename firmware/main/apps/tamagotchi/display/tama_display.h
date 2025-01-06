#ifndef _TAMA_DISPLAY_H_
#define _TAMA_DISPLAY_H_

#include <stdbool.h>
#include <stdio.h>

void hal_set_lcd_icon(uint8_t icon, bool val);

void hal_set_lcd_matrix(uint8_t x, uint8_t y, bool val);

void tama_display_update_screen();

void tama_display_begin();

#endif // _TAMA_DISPLAY_H_