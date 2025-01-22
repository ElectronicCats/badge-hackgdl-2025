#ifndef _TAMA_FRIENDS_H_
#define _TAMA_FRIENDS_H_

#include <stdio.h>

void tama_friends_begin();
uint16_t tama_friends_get_count();
void tama_friends_add(const char *friend_name, const uint8_t *friend_mac);

#endif