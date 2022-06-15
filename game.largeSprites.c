// Copyright (c) 2019 St. Mother Teresa HS All rights reserved.
//
// Created by: Yiyun Qin
// Created on: May 2022
// This program is the sprite program for the GameBoy

#include <stdio.h>
#include <gb/gb.h>

#include <simpleBackground.c>
#include <simpleBackgroundMap.c>
#include <gameCharacter.c>
#include <gamesprites.c>

struct GameCharacter ship;
struct GameCharacter bug;
UBYTE spriteSize = 8;


void performantDelay(UINT8 loopCounter) {
    INT8 i;
    for (i = 0; i < loopCounter; i++) {
        // less CPU use than delay
        wait_vbl_done();
    }
}


UBYTE checkCollisions(struct GameCharacter* one, struct GameCharacter* two) {
     return (one->x >= two->x && one->x <= two->x + two->width) &&
     (one->y >= two->y && one->y <= two->y + two->height) ||
     (two->x >= one->x && two->x <= one->x + one->width) &&
     (two->y >= one->y && two->y <= one->y + one->height);
}


void MoveGameCharacter(struct GameCharacter* character, UINT8 x, UINT8 y) {
    // because using a pointer*, we need to use -> instead of .
    // move spriteID[0] into x and y
    move_sprite(character->spriteID[0], x, y);
    move_sprite(character->spriteID[1], x + spriteSize, y);
    move_sprite(character->spriteID[2], x, y + spriteSize);
    move_sprite(character->spriteID[3], x + spriteSize, y + spriteSize);
}


void SetupShip() {
    ship.x = 80;
    ship.y = 130;
    ship.width = 16;
    ship.height = 16;

    // The first sprite of the ship is the 0th sprite
    set_sprite_tile(0, 0);
    ship.spriteID[0] = 0;
    set_sprite_tile(1, 1);
    ship.spriteID[1] = 1;
    set_sprite_tile(2, 2);
    ship.spriteID[2] = 2;
    set_sprite_tile(3, 3);
    ship.spriteID[3] = 3;

    MoveGameCharacter(&ship, ship.x, ship.y);
}


void SetupBug() {
    bug.x = 30;
    bug.y = 0;
    bug.width = 16;
    bug.height = 16;

    // The first sprite of the ship is the 0th sprite
    set_sprite_tile(4, 4);
    bug.spriteID[0] = 4;
    set_sprite_tile(5, 5);
    bug.spriteID[1] = 5;
    set_sprite_tile(6, 6);
    bug.spriteID[2] = 6;
    set_sprite_tile(7, 7);
    bug.spriteID[3] = 7;

    MoveGameCharacter(&bug, bug.x, bug.y);
}


void main() {
    // sprite x and y coordinates
    const screenX = 160;
    const screenY = 144;

    // background
    // start at 37 to prevent overlap with font
    //   set_bkg_data(37, 7, backgroundtiles);
    // start at 0 in 0, x, y, go to backgroundmap to know how to use
    //   set_bkg_tiles(0, 0, 40, 18, backgroundmap);

    // sprite tiles
    set_sprite_data(0, 8, LargeSprites);

    // load ship and bug
    SetupShip();
    SetupBug();

    // show the map
    // SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    // a collision will return true(1), no collision will return false(0)
    // ! make false becomes true, true becomes false
    while (!checkCollisions(&ship, &bug)) {
        // scroll background
        // scroll_bkg(1, 0);
        // delay(100);

        // sprite moving
        switch (joypad()) {
            case J_LEFT:
            // index of sprite to access, x, y
                if (ship.x > 8) {
                    ship.x = ship.x - 2;
                    MoveGameCharacter(&ship, ship.x, ship.y);
                }
            break;
            case J_RIGHT:
                if (ship.x <= 150) {
                    ship.x = ship.x + 2;
                    MoveGameCharacter(&ship, ship.x, ship.y);
                }
            break;
        }

        bug.y += 5;
        if (bug.y >= 144) {
            bug.y = 0;
            bug.x = ship.x;
        }
        MoveGameCharacter(&bug, bug.x, bug.y);

        performantDelay(5);
    }

    printf("\n \n \n \n \n \n \n === GAME  OVER ===");
}
