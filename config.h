#pragma once
#include <stdio.h>
#include <cmath>
#include <string.h>
#include <SDL.h>

int const SCREEN_HEIGHT = 540;//px
int const SCREEN_WIDTH = 960;//px
int const TILE_WIDTH = 32;//px
int const TILE_HEIGHT = 32;//px
//int const MAP_HEIGHT = 17;//number of terrain tiles
//int const MAP_WIDTH = 50;//number of terrain tiles
double const MARIO_ACCELERATION = 10;//px/s^2
double const MARIO_DEACCELERATION = 0.05;//px/s^2
double const MARIO_MAX_SPEED = 1;//px/s
double const MARIO_JUMP_SPEED = 0.9;//px/s
double const FREE_FALL_ACCELERATION = 0.01;//px/s^2
int const MARIO_HEIGHT = 32;//px
int const MARIO_WIDTH = 22;//px
int const HEART_HEIGHT = 18;//px
int const HEART_WIDTH = 20;//px
int const BUFFER_SIZE = 128;//characters
int const TIME_TEXT_POSITION_X = 2;//px
int const TIME_TEXT_POSITION_Y = 2;//px
int const MENU_MESSAGE_POSITION_X = 270;//px
int const MENU_MESSAGE_POSITION_Y = 250;//px
int const LIFE_DIALOG_POSITION_X = 938;//px
int const LIFE_DIALOG_POSITION_Y = 2;//px
char const MAX_LIFES_NUMBER = 3;
int const NUMBER_OF_LEVELS = 3;
int const WALKING_DURATION = 125;//ms
int const NUMBER_OF_WALKING_FRAMES = 3;
Uint32 const TIME_LIMIT = 10 * 1000; //ms
SDL_Color const BACKGROUND_COLOR = { 0x7b, 0x99, 0xFA };//R G B