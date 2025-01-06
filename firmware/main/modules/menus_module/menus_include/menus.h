#ifndef _MENUS_H_
#define _MENUS_H_

#include <stdbool.h>
#include <stdio.h>

typedef enum {
  MENU_MAIN = 0,
  MENU_APPLICATIONS,
  MENU_TAMAGOTCHI,
} menu_idx_t;

typedef struct {
  menu_idx_t menu_idx;
  menu_idx_t parent_idx;
  const char *display_name;
  const char *entry_cmd;
  uint8_t last_selected_submenu;
  void *on_enter_cb;
  void *on_exit_cb;
  bool is_visible;
} menu_t;

typedef struct {
  menu_idx_t current_menu;
  menu_idx_t parent_menu_idx;
  uint8_t selected_submenu;
  uint8_t submenus_count;
  uint8_t menus_count;
  uint8_t **submenus_idx;
  bool input_lock;
} menus_manager_t;

extern menu_t menus[];

uint8_t get_menus_count();

#endif