#include "tama_buzzer.h"

#include "buzzer.h"
#include <driver/gpio.h>

#define FREQ_DIVIDER 1

static uint16_t current_freq = 0;
void hal_set_frequency(uint32_t freq) { current_freq = freq / FREQ_DIVIDER; }

void hal_play_frequency(bool en) {
  if (en) {
    buzzer_set_freq(current_freq);
    buzzer_play();
  } else {
    buzzer_stop();
  }
}

void tama_buzzer_begin() {
  // buzzer_begin(PIN_BUZZER);
}