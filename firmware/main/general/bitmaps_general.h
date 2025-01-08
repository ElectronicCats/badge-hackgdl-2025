#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef BITMAPS_GENERAL_H
#define BITMAPS_GENERAL_H

typedef enum {
  MININO_LETTERS,
  MININO_FACE,
  MININO_PWNLABS,
  MININO_ELECTRONICCATS,
  MININO_FACE_MINI,
  MININO_BABY_DRAGON,
  MININO_COUNT
} epd_bitmap_type_t;

// 'FACE', 8x8px
const unsigned char minino_face[] = {0x42, 0xe7, 0xff, 0xff,
                                     0x99, 0x99, 0x7e, 0x3c};

typedef struct {
  const epd_bitmap_type_t idx;
  const char *name;
  const unsigned char *bitmap;
  uint8_t width;
  uint8_t height;
} epd_bitmap_t;

// 'minino-logo', 64x32px
const unsigned char epd_bitmap_minino_text_logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7f, 0x09, 0xc7, 0xe2, 0x78, 0xf8, 0x9f, 0xf8, 0x3f, 0x0c, 0xe3, 0xf3,
    0x3c, 0x7c, 0xcf, 0xfc, 0x1f, 0x1c, 0xf9, 0xf3, 0x3e, 0x3c, 0xc7, 0xfe,
    0x1f, 0x3e, 0x79, 0xf3, 0x9e, 0x3c, 0xe3, 0x9e, 0x0f, 0x7e, 0x79, 0xf7,
    0x9e, 0x3d, 0xe3, 0x0e, 0x1f, 0x7c, 0xf9, 0xff, 0xbe, 0x7f, 0xe7, 0x1e,
    0x1f, 0xfc, 0xf3, 0xff, 0x3c, 0x7f, 0xe7, 0x1c, 0x3f, 0xfc, 0xf3, 0xff,
    0x3c, 0xff, 0xce, 0x1c, 0x3f, 0xfc, 0xf3, 0xbf, 0x3c, 0xef, 0xce, 0x3c,
    0x3f, 0xb9, 0xe7, 0xbe, 0x78, 0xef, 0x8f, 0x78, 0x7b, 0xb9, 0xe7, 0x9e,
    0x79, 0xe7, 0x8f, 0xf8, 0x7b, 0x39, 0xef, 0x1e, 0x79, 0xc7, 0x8f, 0xf0,
    0x72, 0x70, 0xcf, 0x1c, 0x31, 0xc7, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00};

// 'face-logo', 32x32px
static const unsigned char epd_bitmap_face_logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x30,
    0x0e, 0x00, 0x00, 0x70, 0x1f, 0x00, 0x00, 0xf8, 0x1f, 0x80, 0x01, 0xf8,
    0x3f, 0xe0, 0x07, 0xfc, 0x3f, 0xf0, 0x0f, 0xfc, 0x3f, 0xff, 0xff, 0xfc,
    0x3f, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe,
    0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe,
    0x7f, 0xff, 0xff, 0xfe, 0x7e, 0x1f, 0xf8, 0x7e, 0x7c, 0x0f, 0xf0, 0x3e,
    0x78, 0x07, 0xe0, 0x1e, 0x78, 0x07, 0xe0, 0x1e, 0x78, 0x07, 0xe0, 0x1e,
    0x3c, 0x0f, 0xf0, 0x3c, 0x3e, 0x1f, 0xf8, 0x7c, 0x1f, 0xff, 0xff, 0xf8,
    0x0f, 0xfe, 0x7f, 0xf0, 0x07, 0xfc, 0x3f, 0xe0, 0x03, 0xff, 0xff, 0xc0,
    0x00, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x0f, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 'PWNlab-logo', 64x11px
const unsigned char epd_bitmap_pwn_02[] = {
    0xfe, 0x08, 0x20, 0x83, 0x04, 0x40, 0x00, 0x80, 0xff, 0x1c, 0x71,
    0xc7, 0x8e, 0x40, 0x00, 0x80, 0xeb, 0x9c, 0x71, 0xcf, 0xee, 0x40,
    0x00, 0x80, 0xc1, 0xdc, 0x71, 0xce, 0xee, 0x40, 0x78, 0xbc, 0xeb,
    0xdc, 0x71, 0xce, 0xee, 0x40, 0xcc, 0xe6, 0xc1, 0x9c, 0x71, 0xce,
    0xee, 0x41, 0x86, 0xc3, 0xeb, 0x1c, 0x71, 0xce, 0xee, 0x41, 0x02,
    0x81, 0xfe, 0x1e, 0xfb, 0xce, 0xee, 0x41, 0x02, 0x81, 0xf0, 0x1f,
    0xff, 0xce, 0xfe, 0x41, 0x86, 0xc3, 0xf0, 0x0f, 0xff, 0x8e, 0x7c,
    0x20, 0xce, 0x66, 0x60, 0x07, 0xcf, 0x04, 0x18, 0x1e, 0x7a, 0x3c};

// 'ec-face-logo', 48x30px
const unsigned char epd_bitmap_electroniccats[] = {
    0x00, 0x30, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x3e, 0x00,
    0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
    0x00, 0xff, 0x80, 0x01, 0xff, 0x00, 0x00, 0xff, 0xc7, 0xe3, 0xff, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x00,
    0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfe, 0x00,
    0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00,
    0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x30, 0x7f, 0xff, 0xff, 0xfc, 0x0c,
    0x78, 0x7c, 0x3f, 0xfc, 0x3e, 0x1e, 0x4c, 0xfc, 0x1f, 0xf8, 0x3e, 0x32,
    0x7f, 0xfc, 0x0f, 0xf0, 0x3f, 0xfe, 0x30, 0xfe, 0x0f, 0xf0, 0x7e, 0x0c,
    0x60, 0xfe, 0x07, 0xe0, 0x7e, 0x06, 0xf0, 0xff, 0x87, 0xe1, 0xfe, 0x0f,
    0x9f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xf0, 0x3f, 0xff, 0xff, 0xfe, 0x0f,
    0x60, 0x1f, 0xff, 0xff, 0xfc, 0x06, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x00,
    0x30, 0xff, 0xff, 0xff, 0xff, 0x0c, 0x7f, 0x83, 0xff, 0xff, 0xc1, 0xfe,
    0x4c, 0x00, 0xff, 0xff, 0x00, 0x32, 0x78, 0x00, 0x3f, 0xfc, 0x00, 0x1e,
    0x30, 0x00, 0x07, 0xe0, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 'baby-dragon', 32x32px
const unsigned char epd_bitmap_baby_dragon_ss[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x80, 0x00,
    0x00, 0x27, 0x40, 0x00, 0x00, 0x10, 0xf8, 0x00, 0x00, 0x08, 0x04, 0x00,
    0x00, 0x70, 0x02, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x20, 0x25, 0x00,
    0x00, 0x10, 0x25, 0x00, 0x00, 0x60, 0x24, 0x80, 0x00, 0x20, 0x00, 0x80,
    0x00, 0x18, 0x01, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x01, 0x10, 0x00,
    0x00, 0x01, 0x10, 0x00, 0x00, 0x02, 0x08, 0x00, 0x01, 0x02, 0x06, 0x00,
    0x02, 0x84, 0x05, 0x00, 0x02, 0x88, 0x02, 0x80, 0x04, 0x90, 0x02, 0x40,
    0x04, 0x51, 0x01, 0x40, 0x04, 0x52, 0x01, 0x80, 0x02, 0x2c, 0x01, 0x00,
    0x02, 0x14, 0x01, 0x00, 0x01, 0x08, 0x02, 0x00, 0x00, 0x88, 0x02, 0x00,
    0x00, 0x68, 0x72, 0x00, 0x00, 0x10, 0x89, 0x80, 0x00, 0x11, 0x08, 0x40,
    0x00, 0x0e, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00};

// 'arrow_left', 16x16px
static const unsigned char epd_bitmap_arrow_left[] = {
    0x01, 0x80, 0x03, 0x80, 0x07, 0x80, 0x0f, 0x80, 0x1f, 0x80, 0x3f,
    0x80, 0x7f, 0x80, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x80, 0x3f, 0x80,
    0x1f, 0x80, 0x0f, 0x80, 0x07, 0x80, 0x03, 0x80, 0x01, 0x80};
// 'arrow_right', 16x16px
static const unsigned char epd_bitmap_arrow_right[] = {
    0x01, 0x80, 0x01, 0xc0, 0x01, 0xe0, 0x01, 0xf0, 0x01, 0xf8, 0x01,
    0xfc, 0x01, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x01, 0xfe, 0x01, 0xfc,
    0x01, 0xf8, 0x01, 0xf0, 0x01, 0xe0, 0x01, 0xc0, 0x01, 0x80};
// 'arrow_up', 16x16px
static const unsigned char
    epd_bitmap_arrow_up[] = {0x01, 0x80, 0x03, 0xc0, 0x07, 0xe0, 0x0f,
                             0xf0, 0x1f, 0xf8, 0x3f, 0xfc, 0x7f, 0xfe,
                             0xff, 0xff, 0xff, 0xff, 0x01, 0x80, 0x01,
                             0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
                             0x00, 0x00, 0x00, 0x00}; // 'arrow_down', 16x16px
static const unsigned char epd_bitmap_arrow_down[] = {
    0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
    0x80, 0x01, 0x80, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0x3f, 0xfc,
    0x1f, 0xf8, 0x0f, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x80};

static const unsigned char *arrows_bitmap_arr[] = {
    epd_bitmap_arrow_left, epd_bitmap_arrow_right, epd_bitmap_arrow_up,
    epd_bitmap_arrow_down};
// simple_left_arrow 8x8
static const unsigned char simple_left_arrow_bmp[] = {0x08, 0x10, 0x20, 0x40,
                                                      0x40, 0x20, 0x10, 0x08};
// simple_right_arrow 8x8
static const unsigned char simple_right_arrow_bmp[] = {0x10, 0x08, 0x04, 0x02,
                                                       0x02, 0x04, 0x08, 0x10};
// simple_up_arrow 8x8
static const unsigned char simple_up_arrow_bmp[] = {0x00, 0x18, 0x24, 0x42,
                                                    0x81, 0x00, 0x00, 0x00};
// simple_down_arrow 8x8
static const unsigned char simple_down_arrow_bmp[] = {0x00, 0x00, 0x00, 0x81,
                                                      0x42, 0x24, 0x18, 0x00};

unsigned char *epd_startup_logo[] = {epd_bitmap_minino_text_logo,
                                     epd_bitmap_face_logo};

const epd_bitmap_t minino_face_bitmap = {
    .idx = MININO_FACE_MINI,
    .name = "Mini Face",
    .bitmap = minino_face,
    .width = 8,
    .height = 8,
};

const epd_bitmap_t minino_letters_bitmap = {
    .idx = MININO_LETTERS,
    .name = "Letters",
    .bitmap = epd_bitmap_minino_text_logo,
    .width = 64,
    .height = 32,
};

const epd_bitmap_t minino_face_logo = {
    .idx = MININO_FACE,
    .name = "Face",
    .bitmap = epd_bitmap_face_logo,
    .width = 32,
    .height = 32,
};

const epd_bitmap_t minino_pwnlabs_logo = {
    .idx = MININO_PWNLABS,
    .name = "PwnLabs",
    .bitmap = epd_bitmap_pwn_02,
    .width = 64,
    .height = 11,
};

const epd_bitmap_t minino_electroniccats_logo = {
    .idx = MININO_ELECTRONICCATS,
    .name = "EC",
    .bitmap = epd_bitmap_electroniccats,
    .width = 48,
    .height = 30,
};

const epd_bitmap_t minino_baby_dragon_bitmap = {
    .idx = MININO_BABY_DRAGON,
    .name = "Baby Dragon",
    .bitmap = epd_bitmap_baby_dragon_ss,
    .width = 32,
    .height = 32,
};

// 'pixil-frame-0', 32x32px
const unsigned char michi_punch_1[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00,
    0x01, 0x8c, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00,
    0x03, 0x76, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x03, 0xde, 0x00, 0x00,
    0x01, 0xac, 0x00, 0x00, 0x30, 0xf8, 0x00, 0x00, 0x3d, 0xf8, 0x00, 0x00,
    0x3f, 0xf8, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00,
    0x03, 0xfe, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00,
    0x01, 0xfe, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00,
    0x01, 0xff, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x0d, 0xfe, 0x00, 0x00,
    0x01, 0xfe, 0x00, 0x00, 0x01, 0xee, 0x00, 0x00, 0x01, 0xc6, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 'pixil-frame-0', 32x32px
const unsigned char michi_punch_2[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00,
    0x01, 0x8c, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00,
    0x03, 0x76, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x03, 0xde, 0x00, 0x00,
    0x01, 0xac, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00,
    0x00, 0xf8, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00,
    0x00, 0x0e, 0x00, 0x00, 0x01, 0x86, 0x00, 0x00, 0x01, 0xc6, 0x00, 0x00,
    0x01, 0xfe, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00,
    0x01, 0xff, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x0d, 0xfe, 0x00, 0x00,
    0x01, 0xfe, 0x00, 0x00, 0x01, 0xee, 0x00, 0x00, 0x01, 0xc6, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char *punch_animation[] = {
    michi_punch_1,
    michi_punch_2,
};

const epd_bitmap_t *screen_savers[] = {
#if CONFIG_SCREEN_SAVER_LETTERS
    &minino_letters_bitmap,
#endif
#if CONFIG_SCREEN_SAVER_FACE
    &minino_face_logo,
#endif
#if CONFIG_SCREEN_SAVER_PWNLABS
    &minino_pwnlabs_logo,
#endif
#if CONFIG_SCREEN_SAVER_EC
    &minino_electroniccats_logo,
#endif
#if CONFIG_SCREEN_SAVER_MINI_FACE
    &minino_face_bitmap,
#endif
#if CONFIG_SCREEN_SAVER_BABY_DRAGON
    &minino_baby_dragon_bitmap,
#endif
};

char *epd_bitmaps_list[] = {"Letters",   "Face",        "PwnLabs", "EC",
                            "Mini face", "Baby Dragon", NULL};
#endif // BITMAPS_GENERAL_H