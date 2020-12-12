#include <stdint.h>
#include "constants.h"

typedef struct opcode {
    uint8_t op;
    uint8_t x;
    uint8_t y;
    uint8_t kk;
    uint16_t nnn;
    uint8_t n;
} Opcode;

typedef struct cpu{
    uint8_t V[16];
    uint16_t pc;    
    uint16_t I;
    uint8_t sp;
    uint8_t dt;
    uint8_t st;

    Opcode* decoded;

} Cpu;


void CPU_init(Cpu* cpu);

// returns opcode decoded
void CPU_decode(Cpu* cpu, uint16_t opcode);

void CPU_destroy(Cpu* cpu);




