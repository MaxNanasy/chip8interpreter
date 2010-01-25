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

#include <stdlib.h>

#include "SDL.h"
#include "display.h"
#include "cpu.h"

int main(int argc, char *argv[])
{

  if (argc < 2) {
    fprintf (stderr, "Error: No argument to %s.\n  Usage: %s [BINARY_CHIP8_PROGRAM]\n", argv [0], argv [0]);
    exit (1);
  }

	if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 )
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	Display display;
  Sound sound (100);
	CPU cpu(display, sound);

  cpu.load_rom(argv [1]);
	cpu.run();

	return 0;

}
