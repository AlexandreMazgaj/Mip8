#include <stdint.h>
#include "cpu.h"
#include "input.h"
#include "constants.h"

typedef struct chip8 {
    uint8_t ram[MEMORY_SIZE];
    uint16_t stack[STACK_SIZE];

    uint8_t graphics[DISPLAY_HEIGHT][DISPLAY_WIDTH];

    Input* input;

    Cpu* cpu;

    uint8_t draw_flag;

    uint8_t isQuitting;

} Chip8;


void CHIP8_init(Chip8* chip);

void CHIP8_destroy(Chip8* chip);


void CHIP8_loadRom(Chip8* chip, char* path);


void CHIP8_clock(Chip8* chip);


void exe00E0(Chip8* chip); void exe00EE(Chip8* chip); void exe1nnn(Chip8* chip); void exe2nnn(Chip8* chip);
void exe3xkk(Chip8* chip); void exe4xkk(Chip8* chip); void exe5xy0(Chip8* chip); void exe6xkk(Chip8* chip);
void exe7xkk(Chip8* chip); void exe8xy0(Chip8* chip); void exe8xy1(Chip8* chip); void exe8xy2(Chip8* chip);
void exe8xy3(Chip8* chip); void exe8xy4(Chip8* chip); void exe8xy5(Chip8* chip); void exe8xy6(Chip8* chip);
void exe8xy7(Chip8* chip); void exe8xyE(Chip8* chip); void exe9xy0(Chip8* chip); void exeAnnn(Chip8* chip);
void exeBnnn(Chip8* chip); void exeCxkk(Chip8* chip); void exeDxyn(Chip8* chip); void exeEx9E(Chip8* chip);
void exeExA1(Chip8* chip); void exeFx07(Chip8* chip); void exeFx0A(Chip8* chip); void exeFx15(Chip8* chip);
void exeFx18(Chip8* chip); void exeFx1E(Chip8* chip); void exeFx29(Chip8* chip); void exeFx33(Chip8* chip);
void exeFx55(Chip8* chip); void exeFx65(Chip8* chip); void exeXXXX(Chip8* chip);
