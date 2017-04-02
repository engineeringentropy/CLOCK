#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
// This removes the short delay - it's only 16 clock cycles, and I can guarantee
//  that doing the operations will take longer than that!
#define _delay_us(n) {}
void write_register(uint16_t v)
{
	uint8_t i;
	uint8_t b;
	
	/* Because this hardware inverts the output, invert v */
	v = ~v;
	
	/* Reset the port to something known */
	PORTD = 0x02; // PD2 -> 1

	/* Shift the data out one high bit at a time */
	for (i = 0; i < 16; i++) {
		b = (v&0x8000)?1:0;
		v <<= 1;
		PORTD = (PORTD & 0xFB) | (b << 2);
		_delay_us(1);
		PORTD |= 0x10;
		_delay_us(1);
		PORTD &= 0xEF;
		_delay_us(1);
	}

	/* Toggle STCP */
	PORTD |= 0x08;
	_delay_us(1);
	PORTD &= 0xF7;
	_delay_us(1);
}

uint16_t read_register()
{
	uint16_t r = 0;
	uint8_t i;

	/* Configure the register - this is the only operation occurring here, so smashing contents not critical */
	PORTD = 0x02; // PD1->1

	/* First is pulse nPL */
	_delay_us(1);
	PORTD &= 0x0D;
	_delay_us(1);
	PORTD |= 0x2;

	/* Now contents are in shift register (DATA already contains QH!) */
	for (i = 0; i < 16; i++) {
		/* Sample first bit and shift left */
		r <<= 1;
		r |= (PINB & 1);
		/* Now shift shift register */
		_delay_us(1);
		PORTD |= 0x01; // PD0->1
		_delay_us(1);
		PORTD &= 0xFE; // PD0->0
		_delay_us(1);
	}

	/* Invert bits to get right result */
	return ~r;
}
