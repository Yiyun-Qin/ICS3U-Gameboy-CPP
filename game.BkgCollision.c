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
#include <snail.c>
#include <mazesprite.c>
#include <mazemap.c>


// one element in blankmap
const char blankmap[1] = {0x00};
const char key[1] = {0x03};
const char door[1] = {0x02};
// UINT instead of INT
UINT8 playerLocation[2];
UBYTE debug, haskey, gamerunning;


void performantDelay(UINT8 loopCounter) {
    INT8 i;
    for (i = 0; i < loopCounter; i++) {
        // less CPU use than delay
        wait_vbl_done();
    }
}


// return true when can move, false when can't move
UBYTE canMove(UINT8 newPlayerX, UINT8 newPlayerY) {
    // index top-left x
    UINT16 indexTLX;
    UINT16 indexTLY;
    UINT16 indexTLtile;
    UBYTE result;

    // position number x is 8 bigger than TL position
    // a sprite right is 16 instead of 8
    // divide by 8 to get column and row
    // if the player us at 8, the row is the index 0
    indexTLX = (newPlayerX - 8) / 8;
    // position number y is 16 bigger than TL position
    // e.g. at one sprite down is 24 instead of 8
    indexTLY = (newPlayerY - 16) / 8;
    // one sprite down and right is (16, 24) instead of (8, 8)
    // 20 index a row * how many rows + how many column
    // the first row and column is 0. So + 1 means the second index
    indexTLtile = 20 * indexTLY + indexTLX;

    if (debug) {
        // unsigned int
        printf("%u %u \n", (UINT16)(newPlayerX), (UINT16)(newPlayerY));
        printf("%u %u %u \n", (UINT16)(indexTLX), (UINT16)(indexTLY),
        (UINT16)(indexTLtile));
    }

    // if is blank, return True
    result = MazeMap[indexTLtile] == blankmap[0];

    if (MazeMap[indexTLtile] == key[0]) {
        // 0x03 is the key in MazeMap
        // where the key is
        // start at position (1, 16), 1 row 1 column
        set_bkg_tiles(indexTLX, indexTLY, 1, 1, blankmap);
        result = 1;
        haskey = 1;  // have key
    } else if (MazeMap[indexTLtile] == door[0] && haskey) {
        // go through door
        set_bkg_tiles(indexTLX, indexTLY, 1, 1, blankmap);
        result = 1;
    } else if (indexTLtile == 340) {
        // finish game
        gamerunning = 0;
        HIDE_SPRITES;
        printf("\n \n \n \n \n \n \n \n \n      YOU WIN!");
        result = 1;
    }

    return result;
}


//  which byte we want to animate, how much to move in x and y
//  because x and y can be negative, so use int
void animateSprite(UINT8 spriteIndex, INT8 moveX, INT8 moveY) {
    // while it's move in x or y
    while (moveX != 0) {
        scroll_sprite(spriteIndex, moveX < 0 ? -1 : 1, 0);
        moveX += (moveX < 0 ? 1 : -1);
        wait_vbl_done();
    }
    while (moveY != 0) {
        scroll_sprite(spriteIndex, 0, moveY < 0 ? -1 : 1);
        moveY += moveY < 0 ? 1 : -1;
        wait_vbl_done();
    }
}


void main() {
    // sprite x and y coordinates
    const screenX = 160;
    const screenY = 144;
    const spriteX = 8;
    const spriteY = 8;

    // background
    set_bkg_data(0, 4, MazeSprites);
    // start at 0 in 0, 20 column(x), 18 rows(y),
    // go to backgroundmap to know how to use
    set_bkg_tiles(0, 0, 20, 18, MazeMap);

    // load sprite
    // start from nth tiles, to load how many elements, file name
    set_sprite_data(0, 1, Snail);
    // number of sprite, the nth tile(smile)
    set_sprite_tile(0, 0);

    // x
    playerLocation[0] = 16;
    // y
    playerLocation[1] = 24;

    // move the nth sprite to (88, 78)
    move_sprite(0, playerLocation[0], playerLocation[1]);

    gamerunning = 1;

    // show the map
    SHOW_BKG;
    DISPLAY_ON;
    SHOW_SPRITES;

    while (gamerunning) {
        if (joypad() & J_A) {
            debug = 1;
        }
        // sprite moving
        if (joypad() & J_LEFT) {
        // index of sprite to access, x, y
            // test whether the destination is blank
            if (canMove(playerLocation[0] - 8, playerLocation[1])) {
                playerLocation[0] = playerLocation[0] - 8;
                animateSprite(0, -8, 0);
            }
        } else if (joypad() & J_RIGHT) {
            if (canMove(playerLocation[0] + 8, playerLocation[1])) {
                playerLocation[0] = playerLocation[0] + 8;
                animateSprite(0, 8, 0);
            }
        } else if (joypad() & J_UP) {
            if (canMove(playerLocation[0], playerLocation[1] - 8)) {
                playerLocation[1] = playerLocation[1] - 8;
                animateSprite(0, 0, -8);
            }
        } else if (joypad() & J_DOWN) {
            if (canMove(playerLocation[0], playerLocation[1] + 8)) {
                playerLocation[1] = playerLocation[1] + 8;
                animateSprite(0, 0, 8);
            }
        }
        performantDelay(6);
    }
}
