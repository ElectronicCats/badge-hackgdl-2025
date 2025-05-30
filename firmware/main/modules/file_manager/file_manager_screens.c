#include "file_manager_screens.h"
#include "oled_screen.h"

#ifdef CONFIG_RESOLUTION_128X64
#define MAX_ITEMS_NUM 8
#define ITEMOFFSET 1
#else // CONFIG_RESOLUTION_128X32
#define MAX_ITEMS_NUM 4
#define ITEMOFFSET 1
#endif

static void update_list(file_manager_context_t *ctx) {
  static uint8_t items_offset = 0;
  items_offset = MAX(ctx->selected_item - MAX_ITEMS_NUM + 2, items_offset);
  items_offset =
      MIN(MAX(ctx->items_count - MAX_ITEMS_NUM + 1, 0), items_offset);
  items_offset = MIN(ctx->selected_item, items_offset);
  oled_screen_clear_buffer();
  oled_screen_display_text("< Back", 0, 0, OLED_DISPLAY_NORMAL);
  if (ctx->items_count == 0) {
    oled_screen_display_text("  Empty folder  ", 0, MAX_ITEMS_NUM / 2 - 1,
                             OLED_DISPLAY_NORMAL);
    oled_screen_display_text("Nothing to show", 0, MAX_ITEMS_NUM / 2,
                             OLED_DISPLAY_NORMAL);
  } else {
    for (uint8_t i = 0; i < (MIN(ctx->items_count, MAX_ITEMS_NUM - 1)); i++) {
      char *str = (char *)malloc(30);
      sprintf(str, "%s%s", ctx->file_items_arr[i + items_offset]->name,
              ctx->file_items_arr[i + items_offset]->is_dir ? ">" : "");
      oled_screen_display_text(str, 0, i + 1,
                               ctx->selected_item == i + items_offset);
      free(str);
    }
  }
  oled_screen_display_show();
}

static void show_fatal_error(char *error_tag) {}
void file_manager_screens_event_handler(file_manager_events_t event,
                                        void *context) {
  switch (event) {
  case FILE_MANAGER_UPDATE_LIST_EV:
    update_list(context);
    break;
  case FILE_MANAGER_SHOW_FATAL_ERR_EV:
    show_fatal_error(context);
    break;
  default:
    break;
  }
}