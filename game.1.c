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
    NR52_REG = 0x80;
    // is 1000 0000 in binary and tells Gameboy to open the whole sound system
    NR50_REG = 0x77;
    // set the volume for both left and right channels to the max 0x77
    NR51_REG = 0xFF;
    // is 1111 1111 in binary and select which channels we want to use
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
        if (joypad()) {
            // 5-0 sound length
            // 0x1F = 0001 1111
            NR41_REG = 0x1F;

            // volume envelope
            // (makes the volume get lounder or quieter each "tick")
            // 7-4 initial channel volume (0 - no sound)
            // 3   volume sweep direction(0 is down, 1 is up)
            // 2-0 length of each step in sweep(if is 0, sweeping is off)
            // (how quickly the sound fades)
            // Note: each step is n/64 seconds long, where n is 1-7
            // 0xF1 = 1111 0001, volume 7, sweep down, sweep length 3
            NR42_REG = 0xF1;

            // 7-4 shift clock frequency (s)
            // 3   counter step/width (width of steps)
            // 2-0 dividing ratio of frequencies (r)
            // The amplitude is randomly switched between high and low at the
            // giving frequency.
            // A higher frequency will make the noise to appear "softer".
            // When bit 3 is set, the output will become more regular, and some
            // of the frequencies will sound more like tone than noise
            // 0x30 = 0011 0000, shift clock frequency 3, step 0,
            // dividing ratio 0
            NR43_REG = 0x30;

            // 7   initialize (trigger channel start, AKA channel INIT)
            // (write only)
            // when bit 7 is 1, it means restarting the sound
            // 6   consecutive select/length counter enable (read/write)
            // regardless of the lenght of data in register 1, when it is "0",
            // sound can be produced until the next sound is played
            // *The sound will last forever or stop when NR41 is over
            // 5-0 unused
            // 0xC3 = 1100 0000, initialize, no consecutive
            NR44_REG = 0xC0;

            delay(100);
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
