#include "display.h"

Display::Display()
{
	screen = SDL_SetVideoMode(WIN_W, WIN_H, 32, SDL_HWSURFACE);

	if (screen == NULL) {
		fprintf(stderr, "Unable to set %dx%d video: %s\n", SDL_GetError(), WIN_W, WIN_H);
		exit(1);
	}

	pixels = (Uint32 *)screen->pixels;
	pitch_px = screen->pitch / 4;
}

Display::~Display()
{
	// Purposely empty.
}

void Display::clear()
{
	SDL_FillRect(screen, NULL, 0x000000);
}

void Display::update()
{
	SDL_UpdateRect(screen, 0, 0, WIN_W, WIN_H);
}

Uint32 Display::toggle_pixel(int pos_x, int pos_y)
{
	int x = pos_x * STEP_X;
	int y = pos_y * STEP_Y;
	SDL_Rect square = { x, y, STEP_X, STEP_Y };

	Uint32 color = pixels[x + y * pitch_px] ^ 0xffffff;
	SDL_FillRect(screen, &square, color);

	return color;
}