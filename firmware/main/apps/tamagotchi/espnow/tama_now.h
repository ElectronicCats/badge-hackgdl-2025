#ifndef _TAMA_NOW_H_
#define _TAMA_NOW_H_

#include <stdio.h>

#define TAMA_NOW_MAGIC 0xD4

typedef enum { ADV_REQ_CMD = 0x67, ADV_RES_CMD = 0xA6 } tama_now_cmds_e;

typedef struct {
  uint8_t magic;
  uint8_t cmd;
} tama_now_adv_req_cmd_t;

typedef struct {
  uint8_t magic;
  uint8_t cmd;
  char nickname[13];
} tama_now_adv_res_cmd_t;

void tama_now_begin();

#endif // _TAMA_NOW_H_