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

#include <cmath>

#include <SDL/SDL.h>

#include "sound.h"

Sound::Sound (Uint32 freq)
  : freq (freq), timerID (0), id_mutex (SDL_CreateMutex ()), currentSample (0)
{

  SDL_AudioSpec desiredAudio, obtainedAudio;
  desiredAudio.freq = 22050;
  desiredAudio.format = AUDIO_S8;
  desiredAudio.channels = 1;
  desiredAudio.samples = 0x400;
  desiredAudio.callback = &static_callback;
  desiredAudio.userdata = this;

  if (SDL_OpenAudio(&desiredAudio, &obtainedAudio) < 0) {
    fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
    exit(-1);
  }

  samplesPerPeriod = obtainedAudio.freq / freq;
  samples = (Uint8 *) malloc (samplesPerPeriod * sizeof (Uint8));

  for (Uint32 i = 0; i < samplesPerPeriod; i ++)
    samples [i] = 127 * cos (i * 6.28 / samplesPerPeriod);

}

Sound::~Sound ()
{
  free (samples);
  if (timerID)
    SDL_RemoveTimer (timerID);
  SDL_DestroyMutex (id_mutex);
}

void Sound::play (Uint32 duration)
{
  SDL_mutexP (id_mutex); {
    if (timerID)
      SDL_RemoveTimer (timerID);
    if (duration) {
      on ();
      timerID = SDL_AddTimer (duration, static_timer_callback, this);
      if (! timerID) {
        fprintf(stderr, "Unable to add timer: %s\n", SDL_GetError ());
        exit (1);
      }
    }
    else {
      off ();
      timerID = 0;
    }
  } SDL_mutexV (id_mutex);
}

Uint32 Sound::timer_callback (Uint32 duration)
{
  SDL_mutexP (id_mutex); {
    off ();
    timerID = 0;
  } SDL_mutexV (id_mutex);
  return 0;
}

Uint32 Sound::static_timer_callback (Uint32 duration, void *sound)
{
  return ((Sound*) sound) -> timer_callback (duration);
}

void Sound::on ()
{
  SDL_PauseAudio (false);
}

void Sound::off ()
{
  SDL_PauseAudio (true);
}

void Sound::callback (Uint8 *stream, int len)
{
  for (int i = 0; i < len; i ++, currentSample = (currentSample + 1) % samplesPerPeriod)
    stream [i] = samples [currentSample];
}

void Sound::static_callback (void *sound, Uint8 *stream, int len)
{
  ((Sound*) sound) -> callback (stream, len);
}
