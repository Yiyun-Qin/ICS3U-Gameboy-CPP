// Copyright (c) 2019 St. Mother Teresa HS All rights reserved.
//
// Created by: Yiyun Qin
// Created on: May 2022
// This program is the sprite program for the GameBoy

#include <stdio.h>
#include <gb/gb.h>
#include <gb/font.h>

#include <simpleBackground.c>
#include <simpleBackgroundMap.c>
#include <windowmap.c>
#include <character.h>

INT16 playerLocation[2];
BYTE jumping;
UINT8 gravity = 4;
UINT8 currentSpeedY;
UINT8 floorY = 100;


void performantDelay(UINT8 loopCounter) {
    INT8 i;
    for (i = 0; i < loopCounter; i++) {
        // less CPU use than delay
        wait_vbl_done();
    }
}


INT8 wouldHitSurface(UINT8 positionY) {
    if (positionY >= floorY) {
        return floorY;
    } else {
        return -1;
    }
}


void jump() {
    INT8 possibleSurfaceY;
    if (jumping == 0) {
        jumping = 1;
        currentSpeedY = 10;
    }
    // gravity slows the speed
    currentSpeedY = currentSpeedY - gravity;
    playerLocation[1] = playerLocation[1] - currentSpeedY;
    possibleSurfaceY = wouldHitSurface(playerLocation[1]);
    if (possibleSurfaceY != -1) {
        // hit the surface
        jumping = 0;  // stop jumping
        move_sprite(0, playerLocation[0], floorY);  // move it on the floor
    } else {
        move_sprite(0, playerLocation[0], playerLocation[1]);
    }
}


void main() {
    // sprite x and y coordinates
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
    set_sprite_data(0, 8, bloke);
    // number of sprite, the nth tile(smile)
    set_sprite_tile(0, 0);

    // x
    playerLocation[0] = 10;
    // y
    playerLocation[1] = floorY;
    // jumping
    jumping = 0;

    // move the nth sprite to (88, 78)
    move_sprite(0, playerLocation[0], playerLocation[1]);

    // show the map
    SHOW_BKG;
    SHOW_WIN;
    DISPLAY_ON;
    SHOW_SPRITES;

    while (1) {
        // scroll background
        scroll_bkg(1, 0);
        delay(100);

        // sprite moving
        switch (joypad()) {
            case J_LEFT:
            // index of sprite to access, x, y
                if (playerLocation[0] > spriteX) {
                    playerLocation[0] = playerLocation[0] - 2;
                    move_sprite(0, playerLocation[0], playerLocation[1]);
                    // when I add this jumping code,
                    // I can fly when pressing left
                    // If I don't add this code, I jump when I press left
                    jumping = 0
                }
            break;
            case J_RIGHT:
                if (playerLocation[0] < screenX - spriteX) {
                    playerLocation[0] = playerLocation[0] + 2;
                    move_sprite(0, playerLocation[0], playerLocation[1]);
                    jumping = 0;
                }
            break;
        }
        if ((joypad() && J_A) || jumping == 1) {
            jump();
        }
        performantDelay(5);
    }
}
