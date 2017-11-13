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

using namespace std;

int main(int arg, char **argv) {

	Chip_8 emulator = new Chip_8();

	//create_graphics()

	//get_input()

	//Start SDL
	SDL2_Init( SDL_INIT_EVERYTHING );

	//Load Game

	//Infinite Emulation Loop

		//Emulate a cycle
		emulator.Emulation_Cycle();

		//Update Screen


	//Quit SDL
	SDL2_Quit();
	return 0;
}

