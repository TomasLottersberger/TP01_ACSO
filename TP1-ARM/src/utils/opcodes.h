#ifndef OPCODES_H
#define OPCODES_H
#include "functions.h"

typedef struct {
    void (*handler)(uint32_t);
    uint32_t opcode_mask;
    int start_bit;
} OpcodeHandler;

extern const OpcodeHandler opcode_table[];
extern const int amount_of_instructions;

#endif