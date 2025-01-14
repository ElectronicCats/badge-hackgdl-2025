#ifndef _DEAUTH_SCENES_H_
#define _DEAUTH_SCENES_H_

#include <stdio.h>

void deauth_scenes_main_menu();
void deauth_scenes_ap_selection(char **ap_names, uint8_t ap_count,
                                uint8_t selected);
void deauth_scenes_attack_selection(uint16_t selected);

#endif // _DEAUTH_SCENES_H_