#ifndef PROGRAM_SWITCH_TEST_H_
#define PROGRAM_SWITCH_TEST_H_

/* How to use this program:
 *  Connect monitor to REGP, ensure that it counts from 0-63 and then rolls over
 */

const uint16_t clock_program[64] PROGMEM = {
	/* 1  */	BUILD_OPCODE(TEST, REGI, 0x3F),
	/* 2  */	BUILD_OPCODE(CSET, REGS, 0x11),
	/* 3  */	BUILD_OPCODE(TEST, REGI, 0x3E),
	/* 4  */	BUILD_OPCODE(CSET, REGS, 0x12),
	/* 5  */	BUILD_OPCODE(TEST, REGI, 0x3D),
	/* 6  */	BUILD_OPCODE(CSET, REGS, 0x13),
	/* 7  */	BUILD_OPCODE(TEST, REGI, 0x3B),
	/* 8  */	BUILD_OPCODE(CSET, REGS, 0x14),
	/* 9  */	BUILD_OPCODE(TEST, REGI, 0x37),
	/* 10 */	BUILD_OPCODE(CSET, REGS, 0x15),
	/* 11 */	BUILD_NOOP,
	/* 12 */	BUILD_NOOP,
	/* 13 */	BUILD_NOOP,
	/* 14 */	BUILD_NOOP,
	/* 15 */	BUILD_NOOP,
	/* 16 */	BUILD_NOOP,
	
	/* 17 */	BUILD_NOOP,
	/* 18 */	BUILD_NOOP,
	/* 19 */	BUILD_NOOP,
	/* 20 */	BUILD_NOOP,
	/* 21 */	BUILD_NOOP,
	/* 22 */	BUILD_NOOP,
	/* 23 */	BUILD_NOOP,
	/* 24 */	BUILD_NOOP,
	/* 25 */	BUILD_NOOP,
	/* 26 */	BUILD_NOOP,
	/* 27 */	BUILD_NOOP,
	/* 28 */	BUILD_NOOP,
	/* 29 */	BUILD_NOOP,
	/* 30 */	BUILD_NOOP,
	/* 31 */	BUILD_NOOP,
	/* 32 */	BUILD_NOOP,
	
	/* 33 */	BUILD_NOOP,
	/* 34 */	BUILD_NOOP,
	/* 35 */	BUILD_NOOP,
	/* 36 */	BUILD_NOOP,
	/* 37 */	BUILD_NOOP,
	/* 38 */	BUILD_NOOP,
	/* 39 */	BUILD_NOOP,
	/* 40 */	BUILD_NOOP,
	/* 41 */	BUILD_NOOP,
	/* 42 */	BUILD_NOOP,
	/* 43 */	BUILD_NOOP,
	/* 44 */	BUILD_NOOP,
	/* 45 */	BUILD_NOOP,
	/* 46 */	BUILD_NOOP,
	/* 47 */	BUILD_NOOP,
	/* 48 */	BUILD_NOOP,
	
	/* 49 */	BUILD_NOOP,
	/* 50 */	BUILD_NOOP,
	/* 51 */	BUILD_NOOP,
	/* 52 */	BUILD_NOOP,
	/* 53 */	BUILD_NOOP,
	/* 54 */	BUILD_NOOP,
	/* 55 */	BUILD_NOOP,
	/* 56 */	BUILD_NOOP,
	/* 57 */	BUILD_NOOP,
	/* 58 */	BUILD_NOOP,
	/* 59 */	BUILD_NOOP,
	/* 60 */	BUILD_NOOP,

	/* Power on reset */
	/* 61 */	BUILD_NOOP,
	/* 62 */	BUILD_NOOP,
	/* 63 */	BUILD_NOOP,
	/* 64 */	BUILD_NOOP,
};


#else
#error Program header included twice
#endif /* PROGRAM_SWITCH_TEST_H_ */