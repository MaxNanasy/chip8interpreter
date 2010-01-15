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

Uint32 Display::toggle_pixel(int pos_x, int pos_y)
{
	int x = pos_x * STEP_X;
	int y = pos_y * STEP_Y;

	SDL_Rect square = { x, y, STEP_X, STEP_Y };
	Uint32 color = pixels[x + y * pitch_px] ^ 0xffffff;
	SDL_FillRect(screen, &square, color);
	return color;
}

int Display::run()
{
	SDL_Event event;

	while (1)
	{
		render();

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				return 0;
			}
		}
	}
}

void Display::render()
{
	if (SDL_MUSTLOCK(screen))
	{
		if (SDL_LockSurface(screen) < 0)
		{
			return;
		}
	}

	// START TEST CODE
	int i, j;
	Uint8 buf;
	Uint8 mem[] = {0xff, 0xaf, 0x87, 0x13 };
	int I_addr = 0;
	static int X = 0;
	static int Y = 0;
	int N = 4;

	for (i = 0; i < N; i++)
	{
		buf = mem[I_addr + i];

		for (j = 0; j < sizeof(buf)*8; j++)
		{
			if (buf & 0x80)
				toggle_pixel(X + j, Y + i);

			buf <<= 1;
		}
	}
	// END TEST CODE

	if (SDL_MUSTLOCK(screen))
	{
		SDL_UnlockSurface(screen);
	}

	SDL_UpdateRect(screen, 0, 0, WIN_W, WIN_H);
}
