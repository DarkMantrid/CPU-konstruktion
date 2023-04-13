#ifndef STACK_H_
#define STACK_H_

#include "cpu.h"

#define STACK_ADDRESS_WIDTH 100

void stack_reset(void);
int stack_push(const uint32_t value);
uint32_t stack_pop(void);

 #endif /* STACK_H_ */