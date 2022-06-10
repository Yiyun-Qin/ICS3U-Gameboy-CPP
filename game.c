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
    const spriteY = 8;

    // font
    font_t min_font;

    // initial and load font library
    font_init();
    min_font = font_load(font_min);  // 36 tiles
    font_set(min_font);

    // fonts, width and height
    set_win_tiles(0, 0, 5, 1, windowmap);
    // window layer is above other layers, so it overlap other layers
    move_win(7, 120);

    // background
    // start at 37 to prevent overlap with font
    set_bkg_data(37, 7, backgroundtiles);
    // start at 0 in 0, x, y, go to backgroundmap to know how to use
    set_bkg_tiles(0, 0, 40, 18, backgroundmap);

    // load sprite
    // start from nth tiles, to load how many elements, file name
    set_sprite_data(0, 1, TileData);
    // number of sprite, the nth tile(smile)
    set_sprite_tile(0, 0);
    // move the nth sprite to (88, 78)
    move_sprite(0, x, y);

    // sound
    // these registers need to be in this specific order
    NR52_REG = 0x80;  // is 1000 0000 in binary and tells
                      // Gameboy to open the whole sound system
    NR50_REG = 0x77;  // set the volume for both left and right
                      // channels to the max 0x77
    NR51_REG = 0xFF;  // is 1111 1111 in binary and select which
                      // channels we want to use
    // now they are all open

    // show the map
    SHOW_BKG;
    SHOW_WIN;
    DISPLAY_ON;
    SHOW_SPRITES;

    while (1) {
        // scroll background
        scroll_bkg(1, 0);
        delay(100);

        // sound
        UBYTE jotpad_state = joypad();

        if (jotpad_state) {
            // channel 1 register 0, frequency sweep settings
            // sweep: increase or decrease in frequency
            // 7    unused
            // 6-4(001)  sweep time(update rate)(if 0, sweeping is off)
            // 3    sweep direction(1 is decrease, 0 is increase)
            // 2-0  sweep RtShift amount(if 0, sweeping is off)
            // 0x16 = 0001 0110, from left to right is 7-0
            // sweep time 1, direction: increase,
            // shift amount per step 110(6 decimals)
            NR10_REG = 0x51;

            NR11_REG = 0x40;

            NR12_REG = 0x73;

            NR13_REG = 0x00;

            NR14_REG = 0xC3;
        }

        // sprite moving
        switch (joypad()) {
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
