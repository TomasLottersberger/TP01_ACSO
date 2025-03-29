#ifndef SIM_H
#define SIM_H

#include <stdint.h>
#include "shell.h"
#include "utils/opcodes.h"
#include "utils/functions.h"

extern int is_branch_taken;

void process_instruction(void);
void update_program_counter(uint32_t current_pc, int is_branch_taken);

uint32_t get_bits(uint32_t instruction, int start_bit, int total_bits);

#endif