#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "cpu.h"

#include "preferences.h"
#include "tama_state.h"
#define EEPROM_MAGIC_NUMBER 0x12

#define MAGIC_NUMBER_MEM "magic"
#define CPU_STATE_MEM "cpu_state"
#define CPU_MEMORY_MEM "cpu_memory"

void tama_state_begin() { preferences_begin(); }

void tama_state_load(cpu_state_t *cpuState) {
  if (preferences_get_uchar(MAGIC_NUMBER_MEM, 0) != EEPROM_MAGIC_NUMBER) {
    printf("Invalid magic number\n");
    return;
  }
  cpu_get_state(cpuState);
  u4_t *memTemp = cpuState->memory;
  preferences_get_bytes(CPU_STATE_MEM, cpuState, sizeof(cpu_state_t));
  preferences_get_bytes(CPU_MEMORY_MEM, memTemp, MEMORY_SIZE);
  cpu_set_state(cpuState);
  printf("########    STATE LOADED    ##########\n");
}

void tama_state_save(cpu_state_t *cpuState) {
  cpu_get_state(cpuState);
  preferences_put_uchar(MAGIC_NUMBER_MEM, EEPROM_MAGIC_NUMBER);
  preferences_put_bytes(CPU_STATE_MEM, cpuState, sizeof(cpu_state_t));
  preferences_put_bytes(CPU_MEMORY_MEM, cpuState->memory, MEMORY_SIZE);
  printf("########    STATE SAVED    ##########\n");
}
