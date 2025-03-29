#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "sim.h"
#include "utils/functions.h"
#include "utils/opcodes.h"

void process_instruction() {
  uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
  int instruction_matched = 0;

  for (int index = 0; index < amount_of_instructions; index++) {
      uint32_t extracted_opcode = get_bits(instruction, opcode_table[index].start_bit, 32);
      if (extracted_opcode == opcode_table[index].opcode_mask) {
          opcode_table[index].handler(instruction);
          instruction_matched = 1;
          break;
      }
  }

  if (!instruction_matched) {
      printf("Error: Unrecognized opcode");
      exit(1);
  }

  if(!is_branch_taken){  
    NEXT_STATE.PC += 4;
  }

  is_branch_taken = 0;
  
}