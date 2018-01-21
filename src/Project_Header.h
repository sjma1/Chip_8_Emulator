/*
 * Project_Header.h
 *
 *  Created on: Jan 20, 2018
 *      Author: Jun
 */

#ifndef PROJECT_HEADER_H_
#define PROJECT_HEADER_H_

//Preprocessor Directives
#include <iostream>
#include <iomanip>
#include "Chip_8.h"
#include <SDL.h>
#include <windows.h>
#include <string>
#include <algorithm>


void Display_ROM_Titles(const std::string ROMS[], const int size);

bool Valid_ROM(const std::string ROMS[], const int size, std::string chosen_ROM);

std::string Get_ROM_Directory(std::string ROM);

void Convert_String_Toupper(std::string &s);

#endif /* PROJECT_HEADER_H_ */
