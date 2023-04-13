#ifndef ALU_H_
#define ALU_H_


#include "cpu.h"
#include <math.h>


uint32_t alu(const uint16_t op,
			 const uint32_t a,
			 const uint32_t b,
			 uint8_t sr);


#endif /* ALU_H_ */