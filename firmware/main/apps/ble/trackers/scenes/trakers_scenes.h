#pragma once

#include <stdio.h>

void trakers_scenes_main_menu();
void set_scan_text(char *text);
void trakers_scenes_list_menu(char **trakers, uint8_t count);
void trakers_scenes_no_devices_found();
void trakers_scenes_searching();
void trakers_scenes_device_found();
void traker_scenes_traker_details(char **traker_details);