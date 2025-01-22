#include "tama_friends.h"

#include <string.h>

#include "preferences.h"

#define MAX_FRIENDS 300
#define STR_SIZE 13
#define MAC_SIZE 6

#define FRIENDS_STR_MEM "fstr"
#define FRIENDS_STR_SIZE MAX_FRIENDS *STR_SIZE
#define FRIENDS_MAC_MEM "fmac"
#define FRIENDS_MAC_SIZE MAX_FRIENDS *MAC_SIZE

#define MAGIC_FRIEND_NUM 0x67
#define MAGIC_FRIEND_MEM "mfriend"

static uint16_t friends_count = 0;
static char friends_str[MAX_FRIENDS][STR_SIZE] = {0};
static uint8_t friends_mac[MAX_FRIENDS][MAC_SIZE] = {0};

static bool friend_exists(const uint8_t *mac) {
  for (int i = 0; i < friends_count; i++) {
    if (!memcmp(friends_mac[i], mac, MAC_SIZE)) {
      return true;
    }
  }
  return false;
}

uint16_t tama_friends_get_count() {
  uint16_t count = 0;

  for (uint8_t i = 0; i < MAX_FRIENDS; i++) {
    if (strlen(friends_str[i]) > 0) {
      count++;
    } else {
      break;
    }
  }

  return count;
}

void tama_friends_begin() {
  // preferences_begin();
  if (preferences_get_uchar(MAGIC_FRIEND_MEM, 0) != MAGIC_FRIEND_NUM) {
    return;
  }
  preferences_get_bytes(FRIENDS_STR_MEM, friends_str, FRIENDS_MAC_SIZE);
  preferences_get_bytes(FRIENDS_MAC_MEM, friends_mac, FRIENDS_STR_SIZE);

  friends_count = tama_friends_get_count();
  // printf("FRIENDS: %d\n", friends_count);
}

void tama_friends_add(const char *friend_name, const uint8_t *friend_mac) {
  if (friends_count >= MAX_FRIENDS) {
    return;
  }
  if (friend_exists(friend_mac)) {
    strncpy(friends_str[friends_count], friend_name, STR_SIZE);
    friends_str[friends_count][STR_SIZE - 1] = '\0';
    return;
  }

  strncpy(friends_str[friends_count], friend_name, STR_SIZE);
  friends_str[friends_count][STR_SIZE - 1] = '\0';

  memcpy(friends_mac[friends_count], friend_mac, MAC_SIZE);

  friends_count++;

  preferences_put_uchar(MAGIC_FRIEND_MEM, MAGIC_FRIEND_NUM);
  preferences_put_bytes(FRIENDS_STR_MEM, friends_str, FRIENDS_STR_SIZE);
  preferences_put_bytes(FRIENDS_MAC_MEM, friends_mac, FRIENDS_MAC_SIZE);
}
