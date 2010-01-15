#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <fstream>
#include <vector>

#include "SDL.h"

using namespace std;

class file_parser
{
public:
	file_parser(const char file_name[]);
	~file_parser();

	vector<Uint8> return_rom_data();

private:
	ifstream rom_file;
	vector<Uint8> rom_data;
};

#endif // FILE_PARSER_H