#ifndef _DEAUTH_MODULE_H_
#define _DEAUTH_MODULE_H_

#include <stdio.h>

void deauth_module_begin();

void deauth_module_scan();
void deauth_module_select_ap();
void deauth_module_select_attack();
void deauth_module_run();
void deauth_module_exit();

void deauth_module_set_ap(uint8_t selection);
void deauth_module_set_attack(uint8_t selection);

void deauth_module_run_exit();

void deauth_module_set_captive_portal_settings(uint8_t selection);

#endif // _DEAUTH_MODULE_H_