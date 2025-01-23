#ifndef _CHIPTUNE_T_H
#define _CHIPTUNE_T_H

#include <stdio.h>

typedef struct {
  uint16_t *freq;
  uint32_t *duration_ms;
  size_t len;
} chiptune_t;

#endif // _CHIPTUNE_T_H