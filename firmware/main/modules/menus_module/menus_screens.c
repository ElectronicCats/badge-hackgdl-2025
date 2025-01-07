#include "menus_screens.h"

#include "menus.h"
#include "menus_icons.h"
#include "oled_screen.h"
#include <string.h>

#define MAX_MENUS_ON_SCREEN 8

void menus_screens_display_menus(menus_manager_t *ctx) {
  if (!ctx->submenus_count) {
    return;
  }
  oled_screen_clear_buffer();

  menu_t *menu = &menus[*ctx->submenus_idx[ctx->selected_submenu]];
  char *display_name = menu->display_name;

  if (menu->icon != NULL) {
    menu_icon_t *icon = menu->icon;
    oled_screen_display_bitmap(icon->icon, 64 - icon->width / 2, 8, icon->width,
                               icon->height, OLED_DISPLAY_NORMAL);
  }

  oled_screen_display_text_center(display_name, 6, OLED_DISPLAY_NORMAL);
  oled_screen_display_show();
}
