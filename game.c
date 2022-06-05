// Copyright (c) 2019 St. Mother Teresa HS All rights reserved.
//
// Created by: Yiyun Qin
// Created on: May 2022
// This program is the sprite program for the GameBoy

#include <stdio.h>
#include <gb/gb.h>

#include <smiler.h>


int main() {
    // start from nth tiles, to load how many elements, file name
    set_sprite_data(0, 1, TileData);
    // number of sprite, the nth tile(smile)
    set_sprite_tile(0, 0);
    // move the nth sprite to (88, 78)
    move_sprite(0, 88, 78);
    SHOW_SPRITES;

    while (1) {
        switch (joypad()) {;
            case J_LEFT:
            // index of sprite to access, x, y
                    scroll_sprite(0, -10, 0);
            break;
            case J_RIGHT:
                scroll_sprite(0, 10, 0);
            break;
            case J_UP:
                scroll_sprite(0, 0, -10);
            break;
            case J_DOWN:
                scroll_sprite(0, 0, 10);
            break;
        }
        delay(100);
    }
}
