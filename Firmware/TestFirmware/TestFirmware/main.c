/*
 * TestFirmware.c
 *
 * Created: 19/03/2017 6:06:50 PM
 * Author : James
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

/* Mappings:
 *  74HC595:
 *   PD2->DATA
 *   PD3->STCP
 *   PD4->CP 
 *  74HC165:
 *   PB0->DATA
 *   PD0->CP
 *   PD1->/PL
 *
 *  Register inputs 
 *          0-5->PC0-5
 *          12->SW2
 *          13->SW1
 *			14->CLK
 *			15->/CLK (note that these aren't equilength!)
 *
 *  Register outputs
 *          0-5->DATA0-5
 *          6-8->REG0-2
 *          9-10->OP0-1
 *          12->Scope trigger (for debugging)
 *          13->POR reset (hopefully, to PC==60)
 *			14->CLK Phase 1
 *			15->CLK Phase 2
 
 */

#define SET_INST(i) ((((uint16_t)i)&0x3)<<9)
#define SET_REG(r)  ((((uint16_t)r)&0x7)<<6)
#define SET_DATA(d) ((((uint16_t)d)&0x3F)<<0)
#define SET_nPOR(a)  ((((uint16_t)a)&1)<<13)
#define SET_CLK0(c) ((((uint16_t)c)&1)<<14)
#define SET_CLK1(c) ((((uint16_t)c)&1)<<15)
#define SET_SCOPE(c) ((((uint16_t)c)&1)<<12)

#define GET_PC(data) (data&0x003F)
/* Note, test for not 0 */
#define GET_SW1(data) (data&0x2000)
#define GET_SW2(data) (data&0x1000)
#define GET_CLK(data) (data&0x4000)
#define GET_nCLK(data) (data&0x8000)

/* Found in shiftreg.c */
extern void write_register(uint16_t);
extern uint16_t read_register(void);

void set_leds(uint8_t l)
{
	PORTB = (PORTB & 0xE1) | ((l<<1)&0x1E);
}

void init()
{
	DDRB = 0xFE;
	DDRD = 0xFF;

	PORTB = 0; /* Also turns off any pullups on PB0 */
	PORTD = 0x02;
	
	/* Configure the serial port */
	
}

void do_system_por_reset()
{
	uint8_t i;
	uint16_t data;
	/* Hold POR high for a bit and pulse the clock a few times */
	data = SET_nPOR(0) | SET_CLK0(0) | SET_CLK1(0) | SET_DATA(0) | SET_REG(0);
	write_register(data);
	/* Pulse clock and wait for a few ms between clocks 
	 * Set the Z flag to one at startup by including that in the instruction field.
	 */
	for (i = 0; i < 200; i++) {
		_delay_loop_2(200);
		data = SET_nPOR(0) | SET_CLK0(1) | SET_CLK1(0) | SET_DATA(0x3F) | SET_REG(7) | SET_INST(0);
		write_register(data);
		_delay_loop_2(200);
		data = SET_nPOR(0) | SET_CLK0(0) | SET_CLK1(1) | SET_DATA(0x3F) | SET_REG(7) | SET_INST(0);
		write_register(data);
	}
	
	/* Reset POR to 0 */
	data = SET_nPOR(1) | SET_CLK0(0) | SET_CLK1(0) | SET_DATA(0) | SET_REG(0);
	write_register(data);
}

enum state_e {
	STATE_START = 0,
	STATE_CLK0 = 1,
	STATE_CLK0_DONE = 2,
	STATE_CLK1 = 3,
	STATE_CLK1_DONE = 4,
	STATE_DUMMY0 = 5,
	STATE_DUMMY1 = 6,
};

enum clk_driver_e {
	CLK_DRV_INT,
	CLK_DRV_EXT,
};

void crash(uint8_t pattern)
{
	/* Crash! */
	uint8_t p2 = pattern;
	while(1) {
		set_leds(p2);
		_delay_ms(50);
		p2 ^= pattern;
		set_leds(p2);
		_delay_ms(50);
	}
}

/* Set the outputs according to the program counter input
 * Full input vector is passed to allow more complex logic
 */
extern uint16_t clock_program[64];
uint16_t do_program_memory(uint16_t output, uint16_t data)
{
	uint8_t pc = data & 0x3F;
	uint16_t word = pgm_read_word_near(clock_program + pc);
	output = (output&0xF800) | (word & 0x07FF);
	write_register(output);
	return output;
}

uint8_t is_clk_1(uint16_t data, enum clk_driver_e clk_drv)
{
	if (clk_drv == CLK_DRV_INT) {
		return (GET_CLK(data))?1:0;
	} else {
		/* Rely on SW2 to determine the current state of the clk */
		return (GET_SW2(data))?1:0;
	}
	
	return 0;
}

uint8_t is_clk_2(uint16_t data, enum clk_driver_e clk_drv)
{
	if (clk_drv == CLK_DRV_INT) {
		return (GET_nCLK(data))?1:0;
	} else {
		/* Rely on SW2 to determine the current state of the clk */
		return (GET_SW2(data))?0:1;
	}	
	
	return 0;
}

int main()
{
	uint16_t data;
	uint16_t output;
	enum clk_driver_e clk_drv;
	enum state_e state;
	
	init();
	do_system_por_reset();	
	
	/* Output is the current output state -
	 *   we must save it as we can't read it back later
	 */
	output = SET_nPOR(1);
	state = STATE_START;
	while(1) {
		data = read_register();
		set_leds((data>>2)&0xF);
		if ((data&0x3F)==60) {
			output |= SET_SCOPE(1);
		} else {
			output &= ~SET_SCOPE(1);
		}
		
		if (GET_SW1(data)) {
			clk_drv = CLK_DRV_INT;
		} else {
			clk_drv = CLK_DRV_EXT;
		}
		
		switch(state) {
			case STATE_START:
				// This state occurs just once after POR - address should be 60 (0x3C) or crash
				if (GET_PC(data) != 0x3C) {
					crash(0x1);
				}
				
				// Now we wait until we see CLK appear and go there!
				if (is_clk_1(data, clk_drv)) {
					state = STATE_CLK0;
				}
				
				break;
				
			case STATE_CLK0:
				// This state is where we've set all the outputs we need to,
				//  and we're waiting to see the next clock pulse
				if (is_clk_1(data, clk_drv)) {
					output |= SET_CLK0(1);
					write_register(output);
					_delay_loop_2(200);
					output &= ~SET_CLK0(1);
					write_register(output);
					state = STATE_CLK0_DONE;
				}
				
				break;
				
			case STATE_CLK0_DONE:
				// Register is read above
				// Do the program memory thing
				output = do_program_memory(output, data);
				state = STATE_CLK1;
				
				break;
				
			case STATE_CLK1:
				// This state is where we've set all the outputs we need to,
				//  and we're waiting to see the next clock pulse
				if (is_clk_2(data, clk_drv)) {
					output |= SET_CLK1(1);
					write_register(output);
					_delay_loop_2(200);
					output &= ~SET_CLK1(1);
					write_register(output);
					state = STATE_CLK1_DONE;
				}
				
				break;
				
			case STATE_CLK1_DONE:
				// Register is read above
				// Do the program memory thing
				output = do_program_memory(output, data);
				state = STATE_DUMMY0;
				
				break;
			
			/* These two states are needed as the incoming clock is 100Hz, not 50Hz */
			case STATE_DUMMY0:
				if (is_clk_1(data, clk_drv)) {
					state = STATE_DUMMY1;
				}
				
				break;
				
			case STATE_DUMMY1:
				if (is_clk_2(data, clk_drv)) {
					state = STATE_CLK0;
				}
				
				break;
			
			default:
				// No return;
				crash(0xF);
				break;
		}
	}

	return 0;
}