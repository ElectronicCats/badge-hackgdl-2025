#ifndef _HID_SCENES_H_
#define _HID_SCENES_H_

#include <stdio.h>

void hid_scenes_main_menu();
void hid_scenes_control_menu(uint8_t selected);
void hid_scenes_notification(char *head, char *body, uint32_t duration_ms);
void hid_scenes_notification_handler(char *head, char *body);

#endif // _HID_SCENES_H_