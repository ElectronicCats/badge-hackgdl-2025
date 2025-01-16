#pragma once
#include <stdio.h>

typedef void (*neopixel_event)(void);

void neopixel_scanning_event();
void neopixel_events_stop_event();
void neopixel_events_run_event(neopixel_event event);