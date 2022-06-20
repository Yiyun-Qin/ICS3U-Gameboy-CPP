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


void main() {
    set_bkg_data(0, 114, mario_data);
    // the screen is 20 x 18 tiles
    set_bkg_tiles(0, 0, 20, 18, mario_map);

    SHOW_BKG;
    DISPLAY_ON;

    waitpad(J_START);

    printf("START OF MY GAME");
}
