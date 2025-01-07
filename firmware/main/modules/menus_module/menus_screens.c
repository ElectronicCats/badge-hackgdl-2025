#include "menus_screens.h"

#include <string.h>
// #include "oled_screen.h"
// #include "display_module.h"
#include "menus.h"
#include "menus_icons.h"

#define MAX_MENUS_ON_SCREEN 8

void menus_screens_display_menus(menus_manager_t *ctx) {
  if (!ctx->submenus_count) {
    return;
  }
  // u8g2_t *u8g2 = display_module_get_u8g2();
  // u8g2_SetFont(u8g2, u8g2_font_8x13_tf);
  // u8g2_ClearBuffer(u8g2);

  menu_t *menu = &menus[*ctx->submenus_idx[ctx->selected_submenu]];
  char *display_name = menu->display_name;

  // int16_t textWidth = u8g2_GetStrWidth(u8g2, display_name);
  // int16_t x = (128 - textWidth) / 2;

  if (menu->icon != NULL) {
    menu_icon_t *icon = menu->icon;
    // u8g2_DrawXBMP(u8g2, 64 - icon->width / 2, 8, icon->width, icon->height,
    //               icon->icon);
  }

  // u8g2_DrawStr(u8g2, x, 56, display_name);
  // u8g2_SendBuffer(u8g2);
  //   oled_screen_display_show();
}
