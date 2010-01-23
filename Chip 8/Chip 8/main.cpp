#include <stdlib.h>

#include "SDL.h"
#include "Display.h"
#include "CPU.h"

int main(int argc, char *argv[])
{
	if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0 )
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	Display display;
	CPU cpu(display);
	cpu.load_rom("C:\\Users\\Aaron\\Desktop\\15PUZZLE");
	cpu.run();

	return 0;
}