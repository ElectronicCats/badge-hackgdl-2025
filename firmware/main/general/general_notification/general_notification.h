#ifndef _GENERAL_NOTIFICATION_H_
#define _GENERAL_NOTIFICATION_H_

#include <stdio.h>

typedef struct {
  char *head;
  char *body;
  void (*on_start_cb)();
  void (*on_end_cb)();
  uint32_t duration_ms;
} general_notification_ctx_t;

void general_notification(general_notification_ctx_t ctx);

#endif // _GENERAL_NOTIFICATION_H_