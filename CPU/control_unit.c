#include "control_unit.h"

static void io_reset(void);
static void io_update(void);
static inline void cpu_registers_clear(void);

static inline void monitor_interrupts(void);
static inline void check_for_interrupt_requests(void);
static inline void generate_interrupt(const uint16_t interrupt_vector);
static inline void return_from_interrupt(void);

static uint16_t pc;
static uint64_t ir;
static uint8_t sr;

static uint16_t op_code;
static uint16_t op1;
static uint32_t op2;

static uint32_t current_state;
static volatile uint32_t pina_last_value;
static uint32_t reg[CPU_REGISTER_ADDRESS_WIDTH];

void control_unit_reset(void)
{
	pc = 0x00;
	ir = 0x00;
	sr = 0x00;

	op_code = 0x00;
	op1 = 0x00;
	op2 = 0x00;

	current_state = CPU_STATE_FETCH;
	pina_last_value = 0x00;

	cpu_registers_clear();
	program_memory_write();
	data_memory_reset();
	stack_reset();
	io_reset();
	return;
}

void control_unit_run(void)
{
	switch(current_state)
	{
	   case CPU_STATE_FETCH:
	   {
		  ir = program_memory_read(pc++);
		  current_state = CPU_STATE_DECODE;
		  break;
	   }	
	   case CPU_STATE_DECODE:
	   {
		   op_code = (uint16_t)(ir >> 48);
		   op1     = (uint16_t)(ir >> 32);
		   op2     = (uint32_t)(ir);
		   current_state = CPU_STATE_EXECUTE;
		   break;
	   }
	   case CPU_STATE_EXECUTE:
	   {
		   switch (op_code)
		   {
			   case NOP:
			   {
				   break;
			   }
			   case LDI:
			   {
				   reg[(uint8_t)(op1)] = op2;
				   break;
			   }
			   case MOV:
			   {
				   reg[(uint8_t)(op1)] = reg[(uint8_t)(op2)];
				   break;
			   }
			   case OUT:
			   {
				   if (op1 == PINA)
				   {
					   const uint32_t porta = data_memory_read(PORTA) ^ reg[(uint8_t)(op2)];
					   data_memory_write(PORTA, porta);
				   }
				   else
				   {
					   data_memory_write(op1, reg[(uint8_t)(op2)]);
				   }
				   break;
			   }
			   case IN:
			   {
				   reg[(uint8_t)(op1)] = data_memory_read((uint8_t)(op2));
				   break;
			   }
			   case ST:
			   {
				   data_memory_write(reg[(uint8_t)(op1)], reg[(uint8_t)(op2)]);
				   break;
			   }
			   case LD:
			   {
				   reg[(uint8_t)(op1)] = data_memory_read(reg[(uint8_t)(op2)]);
				   break;
			   }
			   case CLR:
			   {
				   reg[(uint8_t)(op1)] = 0x00;
				   break;
			   }
			   case LSL:
			   {
				   reg[(uint8_t)(op1)] = reg[(uint8_t)(op1)] << 1;
				   break;
			   }
			   case LSR:
			   {
				   reg[(uint8_t)(op1)] = reg[(uint8_t)(op1)] >> 1;
				   break;
			   }
			   case ORI:
			   {
				   reg[(uint8_t)(op1)] = alu(OR, reg[(uint8_t)(op1)], op2, sr);
				   break;
			   }
			   case ANDI:
			   {
				   reg[(uint8_t)(op1)] = alu(AND, reg[(uint8_t)(op1)], op2, sr);
				   break;
			   }
			   case XORI:
			   {
				   reg[(uint8_t)(op1)] = alu(XOR, reg[(uint8_t)(op1)], op2, sr);
				   break;
			   }
			   case OR:
			   {
				   reg[(uint8_t)(op1)] = alu(OR, reg[(uint8_t)(op1)], reg[(uint8_t)(op2)], sr);
				   break;
			   }
			   case AND:
			   {
				   reg[(uint8_t)(op1)] = alu(AND, reg[(uint8_t)(op1)], reg[(uint8_t)(op2)], sr);
				   break;
			   }
			   case XOR:
			   {
				   reg[(uint8_t)(op1)] = alu(XOR, reg[(uint8_t)(op1)], reg[(uint8_t)(op2)], sr);
				   break;
			   }
			   case ADDI:
			   {
				   reg[(uint8_t)(op1)] = alu(ADD, reg[(uint8_t)(op1)], op2, sr);
				   break;
			   }
			   case SUBI:
			   {
				   reg[(uint8_t)(op1)] = alu(SUB, reg[(uint8_t)(op1)], op2, sr);
				   break;
			   }
			   case ADD:
			   {
				   reg[(uint8_t)(op1)] = alu(ADD, reg[(uint8_t)(op1)], reg[(uint8_t)(op2)], sr);
				   break;
			   }
			   case SUB:
			   {
				   reg[(uint8_t)(op1)] = alu(SUB, reg[(uint8_t)(op1)], reg[(uint8_t)(op2)], sr);
				   break;
			   }
			   case INC:
			   {
				   reg[(uint8_t)(op1)] = alu(ADD, reg[(uint8_t)(op1)], 1, sr);
				   break;
			   }
			   case DEC:
			   {
				   reg[(uint8_t)(op1)] = alu(SUB, reg[(uint8_t)(op1)], 1, sr);
				   break;
			   }
			   case CMPI:
			   {
				   (void)(alu(SUB, reg[(uint8_t)(op1)], op2, sr));
				   break;
			   }
			   case CMP:
			   {
				   (void)(alu(SUB, reg[(uint8_t)(op1)], reg[(uint8_t)(op2)], sr));
				   break;
			   }
			   case CALL:
			   {
				   stack_push(pc);
				   pc = op1;
				   break;
			   }
			   case RET:
			   {
				   pc = stack_pop();
				   break;
			   }
			   case RETI:
			   {
				   return_from_interrupt();
				   break;
			   }
			   case PUSH:
			   {
				   stack_push(reg[(uint8_t)(op1)]);
				   break;
			   }
			   case POP:
			   {
				   reg[(uint8_t)(op1)] = stack_pop();
				   break;
			   }
			   case JMP:
			   {
				   pc = (uint16_t)(op1);
				   break;
			   }
			   case JE:
			   {
				   if (read(sr, Z)) pc = op1;
				   break;
			   }
			   case JNE:
			   {
				   if (!read(sr, Z)) pc = op1;
				   break;
			   }
			   case JGE:
			   {
				   if (!read(sr, S)) pc = op1;
				   break;
			   }
			   case JGT:
			   {
				   if (!read(sr, S) && !read(sr, Z)) pc = op1;
				   break;
			   }
			   case JLE:
			   {
				   if (read(sr, S) || read(sr, Z)) pc = op1;
				   break;
			   }
			   case JLO:
			   {
				   if (read(sr, S)) pc = op1;
				   break;
			   }
			   case SEI:
			   {
				   set(sr, I);
				   break;
			   }
			   case CLI:
			   {
				   clr(sr, I);
				   break;
			   }
			   default:
			   {
				   io_reset();
				   break;
			   }
		   }

		   check_for_interrupt_requests();
		   current_state = CPU_STATE_FETCH;
		   break;
	   }
	   default:
	   {
		   io_reset();
		   break;
	   }
   }

   io_update();
   monitor_interrupts();
   return;
   
}

static void io_reset(void)
{
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x00;

	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	
	return;
}

static void io_update(void)
{
	const uint32_t ddra = data_memory_read(DDRA);
	const uint32_t porta = data_memory_read(PORTA);
	const uint32_t pina = (uint32_t)(((uint32_t)PINC << 16) | ((uint16_t)PINB << 8) | PIND);
	
	data_memory_write(PINA, pina);
	
	DDRB = (uint8_t)(ddra >> 8);
	DDRC = (uint8_t)(ddra >> 16);
	DDRD = (uint8_t)(ddra);

	PORTB = (uint8_t)(porta >> 8);
	PORTC = (uint8_t)(porta >> 16);
	PORTD = (uint8_t)(porta);
	
	return;
}

static inline void cpu_registers_clear(void)
{
	for (uint8_t i = 0; i <  CPU_REGISTER_ADDRESS_WIDTH; ++i)
	{
		reg[0] = 0;
	}
}

static inline void monitor_interrupts(void)
{
	const uint32_t pina = data_memory_read(PINA);

	for (uint8_t i = 0; i < CPU_REGISTER_ADDRESS_WIDTH; ++i)
	{
		if (read(pina, i) != read(pina_last_value, i))
		{
			const uint32_t pcmsk = data_memory_read(PCMSK);

			if (read(pcmsk, i))
			{
				uint32_t ifr = data_memory_read(IFR);
				set(ifr, PCIFA);
				data_memory_write(IFR, ifr);
				break;
			}
		}
	}

	pina_last_value = pina;
	return;
}

static inline void check_for_interrupt_requests(void)
{
	if (read(sr, I))
	{
		uint32_t ifr = data_memory_read(IFR);
		const uint32_t icr = data_memory_read(ICR);

		if (read(ifr, PCIFA) && read(icr, PCIEA))
		{
			clr(ifr, PCIFA);
			data_memory_write(IFR, ifr);
			generate_interrupt(PCINT_vect);
		}
	}
	return;
}
static inline void generate_interrupt(const uint16_t interrupt_vector)
{
	stack_push(pc);
	pc = interrupt_vector;
	clr(sr, I);
	return;
}

static inline void return_from_interrupt(void)
{
	pc = stack_pop();
	set(sr, I);
	return;
}
