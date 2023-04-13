#ifndef DATA_MEMORY_H_
#define DATA_MEMORY_H_

#include "cpu.h"


#define DATA_MEMORY_ADDRESS_WIDTH 100
#define DATA_MEMORY_DATA_WIDTH    64

/********************************************************************************
* data_memory_reset: Nollställer dataminnet.
********************************************************************************/
void data_memory_reset(void);

/********************************************************************************
* data_memory_write: Writes an 32-bit value to specified address in data memory.
*                    The value 0 is returned after successful write. Otherwise
*                    if invalid address is specified, no write is done and
*                    error code 1 is returned.
*
*                    - address: Write location in data memory.
*                    - value  : The 32-bit value to write to data memory.
********************************************************************************/
int data_memory_write(const uint32_t address,
					  const uint32_t value);

/********************************************************************************
* data_memory_read: Returns content from specified read location in data memory.
*                   If an invalid address is specified, the value 0 is returned.
*
*                   - address: Read location in data memory.
********************************************************************************/
int data_memory_read(const uint32_t address);

#endif /* DATA_MEMORY_H_ */