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

#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"

class Display 
{
public:
	Display();
	~Display();

	void clear();	
	Uint32 toggle_pixel(int x, int y);
	void update();
  Uint8 get_key ();

private:
	SDL_Surface *screen;	// Pointer to an SDL screen.
	Uint32* pixels;			// Pointer to the array containing the screen's pixel data.
	Uint16 pitch_px;		// Length of a line in pixels.

	static const int WIN_W = 640; // Width of the SDL window.
	static const int WIN_H = 320; // Height of the SDL window.
	static const int STEP_X = 10; // Width of a pixel drawn to the screen.
	static const int STEP_Y = 10; // Height of a pixel drawn to the screen.
};

#endif // DISPLAY_H
