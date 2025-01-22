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

typedef struct {
  uint16_t friends_count;
  char friends_str[MAX_FRIENDS][STR_SIZE];
  uint8_t friends_mac[MAX_FRIENDS][MAC_SIZE];
} tama_friends_ctx_t;

tama_friends_ctx_t *tama_friends_ctx = NULL;

static void dump_tama_friends_ctx(tama_friends_ctx_t *ctx) {
  printf("Friends Count: %u\n", ctx->friends_count);

  for (uint16_t i = 0; i < ctx->friends_count; i++) {
    printf("Friend %u:\n", i + 1);
    printf("  Name: %s\n", ctx->friends_str[i]);
    printf("  MAC: ");

    for (uint8_t j = 0; j < MAC_SIZE; j++) {
      printf("%02X", ctx->friends_mac[i][j]);
      if (j < MAC_SIZE - 1) {
        printf(":");
      }
    }
    printf("\n");
  }
}

static bool friend_exists(const uint8_t *mac) {
  for (int i = 0; i < tama_friends_ctx->friends_count; i++) {
    if (!memcmp(tama_friends_ctx->friends_mac[i], mac, MAC_SIZE)) {
      return true;
    }
  }
  return false;
}

uint16_t tama_friends_get_count() {
  uint16_t count = 0;

  for (uint8_t i = 0; i < MAX_FRIENDS; i++) {
    if (strlen(tama_friends_ctx->friends_str[i]) > 0) {
      count++;
    } else {
      break;
    }
  }

  return count;
}

static void tama_friends_alloc() {
  tama_friends_ctx = calloc(1, sizeof(tama_friends_ctx_t));
}

void tama_friends_begin() {
  // preferences_begin();

  tama_friends_alloc();

  if (preferences_get_uchar(MAGIC_FRIEND_MEM, 0) != MAGIC_FRIEND_NUM) {
    return;
  }

  preferences_get_bytes(FRIENDS_STR_MEM, tama_friends_ctx->friends_str,
                        FRIENDS_STR_SIZE);
  preferences_get_bytes(FRIENDS_MAC_MEM, tama_friends_ctx->friends_mac,
                        FRIENDS_MAC_SIZE);
  tama_friends_ctx->friends_count = tama_friends_get_count();
  dump_tama_friends_ctx(tama_friends_ctx);
}

void tama_friends_add(const char *friend_name, const uint8_t *friend_mac) {
  if (tama_friends_ctx->friends_count >= MAX_FRIENDS) {
    return;
  }
  if (friend_exists(friend_mac)) {
    strncpy(tama_friends_ctx->friends_str[tama_friends_ctx->friends_count],
            friend_name, STR_SIZE);
    preferences_put_bytes(FRIENDS_STR_MEM, tama_friends_ctx->friends_str,
                          FRIENDS_STR_SIZE);
    return;
  }

  strncpy(tama_friends_ctx->friends_str[tama_friends_ctx->friends_count],
          friend_name, STR_SIZE);
  memcpy(tama_friends_ctx->friends_mac[tama_friends_ctx->friends_count],
         friend_mac, MAC_SIZE);

  tama_friends_ctx->friends_count++;

  preferences_put_uchar(MAGIC_FRIEND_MEM, MAGIC_FRIEND_NUM);
  preferences_put_bytes(FRIENDS_STR_MEM, tama_friends_ctx->friends_str,
                        FRIENDS_STR_SIZE);
  preferences_put_bytes(FRIENDS_MAC_MEM, tama_friends_ctx->friends_mac,
                        FRIENDS_MAC_SIZE);
}
