/*
 * Chip_8.h
 *
 *  Created on: Oct 19, 2017
 *      Author: Jun
 */

#ifndef CHIP_8_H_
#define CHIP_8_H_

class Chip_8 {
	private:
		//Chip_8 has 35 potential opcodes, all are 2 bytes long
		unsigned short operation_code;

		//Chip_8 has 4k total memory
		unsigned char  total_memory[4096];

		//Chip_8 has 15 8-bit registers named from V0,V1,...VE
		unsigned char CPU_register_V[16];

		//Index Register and Program Counter, values from
		//0x000 to 0xFFF
		unsigned short index_register_I;
		unsigned short program_counter;

		//count down to 0, buzzer sounds when sound_timer reaches 0
		unsigned char delay_timer;
		unsigned char sound_timer;

		//used to remember current location when program
		//jumps to new addresses, stack has 16 levels
		unsigned short stack[16];
		unsigned short stack_pointer;



	public:
		//graphics are black/white with a total of 2048 pixels, 64 x 32
		unsigned char graphics[64 * 32];

		// Chip 8 has a HEX based keypad (0x0-0xF), store in array
		unsigned short key_state[16];

		//Constructor/Destructor
		Chip_8();
		~Chip_8();

		void Emulation_Cycle();

		bool Initialize(const char[] *file_path);

};



#endif /* CHIP_8_H_ */
