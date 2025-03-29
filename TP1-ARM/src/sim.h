#ifndef SIM_H
#define SIM_H

#include <stdint.h>
#include "shell.h"       // Para las definiciones de CPU_State, mem_read_32, etc.
#include "utils/opcodes.h"     // Para OpcodeHandler y num_instructions
#include "utils/functions.h"

// Declaración de la variable global que indica si se tomó un salto
extern int is_branch_taken;

// Prototipos de funciones públicas
void process_instruction(void);
void update_program_counter(uint32_t current_pc, int is_branch_taken);

// Función auxiliar para extraer bits de una instrucción (si se implementa en sim.c)
uint32_t get_bits(uint32_t instruction, int start_bit, int total_bits);

#endif