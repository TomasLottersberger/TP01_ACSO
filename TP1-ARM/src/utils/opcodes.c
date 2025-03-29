#include "opcodes.h"

const OpcodeHandler opcode_table[] = {
    {adds_extended_register, 0x558, 21},
    {adds_immediate, 0xb1, 24},
    {add_extended_register, 0x458, 21},
    {add_immediate, 0x91, 24},
    {subs_extended_register, 0x758, 21},
    {subs_immediate, 0xf1, 24},
    {hlt, 0x6a2, 21},
    {cmp_extended_register, 0x758, 21},
    {cmp_immediate, 0x7d2, 24},
    {b, 0x5, 26},
    {br, 0x3587C0, 10},
    {b_cond, 0x54, 24},
    {movz, 0x1A5, 23},
    {stur, 0x7C0, 21},
    {sturb, 0x1C0, 21},
    {sturh, 0x3C0, 21},
    {ldur, 0x7C2, 21},
    {ldurb, 0x1C2, 21},
    {ldurh, 0x3C2, 21},
    {ands, 0xEA, 24},
    {eor, 0xCA, 24},
    {orr, 0xAA, 24},
    {mul, 0x4D8, 21},
    {cbz, 0xB4, 24},
    {cbnz, 0xB5, 24},
    {shift, 0x34D, 22},
};

const int amount_of_instructions = sizeof(opcode_table) / sizeof(OpcodeHandler);