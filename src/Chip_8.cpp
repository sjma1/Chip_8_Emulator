/*
 * Chip_8.cpp
 *
 *  Created on: Dec 28, 2017
 *      Author: Jun
 */
#include "Project_Header.h"

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

Chip_8::~Chip_8() {

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

void Chip_8::Handle_Case_0x000() {
	switch(this->operation_code & 0x00FF) {

			case 0x0000:
				for(int i = 0; i < 64 * 32; i++) {
					this->graphics[i] = 0;
				}
				this->program_counter += 2;
				break;

			case 0x000E:
				this->stack_pointer -= 1;
				this->program_counter = this->stack[this->stack_pointer];
				this->program_counter += 2;
				break;

			default:
				std::cout << "ERROR, UNKNOWN OPERATION CODE!\n";
				return;
			}
}


void Chip_8::Handle_Case_0x1000() {
	this->program_counter = this->operation_code
			& 0x00FF;
}

void Chip_8::Handle_Case_0x2000() {
	this->stack[this->stack_pointer] = this->program_counter;
	this->stack_pointer++;
	this->program_counter = this->operation_code
			& 0x0FFF;
}

void Chip_8::Handle_Case_0x3000() {
	//Checks if register V is equal
	//Will update the program counter
	//to skip an instruction if so
	if(this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
							== (this->operation_code & 0x00FF)) {
		this->program_counter += 4;
	}
	else {
		this->program_counter += 2;
	}
}

void Chip_8::Handle_Case_0x4000() {
	if(this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
							!= (this->operation_code & 0x00FF)) {
		this->program_counter += 4;
	}
	else {
		this->program_counter += 2;
	}
}

void Chip_8::Handle_Case_0x5000() {
	if(this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
							== this->CPU_register_V[(this->operation_code & 0x00F0) >> 4]) {
		this->program_counter += 4;
	}
	else {
		this->program_counter += 2;
	}
}

void Chip_8::Handle_Case_0x6000() {
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
						 = this->operation_code & 0x00FF;
	this->program_counter += 2;
}

void Chip_8::Handle_Case_0x7000() {
	this->program_counter += 2;
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
						 += this->operation_code & 0x00FF;
}

void Chip_8::Handle_Case_0x8000() {
	switch(this->operation_code & 0x000F) {
	case 0x0000:
		this->Handle_Case_0x0000();
		break;
	case 0x0001:
		this->Handle_Case_0x0001();
		break;
	case 0x0002:
		this->Handle_Case_0x0002();
		break;
	case 0x0003:
		this->Handle_Case_0x0003();
		break;
	case 0x0004:
		this->Handle_Case_0x0004();
		break;
	case 0x0005:
		this->Handle_Case_0x0005();
		break;
	case 0x0006:
		this->Handle_Case_0x0006();
		break;
	case 0x0007:
		this->Handle_Case_0x0007();
		break;
	case 0x000E:
		this->Handle_Case_0x000E();
		break;
	default:
		std::cerr << "ERROR, UNKNOWN CASE\n";
		return;
	}
}

void Chip_8::Handle_Case_0x0000() {
	this->program_counter += 2;
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
						 = this->CPU_register_V[(this->operation_code & 0x00F0) >> 4];
}

void Chip_8::Handle_Case_0x0001() {
	this->program_counter += 2;
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
						 |= this->CPU_register_V[(this->operation_code & 0x00F0) >> 4];
}

void Chip_8::Handle_Case_0x0002() {
	this->program_counter += 2;
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
						 &= this->CPU_register_V[(this->operation_code & 0x00F0) >> 4];
}

void Chip_8::Handle_Case_0x0003() {
	this->program_counter += 2;
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
						 ^= this->CPU_register_V[(this->operation_code & 0x00F0) >> 4];
}

void Chip_8::Handle_Case_0x0004() {
	this->program_counter += 2;
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
						 += this->CPU_register_V[(this->operation_code & 0x00F0) >> 4];

	if(this->CPU_register_V[(this->operation_code & 0x0F00) >> 4]
			> (0xFF - this->CPU_register_V[(this->operation_code & 0x0F00) >> 8])) {
		this->CPU_register_V[0xF] = 1;
	}
	else {
		this->CPU_register_V[0xF] = 0;
	}
}

void Chip_8::Handle_Case_0x0005() {
	this->program_counter += 2;
	if(this->CPU_register_V[(this->operation_code & 0x00F0) >> 4]
					> this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]) {
		this->CPU_register_V[0xF] = 0;
	}
	else {
		this->CPU_register_V[0xF] = 1;
	}
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
					-= this->CPU_register_V[(this->operation_code & 0x00F0) >> 4];
}

void Chip_8::Handle_Case_0x0006() {
	this->program_counter += 2;
	this->CPU_register_V[0xF] = this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] & 0x1;
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] >>= 1;
}

void Chip_8::Handle_Case_0x0007() {
	this->program_counter += 2;
	if(this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
					> this->CPU_register_V[(this->operation_code & 0x00F0) >> 4]) {
		this->CPU_register_V[0xF] = 0;
	}
	else {
		this->CPU_register_V[0xF] = 1;
	}
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] =
			this->CPU_register_V[(this->operation_code & 0x00F0) >> 4]
			- this->CPU_register_V[(this->operation_code & 0x0F00) >> 8];
}

void Chip_8::Handle_Case_0x000E() {
	this->program_counter += 2;
	this->CPU_register_V[0xF] = this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] >> 7;
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] <<= 1;
}

void Chip_8::Handle_Case_0x9000() {
	if(this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]
					!= this->CPU_register_V[(this->operation_code & 0x00F0) >> 4]) {
		this->program_counter += 4;
	}
	else {
		this->program_counter += 2;
	}
}

void Chip_8::Handle_Case_0xA000() {
	this->program_counter += 2;
	this->index_register_I = this->operation_code & 0x0FFF;
}

void Chip_8::Handle_Case_0xB000() {
	this->program_counter = (this->operation_code & 0x0FFF) +
			this->CPU_register_V[0];
}

void Chip_8::Handle_Case_0xC000() {
	this->program_counter += 2;
	this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] = (rand() % 255) & (this->operation_code & 0x00FF);
}

void Chip_8::Handle_Case_0xD000() {
    uint16_t x = this->CPU_register_V[(this->operation_code & 0x0F00) >> 8];
    uint16_t y = this->CPU_register_V[(this->operation_code & 0x00F0) >> 4];
    uint16_t height = (this->operation_code & 0x000F);
    uint16_t pixel;

    this->CPU_register_V[0xF] = 0;
    for(int y_area = 0; y_area < height; y_area++) {
    	pixel = this->total_memory[this->index_register_I + y_area];
    	for(int x_area = 0; x_area < 8; x_area++) {
    		if((pixel & (0x80 >> x_area)) != 0) {
    			if(this->graphics[(x + x_area + ((y + y_area) * 64))] == 1) {
    				this->CPU_register_V[0xF] = 1;
    			}
    			this->graphics[x + x_area + ((y + y_area) * 64)] ^= 1;
    		}
    	}
    }
    this->program_counter += 2;
}

void Chip_8::Handle_Case_0xE000() {
	switch(this->operation_code & 0x00FF) {
	case 0x009E:
		this->Handle_Case_0x009E();
		break;

	case 0x00A1:
		this->Handle_Case_0x00A1();
		break;

	default:
		std::cerr << "ERROR, OPERATION CODE DID NOT MATCH ANY KNOWN\n";
		break;
	}
}

void Chip_8::Handle_Case_0x009E() {
	this->program_counter += 2;
	if(this->key_state[this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]] != 0) {
		this->program_counter += 2;
	}
}

void Chip_8::Handle_Case_0x00A1() {
	this->program_counter += 2;
	if(this->key_state[this->CPU_register_V[(this->operation_code & 0x0F00) >> 8]] == 0) {
		this->program_counter += 2;
	}
}

void Chip_8::Handle_Case_0xF000() {
	switch(this->operation_code & 0x00FF) {
	case 0x0007:
		this->program_counter += 2;
		this->CPU_register_V[(this->operation_code * 0x0F00) >> 8] = this->delay_timer;
		break;

	case 0x000A:
		this->Handle_Case_0x000A();
		break;

	case 0x0015:
		this->Handle_Case_0x0015();
		break;

	case 0x0018:
		this->Handle_Case_0x0018();
		break;

	case 0x001E:
		this->Handle_Case_0x001E();
		break;

	case 0x0029:
		this->Handle_Case_0x0029();
		break;

	case 0x0033:
		this->Handle_Case_0x0033();
		break;

	case 0x0055:
		this->Handle_Case_0x0055();
		break;

	case 0x0065:
		this->Handle_Case_0x0065();
		break;

	default:
		std::cerr << "ERRROR, OPERATION CODE DOES NOT MATCH\n";
		break;
	}
}

void Chip_8::Handle_Case_0x000A() {
	bool key = false;
	for(int i = 0; i < 16; i++) {
		if(this->key_state[i] != 0) {
			this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] = i;
			key = true;
		}
	}
	if(!key) {
		return;
	}
	this->program_counter += 2;
}

void Chip_8::Handle_Case_0x0015() {
	this->program_counter += 2;
	this->delay_timer = this->CPU_register_V[(this->operation_code & 0x0F00) >> 8];
}

void Chip_8::Handle_Case_0x0018() {
	this->program_counter += 2;
	this->sound_timer = this->CPU_register_V[(this->operation_code & 0x0F00) >> 8];
}

void Chip_8::Handle_Case_0x001E() {
	this->program_counter += 2;
	if(this->index_register_I +
			this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] > 0xFFF) {
		this->CPU_register_V[0xF] = 1;
	}
	else {
		this->CPU_register_V[0xF] = 0;
	}
	this->index_register_I +=
				this->CPU_register_V[(this->operation_code & 0x0F00) >> 8];
}

void Chip_8::Handle_Case_0x0029() {
	this->program_counter += 2;
	this->index_register_I = this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] * 0x5;
}

void Chip_8::Handle_Case_0x0033() {
	this->program_counter += 2;
	this->total_memory[this->index_register_I] = this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] / 100;
	this->total_memory[this->index_register_I + 1] = (this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] / 10) % 10;
	this->total_memory[this->index_register_I + 2] = (this->CPU_register_V[(this->operation_code & 0x0F00) >> 8] % 100) % 10;
}

void Chip_8::Handle_Case_0x0055() {
	this->program_counter += 2;
	this->index_register_I = ((this->operation_code & 0x0F00) >> 8) + 1;
	for(int i = 0; i <= ((this->operation_code & 0x0F00) >> 8); i++) {
		this->total_memory[this->index_register_I + i] = this->CPU_register_V[i];
	}
}

void Chip_8::Handle_Case_0x0065() {
	this->program_counter += 2;
	this->index_register_I = ((this->operation_code & 0x0F00) >> 8) + 1;
	this->index_register_I = ((this->operation_code & 0x0F00) >> 8) + 1;
	for(int i = 0; i <= ((this->operation_code & 0x0F00) >> 8); i++) {
		this->CPU_register_V[i] = this->total_memory[this->index_register_I + i];
	}
}

void Chip_8::Next_Emulation_Cycle() {
	this->Set_Operation_Code();
	std::cout << operation_code << std::endl;
	switch(this->operation_code & 0x00FF) {
		// case 00E...
		case 0x000:
			this->Handle_Case_0x000();
			break;

		case 0x1000:
			this->Handle_Case_0x1000();
			break;

		case 0x2000:
			this->Handle_Case_0x2000();
			break;

		case 0x3000:
			this->Handle_Case_0x3000();
			break;

		case 0x4000:
			this->Handle_Case_0x4000();
			break;

		case 0x5000:
			this->Handle_Case_0x5000();
			break;

		case 0x6000:
			this->Handle_Case_0x6000();
			break;

		case 0x7000:
			this->Handle_Case_0x7000();
			break;

		case 0x8000:
			this->Handle_Case_0x8000();
			break;
		case 0x9000:
			this->Handle_Case_0x9000();
			break;

		case 0xA000:
			this->Handle_Case_0xA000();
			break;

		case 0xB000:
			this->Handle_Case_0xB000();
			break;

		case 0xD000:
			this->Handle_Case_0xB000();
			break;

		case 0xE000:
			this->Handle_Case_0xB000();
			break;

		case 0xF000:
			this->Handle_Case_0xB000();
			break;
		default:
			std::cerr << "ERROR, OPERATION CODE DID NOT MATCH\n";
			break;
	}
	if(this->delay_timer > 0) {
		this->delay_timer -= 1;
	}
	if(this->sound_timer > 0) {
		this->sound_timer -= 1;
	}

}
