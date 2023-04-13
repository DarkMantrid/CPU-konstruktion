#include "alu.h"

uint32_t alu(const uint16_t op,
			 const uint32_t a,
			 const uint32_t b,
			 uint8_t sr)
{
	uint64_t result = 0x00;
	sr &= ~((1 << S) | (1 << N) | (1 << Z) | (1 << V) | (1 << C));

	switch(op)
	{
		case OR:
		{
			result = a | b;
			break;
		}
		case AND:
		{
			result = a & b;
			break;
		}
		case XOR:
		{
			result = a ^ b;
			break;
		}
		case ADD:
		{
			result = a + b;

			if ((read(a, 31) == read(b, 31)) && (read(result, 31) != read(a, 31)))
			{
				set(sr, V);
			}

			break;
		}
		case SUB:
		{
			result = a + (pow(2, 32) - b);

			if ((read(a, 31) == read(b, 31)) && (read(result, 31) != read(a, 31)))
			{
				set(sr, V);
			}

			break;
		}
	}

	if (read(result, 32))           set(sr, C);
	if (read(result, 31))           set(sr, N);
	if ((uint32_t)(result) == 0)      set(sr, Z);
	if (read(sr, N) != read(sr, V)) set(sr, S);

	return result;
}