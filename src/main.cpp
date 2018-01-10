/*
 * main.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: Jun
 */


#include <iostream>
#include <iomanip>
#include "Chip_8.h"
#include <SDL.h>

// Keypad keymap
uint8_t keymap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};


int main(int arg, char **argv) {
	//Initialize the emulator
	Chip_8 test = Chip_8();


	//Initialize the Game Window
	const int WINDOW_HEIGHT = 512;
	const int WINDOW_WIDTH  = 1024;

	SDL_Window* game_window = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	game_window = SDL_CreateWindow("CHIP-8",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);


	//Initialize the SDL Renderer
	SDL_Renderer *game_render = SDL_CreateRenderer(game_window, -1, 0);
	SDL_RenderSetLogicalSize(game_render, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Initialize texture
	//use SDL_TEXTUREACCESS_STREAMING because it will change frequently
	SDL_Texture* game_texture = SDL_CreateTexture(game_render,
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
			64, 32);

	//pixel buffer
	unsigned short temp_pixel_buffer[2048];
	std::string rom_path;

	//Load rom_path and initialize the emulator
	//Continue emulation cycle

	return 0;
}

