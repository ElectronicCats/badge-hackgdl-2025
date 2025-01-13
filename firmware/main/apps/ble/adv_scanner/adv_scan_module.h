#include <stdint.h>
#ifndef ADV_SCAN_MODULE_H
#define ADV_SCAN_MODULE_H

#define SCAN_TYPE_MEM "adv_type"
#define SCAN_FILTER_MEM "adv_filter"

void adv_scanner_module_begin();
void adv_scanner_module_start();
void adv_scanner_module_set_filter(uint8_t selection);
void adv_scanner_module_set_scan_type(uint8_t selection);
#endif // ADV_SCAN_MODULE_H
