#ifndef CPU_H
#define CPU_H

#include "SDL.h"
#include "display.h"

class CPU 
{
public:
	CPU(Display& display);
	~CPU();
	void run();

private:
	static const unsigned int KiB = 1024;
	static const int START_CHAR_DATA = 0x00;

	Display& display;	// Pointer to the display.
	Uint8 V[16];		// Data registers
	Uint8 t_delay;		// Delay Timer
	Uint8 t_sound;		// Sound timer
	Uint16 I_addr;		// Index address: pointer to location in memory.
	Uint16 PC;			// Instruction address register
	Uint16 SP;			// Stack pointer
	Uint16 stack[16];	// Stack: Only used to store return addresses when subroutines are called.
	Uint16 cur_op;		// Current instruction
	Uint8 mem[4 * KiB];	// Memory
	
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
	
	// OPCODES
	void clear_screen();
	void sub_return();
	void jump();
	void sub_call();
	void skip_equal_imm();
	void skip_not_equal_imm();
	void skip_equal();
	void set_imm();
	void add_imm();
	void set();
	void or();
	void and();
	void xor();
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
};

#endif // CPU_H