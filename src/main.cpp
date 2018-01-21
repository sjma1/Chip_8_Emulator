/*
 * main.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: Jun
 */


#include "Project_Header.h"

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
	const int ROMS_SIZE = 4;
	const std::string ROMS[ROMS_SIZE] = {"INVADERS", "TANK", "TETRIS", "PONG"};
	std::string chosen_ROM;
	do {
		std::cout << "AVAILABLE ATARI GAMES TO PLAY: \n";
		Display_ROM_Titles(ROMS, ROMS_SIZE);
		std::cout << "Enter a valid game: ";
		getline(std::cin, chosen_ROM);
		Convert_String_Toupper(chosen_ROM);
	}while(!Valid_ROM(ROMS, ROMS_SIZE, chosen_ROM));

	//Initialize the emulator
	Chip_8 test = Chip_8();


	//Initialize the Game Window
	const int WINDOW_HEIGHT = 512;
	const int WINDOW_WIDTH  = 1024;

	SDL_Window* game_window = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);
	game_window = SDL_CreateWindow("CHIP-8 Interpreter/Emulator",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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
	std::string user_ROM;

	//Load rom_path and initialize the emulator
	//Continue emulation cycle
	test.Initialize(Get_ROM_Directory(chosen_ROM).c_str());

	while(true) {
		test.Next_Emulation_Cycle();
		SDL_Event event;

		//while an SDL event is being detected
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				std::cerr << "No event detected!";
				exit(0);
			}
			else if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					exit(0);
					break;

				//Reload ROM
				case SDLK_F1:
					std::cout << "RELOADING\n";
					test.Initialize(Get_ROM_Directory(chosen_ROM).c_str());
					break;
				default:
					break;
				for(int i = 0; i < 16; i++) {
					 if(event.key.keysym.sym == keymap[i]) {
						 test.key_state[i] = 1;
					 }
				}
				}
			}
			else if(event.type == SDL_KEYUP) {
				for(int i = 0; i < 16; i++) {
					 if(event.key.keysym.sym == keymap[i]) {
						 test.key_state[i] = 0;
					 }
				}
			}
		}
	}

	return 0;
}

