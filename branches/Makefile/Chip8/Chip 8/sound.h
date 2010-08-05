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

#ifndef SOUND_H
#define SOUND_H

#include <SDL/SDL.h>

class Sound
{

 public:

  Sound (Uint32);
  ~Sound ();

  void play (Uint32);

 private:

  static const Uint32 NUM_SAMPLES = 0x10000;

  Uint32 freq;
  Uint32 samplesPerPeriod;

  SDL_TimerID timerID;
  SDL_mutex *id_mutex;

  Uint32 timer_callback (Uint32);
  static Uint32 static_timer_callback (Uint32, void*);

  void callback (Uint8*, int);
  static void static_callback (void*, Uint8*, int);

  Uint8 *samples;
  Uint32 currentSample;

  void on  ();
  void off ();

};

#endif // SOUND_H
