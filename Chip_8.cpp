
/*
 * Chip_8.cpp
 *
 *  Created on: Dec 28, 2017
 *      Author: Jun
 */
#include "Chip_8.h"
#include <iostream>
#include <stdio.h> // Used for file path of rom

//Fontset used by Chip-8
unsigned char chip8_fontset[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip_8::Chip_8() {
	//Set program_counter, reset operation_code, and reset index register / stack
	program_counter  = 0x200;
	operation_code   = 0;
	index_register_I = 0;
	stack_pointer    = 0;

	//Initialize Graphics, reset each of the 2048 (64 x 32) pixels
	for(int i = 0; i < 64 * 32; i++) {
		graphics[i] = 0;
	}

	//Reset/Initialize the stack, the key state, and the CPU register
	for(int i = 0; i < 16; i++) {
		stack[i] 		  = 0;
		key_state[i] 	  = 0;
		CPU_register_V[i] = 0;
	}

	//Reset/Initialize Memory
	for(int i = 0; i < 4096; i++) {
		total_memory[i] = 0;
	}

	//Reset/Initialize Delay Timer
	delay_timer = 0;

	//Reset/Initialize Sound Timer
	sound_timer = 0;
}

void Chip_8::Reset() {
	//Set program_counter, reset operation_code, and reset index register / stack
	program_counter  = 0x200;
	operation_code   = 0;
	index_register_I = 0;
	stack_pointer    = 0;

	//Initialize Graphics, reset each of the 2048 (64 x 32) pixels
	for(int i = 0; i < 64 * 32; i++) {
		graphics[i] = 0;
	}

	//Reset/Initialize the stack, the key state, and the CPU register
	for(int i = 0; i < 16; i++) {
		stack[i] 		  = 0;
		key_state[i] 	  = 0;
		CPU_register_V[i] = 0;
	}

	//Reset/Initialize Memory
	for(int i = 0; i < 4096; i++) {
		total_memory[i] = 0;
	}

	//Reset/Initialize Delay Timer
	delay_timer = 0;

	//Reset/Initialize Sound Timer
	sound_timer = 0;
}

bool Chip_8::Initialize(const char *file_path) {
	//Used for telling FILE functions
	//how many bytes to move from
	const int FILE_OFFSET = 0;

	//Reset the Emulator in case it has been used before
	this->Reset();
	std::cout << "ATTEMPTING TO OPEN: \n";
	std::cout << file_path << std::endl;

	//Get ROM from file path
	FILE *rom_file = fopen(file_path, "rb");

	//File path exists and file was successfully read
	if(rom_file != NULL) {

		//Move cursor from beginning byte of file to end
		//Then get filesize by checking where the end of the
		//file is
		fseek(rom_file,FILE_OFFSET, SEEK_END);
		int rom_filesize = ftell(rom_file);
		fseek(rom_file, FILE_OFFSET, SEEK_SET);

		//Memory Allocation
		char *rom_mem = new char(sizeof(char) * rom_filesize);
		if(rom_mem == NULL) {
			std::cerr << "NOT ENOUGH MEMORY AVAILABLE FOR ROM!\n";
			delete rom_mem;
			return false;
		}

		//Copy the ROM into buffer
		size_t buffer_size = fread(rom_mem, sizeof(char),
				(size_t) rom_filesize, rom_file);

		if(buffer_size != (size_t)rom_filesize) {
			std::cerr << "Error, buffer does not match ROM filesize!\n";
			delete rom_mem;
			return false;
		}

		//Copy the buffer to the memory
		if(buffer_size >= 4096) {
			std::cerr << "Error, this ROM exceeds filesize capabilities!\n";
			delete rom_mem;
			return false;
		}
		for(int i = 0; i < (int)buffer_size; i++) {
			this->total_memory[i + 512] = (unsigned char)rom_mem[i];
		}
		fclose(rom_file);
		free(rom_mem);
		return true;
	}

	//Invalid file path given as argument
	std::cerr << "ERROR, INVALID FILE PATH!";
	return false;
}

void Chip_8::Set_Operation_Code() {
	this->operation_code = this->total_memory[this->program_counter] << 8
			| this->total_memory[this->program_counter + 1];
}


void Chip_8::Emulation_Cycle() {
	this->Set_Operation_Code();
}
