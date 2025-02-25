#include "tama_cmd.h"

#include "argtable3/argtable3.h"
#include "esp_console.h"
#include "esp_log.h"
#include <string.h>

#include "tama_display.h"

#define TAG "Tama_CMD"

static struct {
  struct arg_str *nickname;
  struct arg_end *end;
} nickname_args;

static int set_tama_nickname(int argc, char **argv) {
  int nerrors = arg_parse(argc, argv, (void **)&nickname_args);
  if (strlen(nickname_args.nickname->sval[0]) >
      TAMA_DISPLAY_MAX_NICKNAME_LEN - 1) {
    ESP_LOGE(TAG,
             "NickName lenght is longer than 12 chars, it will be truncated");
  }
  tama_display_set_nickname(nickname_args.nickname->sval[0]);
  return 0;
}

void tama_cmd_nickname_cmd() {
  nickname_args.nickname = arg_str1(NULL, NULL, "<NickName>", NULL);
  nickname_args.end = arg_end(1);

  esp_console_cmd_t tama_nickname_cmd = {
      .command = "tama_nickname",
      .help = "Set Tamagotchi NickName without spaces, Max lenght is 12 chars",
      .category = "Tamagotchi",
      .hint = NULL,
      .func = &set_tama_nickname,
      .argtable = &nickname_args};

  ESP_ERROR_CHECK(esp_console_cmd_register(&tama_nickname_cmd));
}