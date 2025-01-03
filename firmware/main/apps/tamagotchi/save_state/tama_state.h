#ifndef _TAMA_STATE_H
#define _TAMA_STATE_H

#include <stdbool.h>

void tama_state_begin();
void tama_state_load(cpu_state_t *cpuState);
void tama_state_save(cpu_state_t *cpuState);

#endif