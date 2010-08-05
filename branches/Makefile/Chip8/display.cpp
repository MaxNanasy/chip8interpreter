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

#include "display.h"

Display::Display()
{
  screen = SDL_SetVideoMode(WIN_W, WIN_H, BPP, SDL_HWSURFACE);

  if (screen == NULL) {
    fprintf(stderr, "Unable to set %dx%d video: %s\n", WIN_W, WIN_H, SDL_GetError ());
    exit(1);
  }

  pixels = (Uint32 *)screen->pixels;
  pitch_px = (8 * screen->pitch) / BPP;
}

Display::~Display()
{
  // Purposely empty.
}

void Display::clear()
{
  SDL_FillRect(screen, NULL, 0x000000);
}

void Display::updateRect(int x, int y, int w, int h)
{
  SDL_UpdateRect(screen, x * STEP_X, y * STEP_Y, w * STEP_X, h * STEP_Y);
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

Uint8 Display::get_key ()
{
  SDL_Event event;
  do SDL_WaitEvent (&event); while (event.type != SDL_KEYDOWN);
  return event.key.keysym.unicode;
}
