// Copyright (c) 2019 St. Mother Teresa HS All rights reserved.
//
// Created by: Yiyun Qin
// Created on: May 2022
// This program is the sprite program for the GameBoy

#include <stdio.h>
#include <gb/gb.h>
#include <gb/font.h>

#include <mario_data.c>
#include <mario_map.c>
#include <mazesprite.c>
#include <mazemap.c>

UINT8 fade;


void performantDelay(UINT8 loopCounter) {
    INT8 i;
    for (i = 0; i < loopCounter; i++) {
        // less CPU use than delay
        wait_vbl_done();
    }
}


void fadeout() {
    for (fade = 0; fade < 4; fade++) {
        switch (fade) {
            case 0:
                BGP_REG = 0xE4;
                break;
                // fade the screen
                // 11 is black, 10 is dark gray,
                // 01 is light gray, 00 is white
                // E4 = 1110 0100, F9 = 1111 1001,
                // FE = 1111 1110, FF = 1111 1111
                // this can make screen look like it's fading
            case 1:
                BGP_REG = 0xF9;
                break;
            case 2:
                BGP_REG = 0xFE;
                break;
            case 3:
                BGP_REG = 0xFF;
                break;
        }
        performantDelay(100);
    }
}


void fadein() {
    for (fade = 0; fade < 3; fade++) {
        switch (fade) {
            case 0:
                BGP_REG = 0xFE;
                break;
                // fade the screen
                // 11 is black, 10 is dark gray, 01 is light gray, 00 is white
                // E4 = 1110 0100, F9 = 1111 1001,
                // FE = 1111 1110, FF = 1111 1111
                // this can make screen look like it's fading
            case 1:
                BGP_REG = 0xF9;
                break;
            case 2:
                BGP_REG = 0xE4;
                break;
        }
        performantDelay(10);
    }
}


void main() {
    set_bkg_data(0, 114, mario_data);
    // the screen is 20 x 18 tiles
    set_bkg_tiles(0, 0, 20, 18, mario_map);

    SHOW_BKG;
    DISPLAY_ON;

    waitpad(J_START);
    fadeout();

    set_bkg_data(0, 4, MazeSprites);
    // the screen is 20 x 18 tiles
    set_bkg_tiles(0, 0, 20, 18, MazeMap);

    fadein();
}
