
#ifndef CONTROL_UNIT_H_
#define CONTROL_UNIT_H_


#include "cpu.h"
#include "data_memory.h"
#include "program_memory.h"
#include "stack.h"
#include "alu.h"

void control_unit_reset(void);
void control_unit_run(void);

#endif /* CONTROL_UNIT_H_ */