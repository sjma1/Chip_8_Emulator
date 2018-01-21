/*
 * Misc_Functions.cpp
 *
 *  Created on: Jan 20, 2018
 *      Author: Jun
 */

#include "Project_Header.h"

void Display_ROM_Titles(const std::string ROMS[], const int size) {
	for(int i = 0; i < size; i++) {
		std::cout << ROMS[i] << std::endl;
	}
	std::cout << std::endl;
}


bool Valid_ROM(const std::string ROMS[], const int size, std::string chosen_ROM) {
	for(int i = 0; i < size; i++) {
		if(ROMS[i] == chosen_ROM) {
			return true;
		}
	}
	return false;
}

std::string Get_ROM_Directory(std::string ROM) {
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return std::string(buf) + '\\' + "ROM\\" + ROM;
}

void Convert_String_Toupper(std::string &s) {
    std::transform(s.begin(), s.end(), s.begin(),
	   [](unsigned char c){ return std::toupper(c); } // correct
	  );
}
