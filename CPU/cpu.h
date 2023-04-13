/********************************************************************************
* cpu.h: Contains miscellaneous definitions and declarations for implementation
*        of an 8-bit processor based on AVR:s instruction list.
********************************************************************************/
#ifndef CPU_H_
#define CPU_H_

/* Include directives: */
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Include directives: */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define NOP   0x00
#define LDI   0x01
#define MOV   0x02
#define OUT   0x03
#define IN    0x04
#define ST    0x05
#define LD    0x06
#define CLR   0x07
#define LSL   0x08
#define LSR   0x09
#define ORI   0x0A
#define ANDI  0x0B
#define XORI  0x0C
#define OR    0x0D
#define AND   0x0E
#define XOR   0x0F
#define ADDI  0x10
#define SUBI  0x11
#define ADD   0x12
#define SUB   0x13
#define INC   0x14
#define DEC   0x15
#define CMPI  0x16
#define CMP   0x17
#define PUSH  0x18
#define POP   0x19
#define CALL  0x1A
#define RET   0x1B
#define RETI  0x1C
#define JMP   0x1D
#define JE    0x1E
#define JNE   0x1F

#define JGE   0x20
#define JGT   0x21
#define JLE   0x22
#define JLO   0x23
#define SEI   0x24
#define CLI   0x25

#define DDRA   0x00
#define PORTA  0x01
#define PINA   0x02
#define ICR    0x03
#define IFR    0x04
#define PCMSK  0x05



#define PCIEA  0
#define PCIFA  0

#define R0   0x00
#define R1   0x01
#define R2   0x02
#define R3   0x03
#define R4   0x04
#define R5   0x05
#define R6   0x06
#define R7   0x07
#define R8   0x08
#define R9   0x09
#define R10  0x0A
#define R11  0x0B
#define R12  0x0C
#define R13  0x0D
#define R14  0x0E
#define R15  0x0F
#define R16  0x10
#define R17  0x11
#define R18  0x12
#define R19  0x13
#define R20  0x14
#define R21  0x15
#define R22  0x16
#define R23  0x17
#define R24  0x18
#define R25  0x19
#define R26  0x1A
#define R27  0x1B
#define R28  0x1C
#define R29  0x1D
#define R30  0x1E
#define R31  0x1F

#define RESET_vect  0x00
#define PCINT_vect  0x02

#define PORTA0   0x00
#define PORTA1   0x01
#define PORTA2   0x02
#define PORTA3   0x03
#define PORTA4   0x04
#define PORTA5   0x05
#define PORTA6   0x06
#define PORTA7   0x07
#define PORTA8   0x08
#define PORTA9   0x09
#define PORTA10  0x0A
#define PORTA11  0x0B
#define PORTA12  0x0C
#define PORTA13  0x0D
#define PORTA14  0x0E
#define PORTA15  0x0F
#define PORTA16  0x10
#define PORTA17  0x11
#define PORTA18  0x12
#define PORTA19  0x13
#define PORTA20  0x14
#define PORTA21  0x15
#define PORTA22  0x16
#define PORTA23  0x17
#define PORTA24  0x18
#define PORTA25  0x19
#define PORTA26  0x1A
#define PORTA27  0x1B
#define PORTA28  0x1C
#define PORTA29  0x1D
#define PORTA30  0x1E
#define PORTA31  0x1F

#define CPU_REGISTER_ADDRESS_WIDTH 32 /* 32 CPU registers in control unit. */
#define CPU_REGISTER_DATA_WIDTH    8  /* 8 bit data width per CPU register. */
#define IO_REGISTER_DATA_WIDTH     8  /* 8 bit data width per I/O location. */

#define I 5 /* Interrupt flag in status register. */
#define S 4 /* Signed flag in status register. */
#define N 3 /* Negative flag in status register. */
#define Z 2 /* Zero flag in status register. */
#define V 1 /* Overflow flag in status register. */
#define C 0 /* Carry flag in status register. */

/********************************************************************************
* set: Sets bit in specified register without affecting other bits.
*
*      - reg: Reference to the register.
*      - bit: The bit to be set in the referenced register.
********************************************************************************/
#define set(reg, bit)  reg |= (1 << bit)

/********************************************************************************
* clr: Clears bit in specified register without affecting other bits.
*
*      - reg: Reference to the register.
*      - bit: The bit to be cleared in the referenced register.
********************************************************************************/
#define clr(reg, bit)  reg &= ~(1 << (bit))

/********************************************************************************
* read: Reads bit from specified register. The return value is 1 is the bit is
*       high, otherwise 0 if the bit is low.
*
*       - reg: Reference to the register.
*       - bit: The bit to be read in the referenced register.
********************************************************************************/
static inline bool read(const uint16_t reg, const uint32_t bit)
{
	return (bool)(reg & (1 << bit));
}

/********************************************************************************
* cpu_state: Enumeration for the different states of the CPU instructio cycle.
********************************************************************************/
enum cpu_state
{
	CPU_STATE_FETCH,  /* Fetches next instruction from program memory. */
	CPU_STATE_DECODE, /* Decodes the fetched instruction. */
	CPU_STATE_EXECUTE /* Executes the decoded instruction. */
};

#endif /* CPU_H_ */
