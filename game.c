// Copyright (c) 2019 St. Mother Teresa HS All rights reserved.
//
// Created by: Yiyun Qin
// Created on: May 2022
// This program is the sprite program for the GameBoy

#include <stdio.h>
#include <gb/gb.h>
#include <gb/font.h>

#include <smiler.h>
#include <simpleBackground.c>
#include <simpleBackgroundMap.c>
#include <windowmap.c>


void main() {
    // sprite x and y coordinates
    int x = 88;
    int y = 78;
    const screenX = 160;
    const screenY = 144;
    const spriteX = 8;
    const spriteY = 8

    // font
    font_t min_font;

    // initial and load font library
    font_init();
    min_font = font_load(font_min);  // 36 tiles
    font_set(min_font);

    // fonts, width and height
    set_win_tiles(0, 0, 5, 1, windowmap)
    // window layer is above other layers, so it overlap other layers
    move_win(7, 120)

    // background
    // start at 37 to prevent overlap with font
    set_bkg_data(37, 7, backgroundtiles);
    // start at 0 in 0, x, y, go to backgroundmap to know how to use
    set_bkg_tiles(0, 0, 40, 18, backgroundmap);

    // show the map
    SHOW_BKG;
    SHOW_WIN;
    DISPLAY_ON;

    // load sprite
    // start from nth tiles, to load how many elements, file name
    set_sprite_data(0, 1, TileData);
    // number of sprite, the nth tile(smile)
    set_sprite_tile(0, 0);
    // move the nth sprite to (88, 78)
    move_sprite(0, x, y);
    SHOW_SPRITES;

    while (1) {
        // scroll background
        scroll_bkg(1, 0);
        delay(100);

        // sprite moving
        switch (joypad()) {;
            case J_LEFT:
            // index of sprite to access, x, y
                if (x > spriteX) {
                    scroll_sprite(0, -10, 0);
                    x = x - 10;
                }
            break;
            case J_RIGHT:
                if (x < screenX - spriteX) {
                    scroll_sprite(0, 10, 0);
                    x = x + 10;
                }
            break;
            case J_UP:
                if (y > 24) {
                    scroll_sprite(0, 0, -10);
                    y = y - 10;
                }
            break;
            case J_DOWN:
                if (y < screenY) {
                    scroll_sprite(0, 0, 10);
                    y = y + 10;
                }
            break;
        }
        delay(100);
    }
}
