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