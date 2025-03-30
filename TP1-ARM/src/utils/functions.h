#include <stdint.h>

#ifndef SIM_INSTRUCTIONS_H
#define SIM_INSTRUCTIONS_H

void adds_extended_register(uint32_t instruction);
void adds_immediate(uint32_t instruction);
void add_extended_register(uint32_t instruction);
void add_immediate(uint32_t instruction);
void subs_extended_register(uint32_t instruction);
void subs_immediate(uint32_t instruction);
void hlt(uint32_t instruction);
void cmp_extended_register(uint32_t instruction);
void cmp_immediate(uint32_t instruction);
void ands(uint32_t instruction);
void eor(uint32_t instruction);
void orr(uint32_t instruction);
void b(uint32_t instruction);
void br(uint32_t instruction);
void b_cond(uint32_t instruction);
void movz(uint32_t instruction);
void stur(uint32_t instruction);
void sturb(uint32_t instruction);
void sturh(uint32_t instruction);
void ldur(uint32_t instruction);
void ldurb(uint32_t instruction);
void ldurh(uint32_t instruction);
void mul(uint32_t instruction); 
void cbz(uint32_t instruction); 
void cbnz(uint32_t instruction);
void shift(uint32_t instruction);

extern int is_branch_taken;

int64_t sign_extend(uint32_t number, int bits);
uint32_t get_bits(uint32_t number, int start, int end);
uint32_t negate_number(uint32_t number);

#endif