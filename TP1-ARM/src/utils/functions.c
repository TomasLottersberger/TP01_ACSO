#include <stdio.h>
#include <stdint.h>
#include "../shell.h"
#include "functions.h"

int is_branch_taken = 0;

uint32_t get_bits(uint32_t number, int start, int end) {

    if (start < 0 || end < 0 || start > end) {
      return 0;
    }
  
    uint32_t mask = ((1 << (end - start + 1)) - 1) << start;
    uint32_t extracted_bits = number & mask;
  
    return extracted_bits >> start;
}

int64_t sign_extend(uint32_t number, int bits) {
    if ((number >> bits - 1)) {
      number |= 0xFFFFFFFFFFFFFFFF << bits;
    }
  
    return number;
}
  
uint32_t negate_number(uint32_t number){
    uint32_t temp = number;
    int position = 0;
    while (temp) {
        temp >>= 1;
        position++;
    }
    
    number = ~number;
    
    number = number << 32 - position;
    number = number >> 32 - position;
    
    return number;
    
    
}

void adds_extended_register(uint32_t instruction) {

    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
  
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + CURRENT_STATE.REGS[rm];
    NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
    NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
}

void adds_immediate(uint32_t instruction) {

    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t imm12 = get_bits(instruction, 10, 21);
    uint32_t shift = get_bits(instruction, 22, 23);
  
    if (shift){
      imm12 = imm12 << 12;
    }
  
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + imm12;
    NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
    NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
}

void add_extended_register(uint32_t instruction){
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
    
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + CURRENT_STATE.REGS[rm];
}

void add_immediate(uint32_t instruction){
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rd = get_bits(instruction, 0, 4);
    int64_t imm12 = get_bits(instruction, 10, 21);
    uint32_t shift = get_bits(instruction, 22, 23);
  
    if (shift){
      imm12 = imm12 << 12;
    }
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] + imm12;
}

void subs_extended_register(uint32_t instruction) {

    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
  
    int64_t temp = CURRENT_STATE.REGS[rm] - CURRENT_STATE.REGS[rn];
  
    NEXT_STATE.FLAG_N = (temp < 0) ? 1 : 0;
    NEXT_STATE.FLAG_Z = (temp == 0) ? 1 : 0;
  
    if (rd == 31) {
      return;
    }
  
    NEXT_STATE.REGS[rd] = temp;
}

void subs_immediate(uint32_t instruction) {

    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    int64_t imm12 = get_bits(instruction, 10, 21);
    uint32_t shift = get_bits(instruction, 22, 23);
  
    if (shift){
      imm12 = imm12 << 12;
    }
  
    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] - imm12;
    NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[rd] < 0) ? 1 : 0;
    NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[rd] == 0) ? 1 : 0;
}

void hlt(uint32_t instruction) {
    RUN_BIT = 0;
}

void cmp_extended_register(uint32_t instruction) {
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t rm = get_bits(instruction, 16, 20);
  
    int64_t result = CURRENT_STATE.REGS[rn] - CURRENT_STATE.REGS[rm];
    NEXT_STATE.FLAG_N = (result < 0) ? 1 : 0;
    NEXT_STATE.FLAG_Z = (result == 0) ? 1 : 0;
}

void cmp_immediate(uint32_t instruction) {
    uint32_t rn = get_bits(instruction, 5, 9);
    int64_t imm12 = get_bits(instruction, 10, 21);
    uint32_t shift = get_bits(instruction, 22, 23);
  
    if (shift){
      imm12 = imm12 << 12;
    }
  
    int64_t result = CURRENT_STATE.REGS[rn] - imm12;
    NEXT_STATE.FLAG_N = (result < 0) ? 1 : 0;
    NEXT_STATE.FLAG_Z = (result == 0) ? 1 : 0;
}

void ands(uint32_t instruction){
  uint32_t rd = get_bits(instruction, 0, 4); 
  uint32_t rn = get_bits(instruction, 5, 9); 
  uint32_t imm6 = get_bits(instruction, 10, 15); 
  uint32_t rm = get_bits(instruction, 16, 20); 
  uint32_t shift = get_bits(instruction, 22, 23);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rm] & CURRENT_STATE.REGS[rn];
  NEXT_STATE.FLAG_Z = (CURRENT_STATE.REGS[rd] == 0) ? 1 : 0;
}

void eor(uint32_t instruction){
  uint32_t rd = get_bits(instruction, 0, 4); 
  uint32_t rn = get_bits(instruction, 5, 9); 
  uint32_t imm6 = get_bits(instruction, 10, 15); 
  uint32_t rm = get_bits(instruction, 16, 20); 
  uint32_t shift = get_bits(instruction, 22, 23);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rm] ^ CURRENT_STATE.REGS[rn];
}

void orr(uint32_t instruction){
  uint32_t rd = get_bits(instruction, 0, 4); 
  uint32_t rn = get_bits(instruction, 5, 9); 
  uint32_t imm6 = get_bits(instruction, 10, 15); 
  uint32_t rm = get_bits(instruction, 16, 20); 
  uint32_t shift = get_bits(instruction, 22, 23);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rm] | CURRENT_STATE.REGS[rn];
}

void b(uint32_t instruction) {
  uint32_t imm26 = get_bits(instruction, 0, 25); 
  int32_t offset = sign_extend(imm26, 26) << 2;
  
  NEXT_STATE.PC = CURRENT_STATE.PC + offset;
  is_branch_taken = 1;
}


void br(uint32_t instruction) {
  uint32_t rn = get_bits(instruction, 5, 9); 
  uint64_t direction = CURRENT_STATE.REGS[rn];
  NEXT_STATE.PC = direction;
  is_branch_taken = 1;
}

void b_cond(uint32_t instruction){
  uint32_t cond = get_bits(instruction, 0, 3); 
  uint32_t imm19 = get_bits(instruction, 5, 23);
  int32_t offset = sign_extend(imm19, 19) << 2;


  if ((cond == 0 && CURRENT_STATE.FLAG_Z)  ||
     (cond == 1 && !CURRENT_STATE.FLAG_Z) ||
     (cond == 12 && (!CURRENT_STATE.FLAG_Z && !CURRENT_STATE.FLAG_N)) ||
     (cond == 11 && CURRENT_STATE.FLAG_N) || 
     (cond == 10 && !CURRENT_STATE.FLAG_N)||
     (cond == 13 && !(!CURRENT_STATE.FLAG_Z && !CURRENT_STATE.FLAG_N))){

    NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    is_branch_taken = 1;
  } 
}

void stur(uint32_t instruction){
  uint32_t rt = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9);

  uint64_t address = CURRENT_STATE.REGS[rn] + offset;
  uint64_t data = CURRENT_STATE.REGS[rt];

  uint32_t data1 = data & 0xFFFFFFFF;
  uint32_t data2 = (data >> 32) & 0xFFFFFFFF;

  mem_write_32(address, data1);
  mem_write_32(address + 4, data2);
}

void sturb(uint32_t instruction){
  uint32_t rt = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9);

  uint64_t address = CURRENT_STATE.REGS[rn] + offset; 

  uint32_t data = CURRENT_STATE.REGS[rt] & 0xFF;

  uint32_t word = mem_read_32(address);

  word = (word & 0xFFFFFF00) | data;

  mem_write_32(address, word);
}

void sturh(uint32_t instruction){
  uint32_t rt = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9);

  uint64_t address = CURRENT_STATE.REGS[rn] + offset;

  uint32_t data = CURRENT_STATE.REGS[rt] & 0xFFFF;

  uint32_t word = mem_read_32(address);

  word = (word & 0xFFFF0000) | data;

  mem_write_32(address, word); 

}

void ldur(uint32_t instruction){
  uint32_t rt = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9);

  uint64_t address = CURRENT_STATE.REGS[rn] + offset;

  uint64_t data = mem_read_32(address+4); 
  data = data << 32;
  data = data | mem_read_32(address);

  NEXT_STATE.REGS[rt] = data;
}

void ldurb(uint32_t instruction){
  uint32_t rt = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9);

  uint64_t address = CURRENT_STATE.REGS[rn] + offset;

  uint32_t word = mem_read_32(address);
  uint32_t data = word & 0xFF;

  NEXT_STATE.REGS[rt] = data;
}

void ldurh(uint32_t instruction){
  uint32_t rt = get_bits(instruction, 0, 4);
  uint32_t rn = get_bits(instruction, 5, 9);
  int32_t offset = sign_extend(get_bits(instruction, 12, 20), 9);

  uint64_t address = CURRENT_STATE.REGS[rn] + offset;

  uint32_t word = mem_read_32(address);
  uint32_t data = word & 0xFFFF;

  NEXT_STATE.REGS[rt] = data;
}

void movz(uint32_t instruction){
  uint32_t rd = get_bits(instruction, 0, 4);
  uint32_t imm16 = get_bits(instruction, 5, 20);
  
  NEXT_STATE.REGS[rd] = imm16;
}

void mul(uint32_t instruction){
  uint32_t rd = get_bits(instruction, 0, 4); 
  uint32_t rn = get_bits(instruction, 5, 9); 
  uint32_t rm = get_bits(instruction, 16, 20); 
  uint32_t ra = get_bits(instruction, 10, 14); 

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[ra] + (CURRENT_STATE.REGS[rn] * CURRENT_STATE.REGS[rm]); 
}


void cbz(uint32_t instruction){
  uint32_t rt = get_bits(instruction, 0, 4); 
  uint32_t imm19 = get_bits(instruction, 5, 23); 

  int64_t offset = sign_extend(imm19, 19);
  if(!rt){
    NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    is_branch_taken = 1;
  }
}

void cbnz(uint32_t instruction){
  uint32_t rt = get_bits(instruction, 0, 4); 
  uint32_t imm19 = get_bits(instruction, 5, 23); 

  int64_t offset = sign_extend(imm19, 19);
  if(rt){
    NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    is_branch_taken = 1;
  }
}

  void shift(uint32_t instruction){
    uint32_t rd = get_bits(instruction, 0, 4);
    uint32_t rn = get_bits(instruction, 5, 9);
    uint32_t imms = get_bits(instruction, 10, 15); 
    uint32_t immr = get_bits(instruction, 16, 21); 
  
    if(imms == 63){
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] >> immr; 
    }
    else{
      uint32_t result = negate_number(imms);
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] << result;
    }
  }