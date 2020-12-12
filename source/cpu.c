#include "cpu.h"
#include <stdlib.h>


void CPU_init(Cpu* cpu) {
    // Initializing the registers
    cpu->I = 0x0000;
    cpu->st = 0x00;
    cpu->pc = 0x0200;
    for (unsigned int i =0; i < 16; ++i)
        cpu->V[i] = 0x00;

    cpu->dt = 0x00;
    cpu->sp = 0x00;

    cpu->decoded = (Opcode*)malloc(sizeof(Opcode));
}


void CPU_decode(Cpu* cpu, uint16_t opcode) {
    Opcode* decoded = cpu->decoded;

    decoded->op = (opcode & 0xf000) >> 8;

    decoded->x = (opcode & 0x0f00) >> 8;

    decoded->y = (opcode & 0x00f0) >> 4;

    decoded->kk = opcode & 0x00ff;

    decoded->nnn = opcode & 0x0fff;

    decoded->n = opcode & 0x000f;
}

void CPU_destroy(Cpu* cpu) {
    free(cpu->decoded);
}