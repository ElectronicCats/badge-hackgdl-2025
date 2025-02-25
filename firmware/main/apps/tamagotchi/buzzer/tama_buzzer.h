#ifndef _TAMA_BUZZER_H_
#define _TAMA_BUZZER_H_

#include <stdbool.h>
#include <stdio.h>

void hal_set_frequency(uint32_t freq);
void hal_play_frequency(bool en);
void tama_buzzer_begin();

#endif