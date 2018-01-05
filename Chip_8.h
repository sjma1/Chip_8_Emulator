/*
 * Chip_8.h
 *
 *  Created on: Oct 19, 2017
 *      Author: Jun
 */

#ifndef CHIP_8_H_
#define CHIP_8_H_

class Chip_8 {
	public:
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


		//Uses the program counter to set the operation code
		void Set_Operation_Code();

		//Following methods are all helper methods for Next_Emulation_Cycle()
		void Handle_Case_0x000();
		void Handle_Case_0x1000();
		void Handle_Case_0x2000();
		void Handle_Case_0x3000();
		void Handle_Case_0x4000();
		void Handle_Case_0x5000();
		void Handle_Case_0x6000();
		void Handle_Case_0x7000();
		void Handle_Case_0x8000();
		//The following are helper methods for Handle_Case_0x8000()
		void Handle_Case_0x0000();
		void Handle_Case_0x0001();
		void Handle_Case_0x0002();
		void Handle_Case_0x0003();
		void Handle_Case_0x0004();
		void Handle_Case_0x0005();
		void Handle_Case_0x0006();
		void Handle_Case_0x0007();
		void Handle_Case_0x000E();

		void Handle_Case_0x9000();
		void Handle_Case_0xA000();
		void Handle_Case_0xB000();
		void Handle_Case_0xC000();
		void Handle_Case_0xD000();
		void Handle_Case_0xE000();
		//The following are helper methods for Handle_Case_0xE000()
		void Handle_Case_0x009E();
		void Handle_Case_0x00A1();

		void Handle_Case_0xF000();
		//The following are helper methods for Handle_Case_0xF000()
		void Handle_Case_0x000A();
		void Handle_Case_0x0015();
		void Handle_Case_0x0018();
		void Handle_Case_0x001E();
		void Handle_Case_0x0029();
		void Handle_Case_0x0033();
		void Handle_Case_0x0055();
		void Handle_Case_0x0065();


	public:
		//graphics are black/white with a total of 2048 pixels, 64 x 32
		unsigned char graphics[64 * 32];

		// Chip 8 has a HEX based keypad (0x0-0xF), store in array
		unsigned short key_state[16];

		//Constructor/Destructor
		Chip_8();
		~Chip_8();

		void Next_Emulation_Cycle();

		void Reset();

		//method to start up the emulator
		bool Initialize(const char *file_path);

};



#endif /* CHIP_8_H_ */
