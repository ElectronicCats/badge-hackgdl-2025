#ifndef _TAMA_FRIENDS_H_
#define _TAMA_FRIENDS_H_

#include <stdio.h>

#define MAX_FRIENDS 300
#define STR_SIZE 13
#define MAC_SIZE 6

#define FRIENDS_STR_MEM "fstr"
#define FRIENDS_STR_SIZE MAX_FRIENDS *STR_SIZE
#define FRIENDS_MAC_MEM "fmac"
#define FRIENDS_MAC_SIZE MAX_FRIENDS *MAC_SIZE

#define MAGIC_FRIEND_NUM 0x67
#define MAGIC_FRIEND_MEM "mfriend"

typedef struct {
  uint16_t friends_count;
  char friends_str[MAX_FRIENDS][STR_SIZE];
  uint8_t friends_mac[MAX_FRIENDS][MAC_SIZE];
} tama_friends_ctx_t;

void tama_friends_begin();
uint16_t tama_friends_get_count();
void tama_friends_add(const char *friend_name, const uint8_t *friend_mac);
tama_friends_ctx_t *tama_friends_get_ctx();
void tama_friends_show_list();

#endif