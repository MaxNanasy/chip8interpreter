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

#ifndef CPU_H
#define CPU_H

#include <SDL/SDL.h>

#include "display.h"
#include "sound.h"

typedef Uint32 clock_time;

class CPU 
{
public:
  CPU(Display& display, Sound& sound);
  ~CPU();
  void run();
  void load_rom(const char file_name[]);

private:
  static const unsigned int KiB = 1024;
  static const int START_CHAR_DATA = 0x00;
  static const int OPCODE_SIZE = 2;
  static const clock_time CLOCK_RES = 1000;
  static const clock_time TIMER_RES = 60;
  static const float TIMER_RATIO = TIMER_RES / (1.0 * CLOCK_RES);

  Display& display;  // Pointer to the display.
  Sound& sound; // Pointer to sound.
  Uint8 V[16];    // Data registers
  Uint8 t_delay;    // Delay Timer
  clock_time latest_delay_set; // Latest time delay timer was set.
  Uint16 I_addr;    // Index address: pointer to location in memory.
  Uint16 PC;      // Instruction address register
  Uint16 SP;      // Stack pointer
  Uint16 stack[16];  // Stack: Only used to store return addresses when subroutines are called.
  Uint16 cur_op;    // Current instruction
  Uint8 mem[4 * KiB];  // Memory
  bool program_active; // Program not terminated
  
  // These functions return parameters from the current opcode.
  inline int get_X() { return (cur_op & 0x0F00) >> 8; } 
  inline int get_Y() { return (cur_op & 0x00F0) >> 4; }
  inline int get_N() { return (cur_op & 0x000F); }
  inline int get_NN() { return (cur_op & 0x00FF); }
  inline int get_NNN() { return (cur_op & 0x0FFF); }

  // METHODS
  void execute_opcode();  
  void initialize_font();
  void cycle();

  clock_time get_time ();
  
  // OPCODES
  void clear_screen();
  void sub_return();
  void terminate_program();
  void jump();
  void sub_call();
  void skip_equal_imm();
  void skip_not_equal_imm();
  void skip_equal();
  void set_imm();
  void add_imm();
  void set();
  void or_op();
  void and_op();
  void xor_op();
  void add();
  void x_sub_y();
  void shift_right();
  void y_sub_x();
  void shift_left();
  void skip_not_equal();
  void set_index();
  void jump_offset();
  void set_rand();
  void draw_sprite();
  void skip_on_pressed();
  void skip_not_pressed();
  void load_delay_timer();
  void wait_on_press();
  void set_delay_timer();
  void set_sound_timer();
  void add_to_index();
  void set_I_to_char();
  void store_BCD();
  void save_regs_to_mem();
  void load_regs_from_mem();
  void invalid_opcode();
};

#endif // CPU_H
