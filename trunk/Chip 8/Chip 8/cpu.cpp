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

#include <vector>
#include <cstdlib>
#include <iostream>

#include "cpu.h"
#include "file_parser.h"

CPU::CPU(Display& display)
: display(display), I_addr(0), PC(512), SP(15), t_delay(0), t_sound(0), program_active (true)
{
	// Purposely empty.
}

CPU::~CPU()
{
	 //Purposely empty.
}

void CPU::load_rom(const char file_name[])
{
	std::vector<Uint8> rom_data;
	std::vector<Uint8>::iterator it;
	Uint8 *mem = &(this->mem[512]); // ROM loaded into memory at byte 513.

	unsigned int mem_available = 4*KiB - 512; // First 512 bytes are reserved for interpreter.

	initialize_font();
	file_parser fp(file_name);
	rom_data = fp.return_rom_data();
	
	if (rom_data.size() > mem_available)
	{
		cerr << "Rom is too large." << endl;
		exit(1);
	}

	for (it = rom_data.begin(); it < rom_data.end(); it++, mem++)
		*mem = *it;
}

void CPU::run()
{
  /*while(1)
  {
		display.clear();
		draw_sprite();
		display.update();
		I_addr += 5;
  }*/
	
	while (program_active)
	{
		// XXX : Update timers
		cycle();
	}
	
}

void CPU::cycle()
{
	cur_op = (mem[PC] << 8) + mem[PC + 1];
	execute_opcode();
	PC += 2;
	display.update();
}

void CPU::execute_opcode()
{
	int op_1 = (cur_op & 0xF000) >> 12;
	int op_4 = (cur_op & 0x000F);
	int op_3and4 = (cur_op & 0x00FF);

	switch (op_1)
	{
	case 0x0: 
		switch(cur_op)
		{
		case 0x00E0:
			clear_screen();
			break;

		case 0x00EE:
			sub_return();
			break;

		case 0x00FD:
			terminate_program();
			break;

		default:
			// INVALID OPCODE
			break;
		}

		break;

	case 0x1:
		jump();
		break;

	case 0x2:
		sub_call();
		break;

	case 0x3:
		skip_equal_imm();
		break;

	case 0x4:
		skip_not_equal_imm();
		break;

	case 0x5:
		switch (op_4)
		{
		case 0x0:
			skip_equal();
			break;

		default: 
			// INVALID OPCODE
			break;
		}

		break;

	case 0x6:
		set_imm();
		break;

	case 0x7:
		add_imm();
		break;

	case 0x8:
		switch (op_4)
		{
		case 0x0:
			set();
			break;

		case 0x1:
			or_op();
			break;

		case 0x2:
			and_op();
			break;

		case 0x3:
			xor_op();
			break;

		case 0x4:
			add();
			break;

		case 0x5:
			x_sub_y();
			break;

		case 0x6:
			shift_right();
			break;

		case 0x7:
			y_sub_x();
			break;

		case 0xE:
			shift_left();
			break;

		default:
			// INVALID OPCODE
			break;
		}

		break;

	case 0x9:
		switch (op_4)
		{
		case 0x0:
			skip_not_equal();
			break;

		default:
			// INVALID OPCODE
			break;
		}

		break;

	case 0xA:
		set_index();
		break;

	case 0xB:
		jump_offset();
		break;

	case 0xC:
		set_rand();
		break;

	case 0xD:
		draw_sprite();
		break;

	case 0xE:
		switch (op_3and4)
		{
		case 0x9E:
			skip_on_pressed();
			break;

		case 0xA1:
			skip_not_pressed();
			break;

		default:
			// INVALID OPCODE
			break;
		}

		break;

	case 0xF:
		switch (op_3and4)
		{
		case 0x07:
			load_delay_timer();
			break;

		case 0x0A:
			wait_on_press();
			break;

		case 0x15:
			set_delay_timer();
			break;

		case 0x18:
			set_sound_timer();
			break;

		case 0x1E:
			add_to_index();
			break;

		case 0x29:
			set_I_to_char();
			break;

		case 0x33:
			store_BCD();
			break;

		case 0x55:
			save_regs_to_mem();
			break;

		case 0x65:
			load_regs_from_mem();
			break;
		}

		break;

	default:
		// INVALID OPCODE
		break;
	}
}

/*
* Opcode: 00E0
* Explanation: Clears the screen.
*/
void CPU::clear_screen()
{
	display.clear();
}

/*
* Opcode: 00EE
* Explanation: Returns from a subroutine.
*/
void CPU::sub_return()
{
	SP++;
	PC = stack[SP] - OPCODE_SIZE;
}

/*
* Opcode: 00FD
* Explanation: Terminates program.
*/
void CPU::terminate_program()
{
  program_active = false;
}

/*
* Opcode: 1NNN
* Explanation: Jumps to address NNN.
*/
void CPU::jump()
{
	int NNN = get_NNN();

	PC = NNN - OPCODE_SIZE;
}

/*
* Opcode: 2NNN
* Explanation: Calls subroutine at NNN.
*/
void CPU::sub_call()
{
	int NNN = get_NNN();

	stack[SP--] = PC;
	PC = NNN - OPCODE_SIZE;
}

/*
* Opcode: 3XNN
* Explanation: Skips the next instruction if VX equals NN.
*/
void CPU::skip_equal_imm()
{
	int X = get_X();
	int NN = get_NN();

	if (V[X] == NN)
		PC += 2;
}

/*
* Opcode: 4XNN
* Explanation: Skips the next instruction if VX doesn't equal NN.
*/
void CPU::skip_not_equal_imm()
{
	int X = get_X();
	int NN = get_NN();

	if (V[X] != NN)
		PC += 2;
}

/*
* Opcode: 5XY0
* Explanation: Skips the next instruction if VX equals VY.
*/
void CPU::skip_equal()
{
	int X = get_X();
	int Y = get_Y();

	if (V[X] == V[Y])
		PC += 2;
}

/*
* Opcode: 6XNN
* Explanation: Sets VX to NN.
*/
void CPU::set_imm()
{
	int X = get_X();
	int NN = get_NN();

	V[X] = NN;
}

/*
* Opcode: 7XNN
* Explanation: Adds NN to VX
*/
void CPU::add_imm()
{
	int X = get_X();
	int NN = get_NN();

	V[X] += NN;
}

/*
* Opcode: 8XY0
* Explanation: Sets VX to the value of VY
*/
void CPU::set()
{
	int X = get_X();
	int Y = get_Y();

	V[X] = V[Y];
}

/*
* Opcode: 8XY1
* Explanation: Sets VX to (VX or VY).
*/
void CPU::or_op()
{
	int X = get_X();
	int Y = get_Y();

	V[X] = V[X] | V[Y];
}

/*
* Opcode: 8XY2
* Explanation: Sets VX to (VX and VY).
*/
void CPU::and_op()
{
	int X = get_X();
	int Y = get_Y();

	V[X] = V[X] & V[Y];
}

/*
* Opcode: 8XY3
* Explanation: Sets VX to (VX xor VY).
*/
void CPU::xor_op()
{
	int X = get_X();
	int Y = get_Y();

	V[X] = V[X] ^ V[Y];
}

/*
* Opcode: 8XY4
* Explanation: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 
* when there isn't.
*/
void CPU::add()
{
	int X = get_X();
	int Y = get_Y();
	uint8_t sum;
	
	sum = V[X] + V[Y];

	// If the sum is smaller than either addend, overflow has occured.
	if ((sum < V[X]) || (sum < V[Y]))
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[X] = sum;
}

/*
* Opcode: 8XY5
* Explanation: VY is subtracted from VX. VF is set to 0 when there's a borrow, 
* and 1 when there isn't.
*/
void CPU::x_sub_y()
{
	int X = get_X();
	int Y = get_Y();
	
	if (V[X] < V[Y]) 
		V[0xF] = 0;
	else
		V[0xF] = 1;

	V[X] = V[X] - V[Y];
}

/*
* Opcode: 8XY6
* Explanation: Shifts VX right by one. VF is set to the value of the least 
* significant bit of VX before the shift.
*/
void CPU::shift_right()
{
	int X = get_X();
	
	V[0xF] = V[X] & 0x01;
	V[X] = V[X] >> 1;
}

/*
* Opcode: 8XY7
* Explanation: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, 
* and 1 when there isn't.
*/
void CPU::y_sub_x()
{
	int X = get_X();
	int Y = get_Y();

	if (V[Y] < V[X])
		V[0xF] = 0;
	else
		V[0xF] = 1;

	V[X] = V[Y] - V[X];
}

/*
* Opcode: 8XYE
* Explanation: Shifts VX left by one. VF is set to the value of the most 
* significant bit of VX before the shift.
*/
void CPU::shift_left()
{
	int X = get_X();

	V[0xF] = V[X] & 0x80;
	V[X] = V[X] << 1;
}

/*
* Opcode: 9XY0
* Explanation: Skips the next instruction if VX doesn't equal VY.
*/
void CPU::skip_not_equal()
{
	int X = get_X();
	int Y = get_Y();

	if (V[X] != V[Y])
		PC += 2;
}

/*
* Opcode: ANNN
* Explanation: Sets I to the address NNN.
*/
void CPU::set_index()
{
	int NNN = get_NNN();

	I_addr = NNN;
}

/*
* Opcode: BNNN
* Explanation: Jumps to the address NNN plus V0.
*/
void CPU::jump_offset()
{
	int NNN = get_NNN();

	PC = NNN + V[0] - OPCODE_SIZE;
}

/*
* Opcode: CXNN
* Explanation: Sets VX to a random number and NN.
*/
void CPU::set_rand()
{
	int X = get_X();
	int NN = get_NN();

	V[X] = rand() & NN;
}

/*
* Opcode: DXYN
* Explanation: Draws a sprite at coordinate (VX, VY) that has a width of 8 
* pixels and a height of N pixels. As described above, VF is set to a 1 if any
* screen pixels are flipped from set to unset when the sprite is drawn, and to
* 0 if that doesn't happen.
*/
void CPU::draw_sprite()
{
	int i, j;
	Uint8 buf;
	int X = get_X();
	int Y = get_Y();
	int N = get_N();
	int conflict = 0;

	for (i = 0; i < N; i++)
	{
		buf = mem[I_addr + i];

		for (j = 0; j < (int) sizeof(buf)*8; j++)
		{
			if (buf & 0x80)
				conflict |= display.toggle_pixel(X + j, Y + i);

			buf <<= 1;
		}
	}

	V[0xF] = conflict;
}

/*
* Opcode: EX9E
* Explanation: Skips the next instruction if the key stored in VX is pressed.
*/
void CPU::skip_on_pressed()
{
	// XXX: Fill this in.
}

/*
* Opcode: EXA1
* Explanation: Skips the next instruction if the key stored in VX isn't
* pressed.
*/
void CPU::skip_not_pressed()
{
	// XXX: Fill this in.
}

/*
* Opcode: FX07
* Explanation: Sets VX to the value of the delay timer.
*/
void CPU::load_delay_timer()
{
	int X = get_X();

	V[X] = t_delay;
}

/*
* Opcode: FX0A
* Explanation: A key press is awaited, and then stored in VX.
*/
void CPU::wait_on_press()
{
  int X = get_X ();
  V [X] = display.get_key ();
}

/*
* Opcode: FX15
* Explanation: Sets the delay timer to VX.
*/
void CPU::set_delay_timer()
{
	int X = get_X();

	t_delay = V[X];
}

/*
* Opcode: FX18
* Explanation: Sets the sound timer to VX.
*/
void CPU::set_sound_timer()
{
	int X = get_X();
	
	t_sound = V[X];
}

/*
* Opcode: FX1E
* Explanation: Adds VX to I.
*/
void CPU::add_to_index()
{
	int X = get_X();

	I_addr += V[X];
}

/*
* Opcode: FX29
* Explanation: Sets I to the location of the sprite for the character in VX.
* Characters 0-F (in hexadecimal) are represented by a 4x5 font.
*/
void CPU::set_I_to_char()
{
	int X = get_X();
	int character = V[X] * 5; // Each character takes 5 bytes.

	I_addr = START_CHAR_DATA + character;
}

/*
* Opcode: FX33
* Explanation: Stores the Binary-coded decimal representation of VX at the
* addresses I, I plus 1, and I plus 2.
*/
void CPU::store_BCD()
{
	// XXX: Fill this in.
}

/*
* Opcode: FX55
* Explanation: Stores V0 to VX in memory starting at address I.
*/
void CPU::save_regs_to_mem()
{
	int X = get_X();

	for (int i = 0; i <= X; i++)
		mem[I_addr + i] = V[i];
}

/*
* Opcode: FX65
* Explanation: Fills V0 to VX with values from memory starting at address I.
*/
void CPU::load_regs_from_mem()
{
	int X = get_X();

	for (int i = 0; i <= X; i++)
		V[i] = mem[I_addr + i];
}

/*
* Characters must be stored in the interpreter area of Chip-8 memory (0x00 to
* 0x1FF. The variable x is used in this implementation to make it easier to move
* the character data if needed. Each character is 4x5, and requires 5 bytes of 
* memory.
*/
void CPU::initialize_font()
{
	int x = START_CHAR_DATA; 

	// "0"
	mem[x++] = 0xF0;
	mem[x++] = 0x90;
	mem[x++] = 0x90;
	mem[x++] = 0x90;
	mem[x++] = 0xF0;

	// "1"
	mem[x++] = 0x20;
	mem[x++] = 0x60;
	mem[x++] = 0x20;
	mem[x++] = 0x20;
	mem[x++] = 0x70;

	// "2"
	mem[x++] = 0xF0;
	mem[x++] = 0x10;
	mem[x++] = 0xF0;
	mem[x++] = 0x80;
	mem[x++] = 0xF0;

	// "3"
	mem[x++] = 0xF0;
	mem[x++] = 0x10;
	mem[x++] = 0xF0;
	mem[x++] = 0x10;
	mem[x++] = 0xF0;

	// "4"
	mem[x++] = 0x90;
	mem[x++] = 0x90;
	mem[x++] = 0xF0;
	mem[x++] = 0x10;
	mem[x++] = 0x10;

	// "5"
	mem[x++] = 0xF0;
	mem[x++] = 0x80;
	mem[x++] = 0xF0;
	mem[x++] = 0x10;
	mem[x++] = 0xF0;

	// "6"
	mem[x++] = 0xF0;
	mem[x++] = 0x80;
	mem[x++] = 0xF0;
	mem[x++] = 0x90;
	mem[x++] = 0xF0;

	// "7"
	mem[x++] = 0xF0;
	mem[x++] = 0x10;
	mem[x++] = 0x20;
	mem[x++] = 0x40;
	mem[x++] = 0x40;

	// "8"
	mem[x++] = 0xF0;
	mem[x++] = 0x90;
	mem[x++] = 0xF0;
	mem[x++] = 0x90;
	mem[x++] = 0xF0;

	// "9"
	mem[x++] = 0xF0;
	mem[x++] = 0x90;
	mem[x++] = 0xF0;
	mem[x++] = 0X10;
	mem[x++] = 0XF0;

	// "A"
	mem[x++] = 0xF0;
	mem[x++] = 0x90;
	mem[x++] = 0xF0;
	mem[x++] = 0x90;
	mem[x++] = 0x90;

	// "B"
	mem[x++] = 0xE0;
	mem[x++] = 0x90;
	mem[x++] = 0xE0;
	mem[x++] = 0x90;
	mem[x++] = 0xE0;

	// "C"
	mem[x++] = 0xF0;
	mem[x++] = 0x80;
	mem[x++] = 0x80;
	mem[x++] = 0x80;
	mem[x++] = 0xF0;

	// "D"
	mem[x++] = 0xE0;
	mem[x++] = 0x90;
	mem[x++] = 0x90;
	mem[x++] = 0x90;
	mem[x++] = 0xE0;

	// "E"
	mem[x++] = 0xF0;
	mem[x++] = 0x80;
	mem[x++] = 0xF0;
	mem[x++] = 0x80;
	mem[x++] = 0xF0;

	// "F"
	mem[x++] = 0xF0;
	mem[x++] = 0x80;
	mem[x++] = 0xF0;
	mem[x++] = 0x80;
	mem[x++] = 0x80;
}
