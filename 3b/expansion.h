#ifndef EXP_H
#define EXP_H

extern const unsigned simple_numbers[]; 

#include "library.h"

ErrorCode table_expansion(Table**);
unsigned new_capacity(unsigned);

#endif