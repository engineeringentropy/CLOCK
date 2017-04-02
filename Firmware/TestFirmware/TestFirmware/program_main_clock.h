#ifndef PROGRAM_MAIN_CLOCK_H_
#define PROGRAM_MAIN_CLOCK_H_

/* How to use this program:
 *  Connect monitor to REGP, ensure that it counts from 0-10 and then returns to 0s
 */

const uint16_t clock_program[64] PROGMEM = {
	/* Main clock program */
	/* 1  */	BUILD_OPCODE(TEST, REGF, REGF_VALUE), /* If 1==1 */
	/* 2  */	BUILD_OPCODE(CINC, REGS, 0),    /*   REGS++ */
	/* 3  */	BUILD_OPCODE(TEST, REGS, 60),   /* If REGS==60 */
	/* 4  */	BUILD_OPCODE(CSET, REGS, 0),    /*   REGS = 0 */
	/* 5  */	BUILD_OPCODE(CINC, REGM, 0),    /*   REGM++ */
	/* 6  */	BUILD_OPCODE(TEST, REGM, 60),   /* If REGM == 60 */
	/* 7  */	BUILD_OPCODE(CSET, REGM, 0),    /*   REGM = 0 */
	/* 8  */	BUILD_OPCODE(CINC, REGH, 0),    /*   REGH++ */
	/* 9  */	BUILD_OPCODE(TEST, REGH, 13),   /* If REGH == 13 */
	/* 10 */	BUILD_OPCODE(CSET, REGH, 1),    /*   REGH = 1 */
	
	/* Time increment */
	/* Hours */
	/* 11 */	BUILD_OPCODE(TEST, REGI, INPUT_HRS),
	/* 12 */	BUILD_OPCODE(CINC, REGH, 0),
	/* 13 */	BUILD_OPCODE(TEST, REGH, 13),
	/* 14 */	BUILD_OPCODE(CSET, REGH, 1),
	/* Minutes */
	/* 15 */	BUILD_OPCODE(TEST, REGI, INPUT_MNS),
	/* 16 */	BUILD_OPCODE(CINC, REGM, 0),
	/* 17 */	BUILD_OPCODE(TEST, REGM, 60),
	/* 18 */	BUILD_OPCODE(CSET, REGM, 0),
	/* Blank function
	 *  Uses the lower two bits of the REGO, 0->1->2->3->0
	 *  Blank operates when bit 1 is set
	 */
	/* 19 */	BUILD_OPCODE(TEST, REGI, INPUT_BLK),
	/* 20 */	BUILD_OPCODE(CINC, REGO, 0),
	/* 21 */	BUILD_OPCODE(TEST, REGO, 3),
	/* 22 */	BUILD_OPCODE(CSET, REGO, 0),
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
	
	/* Doing timeset a second time means two increments per execution.
	 *  This gives a much faster increment that might keep ahead of the clock!
	 */
	/* 36 */	BUILD_OPCODE(TEST, REGI, INPUT_HRS),
	/* 37 */	BUILD_OPCODE(CINC, REGH, 0),
	/* 38 */	BUILD_OPCODE(TEST, REGH, 13),
	/* 39 */	BUILD_OPCODE(CSET, REGH, 1),
	/* Minutes */
	/* 40 */	BUILD_OPCODE(TEST, REGI, INPUT_MNS),
	/* 41 */	BUILD_OPCODE(CINC, REGM, 0),
	/* 42 */	BUILD_OPCODE(TEST, REGM, 60),
	/* 43 */	BUILD_OPCODE(CSET, REGM, 0),
	/* 44 */	BUILD_NOOP,
	/* 45 */	BUILD_NOOP,
	/* 46 */	BUILD_NOOP,
	/* 47 */	BUILD_NOOP,
	/* 48 */	BUILD_NOOP,
	
	/* 49 */	BUILD_OPCODE(TEST, REGF, REGF_VALUE), /* 50/60Hz change needs to be done in firmware. */
	/* 50 */	BUILD_OPCODE(CSET, REGP, 0),
	/* 51 */	BUILD_NOOP, /* These should be NOOP or housekeeping. No other processing here please. */
	/* 52 */	BUILD_NOOP,
	/* 53 */	BUILD_NOOP,
	/* 54 */	BUILD_NOOP,
	/* 55 */	BUILD_NOOP,
	/* 56 */	BUILD_NOOP,
	/* 57 */	BUILD_NOOP,
	/* 58 */	BUILD_NOOP,
	/* 59 */	BUILD_OPCODE(TEST, REGF, REGF_VALUE),
	/* 60 */	BUILD_OPCODE(CSET, REGP, 0),

	/* Power on reset */
	/* 61 */	BUILD_OPCODE(TEST, REGF, REGF_VALUE),
	/* 62 */	BUILD_OPCODE(CSET, REGO, 0),
	/* 63 */	BUILD_OPCODE(CSET, REGM, 0),
	/* 64 */	BUILD_OPCODE(CSET, REGH, 12),
};


#else
#error Program header included twice
#endif /* PROGRAM_MAIN_CLOCK_H_ */