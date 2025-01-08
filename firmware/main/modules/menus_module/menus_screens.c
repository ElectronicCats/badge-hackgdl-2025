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
  menu_t *parent = &menus[menu->parent_idx];
  char *parent_name = parent->display_name;
  char *display_name = menu->display_name;

  if (menu->icon != NULL) {
    menu_icon_t *icon = menu->icon;
    oled_screen_display_bitmap(icon->icon, 63 - icon->width / 2, 12,
                               icon->width, icon->height, OLED_DISPLAY_NORMAL);
  }

  char str[16];
  snprintf(str, sizeof(str), "<%d/%d>", ctx->selected_submenu + 1,
           ctx->submenus_count);
  oled_screen_display_text_center(str, 6, OLED_DISPLAY_NORMAL);
  oled_screen_draw_box(0, 0, 128, 8, OLED_DISPLAY_NORMAL);
  oled_screen_display_text_center(parent_name, 0, OLED_DISPLAY_INVERT);
  oled_screen_display_text_center(display_name, 7, OLED_DISPLAY_NORMAL);
  oled_screen_display_show();
}
