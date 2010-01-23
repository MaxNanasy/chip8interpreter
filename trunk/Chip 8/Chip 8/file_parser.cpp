//    This program is a Chip-8 interpreter. It takes Chip-8 programs and runs them.
//    Copyright (C) 2010  Aaron Peschel
//
//    This file is part of Chip-8 Interpreter.
//
//    Chip-8 Interpreter is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Chip-8 Interpreter is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Chip-8 Interpreter.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <fstream>
#include <vector>

#include "SDL.h"
#include "file_parser.h"

using namespace std;

file_parser::file_parser(const char file_name[])
{
	rom_file.open(file_name, ifstream::binary);
	if (!rom_file)
	{
		cerr << "Cannot open file " << file_name << endl;
		exit(1);
	}

	Uint8 datum;
	while (rom_file.good())
	{
		datum = rom_file.get();
		rom_data.push_back(datum);
	}
	rom_data.pop_back(); // Trim the EOF character.

	rom_file.close();
}

file_parser::~file_parser()
{
	// Purposely Empty
}

vector<Uint8> file_parser::return_rom_data()
{
	return rom_data;
}