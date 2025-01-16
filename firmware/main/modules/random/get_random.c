#include "get_random.h"

#include "esp_random.h"

uint8_t get_random_uint8() { return (uint8_t)(esp_random() & 0xFF); }
