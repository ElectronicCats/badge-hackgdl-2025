#pragma once

#include "general_screens.h"
#include <stdbool.h>
#include <stdio.h>

typedef void (*submenu_selection_handler_t)(uint8_t);

typedef struct {
  char *title;
  uint8_t options_count;
  char **options;
  uint8_t selected_option;
  submenu_selection_handler_t *select_cb;
  void *exit_cb;
} general_submenu_menu_t;

void general_submenu(general_submenu_menu_t radio_selection_menu);