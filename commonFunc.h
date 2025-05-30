#ifndef COMMON_FUNC_H
#define COMMON_FUNC_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

extern SDL_Window* g_window;
extern SDL_Renderer* g_render;
extern SDL_Event g_event;
extern TTF_Font* g_font;

extern Mix_Music* g_background_music;  // Nhạc nền
extern Mix_Chunk* g_attack_sound; // Âm thanh vũ khí

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 255;

const int TILE_SIZE_WIDTH = 16;
const int TILE_SIZE_HEIGHT = 24;

extern int map_width ;
extern int map_height ;

#endif