#include <stdint.h>
#ifndef HID_MODULE_H
#define HID_MODULE_H
void hid_module_begin();

void hid_module_name();
void hid_module_mac();
void hid_module_start();
void hid_module_exit();

void hid_module_control_vol_up();
void hid_module_control_vol_down();
void hid_module_control_play_pause();
#endif // HID_MODULE_H
