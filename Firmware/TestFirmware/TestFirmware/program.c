/*
 * program.c
 *
 * Created: 21/03/2017 6:46:05 PM
 *  Author: jport_000
 */ 

/* Layout of the codes:
 *  0-5 immediate (6)
 *  6-8 reg       (3)
 *  9-10 opcode   (2)
 *  15 VALID (if zero, do NOOP)
 *  Before writing to the output register, do: output = (output & 0xF800) | (prog & 0x07FF);
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#define REGO (0x0) /* Output register */
#define REGS (0x1)
#define REGM (0x2)
#define REGH (0x3)
#define REGP (0x4) /* Program Counter */
#define REGI (0x5) /* Input register */
#define REGF (0x7) /* Bus-keeper 0xFF */

#define REGF_VALUE (0x3F) /* The value to check REGF against */

#define NOOP (0x2)
#define CINC (0x1)
#define CSET (0x3)
#define TEST (0x0)

#define BUILD_OPCODE(op, reg, data) ((((uint16_t)data)&0x3F) | ((((uint16_t)reg)&0x7)<<6) | ((((uint16_t)op)&0x3)<<9))
#define BUILD_NOOP (BUILD_OPCODE(NOOP, 0, 0))

#define INPUT_HRS (0x3E) /* 1110 */
#define INPUT_MNS  (0x3D) /* 1101 */
#define INPUT_BLK  (0x3B) /* 1011 */
#define INPUT_60H  (0x37) /* 0111 */

/* Define which program you want here */
#define PROGRAM_MAIN_CLOCK

#if defined(PROGRAM_PC_TEST)
#include "program_pc_test.h"
#elif defined(PROGRAM_PC_BRANCH_TEST)
#include "program_pc_branch_test.h"
#elif defined(PROGRAM_MAIN_CLOCK)
#include "program_main_clock.h"
#elif defined(PROGRAM_SWITCH_TEST)
#include "program_switch_test.h"
#else
#error No valid program specified
#endif