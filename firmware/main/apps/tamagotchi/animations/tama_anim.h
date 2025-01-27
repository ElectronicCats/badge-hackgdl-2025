#ifndef _TAMA_ANIM_H_
#define _TAMA_ANIM_H_

#include <stdio.h>

void tama_anim_splah_screen();
void tama_anim_scan();
void tama_anim_new_friend(void (*new_friend_exit)());
void tama_anim_friend(char *name, void (*friend_exit)());
void tama_anim_stop_any();

#endif // _TAMA_ANIM_H_