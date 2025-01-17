#ifndef _TAMA_DISPLAY_H_
#define _TAMA_DISPLAY_H_

#include <stdbool.h>
#include <stdio.h>

#define TAMA_DISPLAY_MAX_NICKNAME_LEN 13

void hal_set_lcd_icon(uint8_t icon, bool val);

void hal_set_lcd_matrix(uint8_t x, uint8_t y, bool val);

void tama_display_update_screen();

void tama_display_begin();

void tama_display_set_nickname(char *new_nickname);

#endif // _TAMA_DISPLAY_H_