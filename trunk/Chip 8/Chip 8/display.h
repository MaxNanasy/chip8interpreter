#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"

class Display 
{
public:
	Display();
	~Display();

	int run();
	Uint32 toggle_pixel(int x, int y);
	void clear();

private:
	SDL_Surface *screen;	// Pointer to an SDL screen.
	Uint32* pixels;			// Pointer to the array containing the screen's pixel data.
	Uint16 pitch_px;		// Length of a line in pixels.

	static const int WIN_W = 640; // Width of the SDL window.
	static const int WIN_H = 320; // Height of the SDL window.
	static const int STEP_X = 10; // Width of a pixel drawn to the screen.
	static const int STEP_Y = 10; // Height of a pixel drawn to the screen.

	void render();
};

#endif // DISPLAY_H