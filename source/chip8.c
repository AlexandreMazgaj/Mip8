#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void CHIP8_init(Chip8* chip) {

    uint8_t chip8_fontset[80] =
    { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // initialize the random number generator for 
    // a specific opcode
    srand((time(NULL)));

    // initialize the memory
    for (unsigned int i = 0; i < MEMORY_SIZE; ++i)
        chip->ram[i] = 0x00;

    // load the fontset in the memory
    for (unsigned int i = 0; i < 80; i++) {
        chip->ram[i] = chip8_fontset[i];
    }

    // initialize the pixel array
    for (unsigned int i = 0; i < DISPLAY_HEIGHT; i++) {
        for (unsigned int j = 0; j < DISPLAY_WIDTH; j++)
            chip->graphics[i][j] = 0;
    }

    chip->cpu = (Cpu*)malloc(sizeof(Cpu));

    chip->input = (Input*)malloc(sizeof(Input));

    CPU_init(chip->cpu);

    INPUT_init(chip->input);

    chip->draw_flag = 0;
    chip->isQuitting = 0;
}


void CHIP8_destroy(Chip8* chip) {
    CPU_destroy(chip->cpu);
    free(chip->cpu);
    free(chip->input);
}



void CHIP8_loadRom(Chip8* chip, char* path) {
    FILE *romPtr;

    romPtr = fopen(path, "rb"); // b is for binary

    if (romPtr == NULL)
        printf("Could not open the file\n");

    fseek(romPtr, 0, SEEK_END);

    size_t size = ftell(romPtr);

    fseek(romPtr, 0L, SEEK_SET);

    for (unsigned int i = 0; i < size; ++i) {
        uint8_t c = fgetc(romPtr);
        chip->ram[0x0200 + i] = c;
        // printf("%X\n", c);
    }

    fclose(romPtr);
}



void CHIP8_clock(Chip8* chip) {
    Cpu* cpu = chip->cpu;

    uint16_t opcode = chip->ram[cpu->pc] << 8 | chip->ram[cpu->pc + 1];
 
    CPU_decode(cpu, opcode);

    printf("executing: 0x%X\n", opcode);

    // printf("pc = 0x%X\n", chip->cpu->pc);

    // printf("I -> 0x%X\n", cpu->I);
    // for (int i = 0; i < 16; i++) {
    //     printf("V[%X] -> 0x%X\n", i, cpu->V[i]);
    // }

    // printf("\n--------------------------------\n\n");

    cpu->pc +=2;

    switch(cpu->decoded->op) {
        case 0x00:
            switch(cpu->decoded->kk){
                case 0xe0: exe00E0(chip); break;
                case 0xee: exe00EE(chip); break;
                default: exeXXXX(chip);   break;
            }
            break;
        case 0x10: exe1nnn(chip); break;
        case 0x20: exe2nnn(chip); break;
        case 0x30: exe3xkk(chip); break;
        case 0x40: exe4xkk(chip); break;
        case 0x50: exe5xy0(chip); break;
        case 0x60: exe6xkk(chip); break;
        case 0x70: exe7xkk(chip); break;
        case 0x80:
            switch(cpu->decoded->n) {
                case 0x00: exe8xy0(chip); break;
                case 0x01: exe8xy1(chip); break;
                case 0x02: exe8xy2(chip); break;
                case 0x03: exe8xy3(chip); break;
                case 0x04: exe8xy4(chip); break;
                case 0x05: exe8xy5(chip); break;
                case 0x06: exe8xy6(chip); break;
                case 0x07: exe8xy7(chip); break;
                case 0x0e: exe8xyE(chip); break;
                default: exeXXXX(chip);   break;
            }
            break;
        case 0x90: exe9xy0(chip); break;
        case 0xA0: exeAnnn(chip); break;
        case 0xB0: exeBnnn(chip); break;
        case 0xC0: exeCxkk(chip); break;
        case 0xD0: exeDxyn(chip); break;
        case 0xE0:
            switch (cpu->decoded->kk)
            {
                case 0x9e: exeEx9E(chip); break;
                case 0xA1: exeExA1(chip); break;
                default: exeXXXX(chip);   break;
            }
            break;
        case 0xF0:
            switch(cpu->decoded->kk) {
                case 0x07: exeFx07(chip); break;
                case 0x0A: exeFx0A(chip); break;
                case 0x15: exeFx15(chip); break;
                case 0x18: exeFx18(chip); break;
                case 0x1e: exeFx1E(chip); break;
                case 0x29: exeFx29(chip); break;
                case 0x33: exeFx33(chip); break;
                case 0x55: exeFx55(chip); break;
                case 0x65: exeFx65(chip); break;
                default: exeXXXX(chip);   break;
            }
            break;
        default: exeXXXX(chip); break;
    }

    // Update timers
  if(cpu->dt > 0)
    --cpu->dt;
 
  if(cpu->st > 0) {
    if(cpu->st == 1)
    //   printf("BEEP!\n");
    --cpu->st;
  }
}






// ------------------------------------------------------------
//                    OPCODES INSTRUCTIONS
// ------------------------------------------------------------

/*
 00E0 - CLS
Clear the display.
 */
void exe00E0(Chip8* chip) {
    chip->draw_flag = 1;
    for (unsigned int i = 0; i < DISPLAY_HEIGHT; ++i) {
        for (unsigned int j = 0; j < DISPLAY_WIDTH; ++j)
            chip->graphics[i][j] = 0;
    }
}

/*
00EE - RET
Return from a subroutine.
*/
void exe00EE(Chip8* chip) {
    Cpu* cpu = chip->cpu;

    cpu->pc = chip->stack[cpu->sp];
    cpu->sp--;
}

/*
1nnn - JP addr
Jump to location nnn.
*/
void exe1nnn(Chip8* chip) {
    Cpu* cpu = chip->cpu;

    cpu->pc = cpu->decoded->nnn;
}

/*
2nnn - CALL addr
Call subroutine at nnn.
*/
void exe2nnn(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->sp++;
    chip->stack[cpu->sp] = cpu->pc;
    cpu->pc = cpu->decoded->nnn;
}

/*
3xkk - SE Vx, byte
Skip next instruction if Vx = kk.
*/
void exe3xkk(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    if (cpu->V[cpu->decoded->x] == cpu->decoded->kk)
        cpu->pc += 2;
}

/*
4xkk - SNE Vx, byte
Skip next instruction if Vx != kk.
*/
void exe4xkk(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    if (cpu->V[cpu->decoded->x] != cpu->decoded->kk)
        cpu->pc += 2;
}

/*
5xy0 - SE Vx, Vy
Skip next instruction if Vx = Vy
*/
void exe5xy0(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    if (cpu->V[cpu->decoded->x] == cpu->V[cpu->decoded->y])
        cpu->pc += 2;
}

/*
6xkk - LD Vx, byte
Set Vx = kk.
*/
void exe6xkk(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->V[cpu->decoded->x] = cpu->decoded->kk;
}

/*
7xkk - ADD Vx, byte
Set Vx = Vx + kk.
*/
void exe7xkk(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->V[cpu->decoded->x] += cpu->decoded->kk;
}

/*
8xy0 - LD Vx, Vy
Set Vx = Vy.
*/
void exe8xy0(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->V[cpu->decoded->x] = cpu->V[cpu->decoded->y];
}

/*
8xy1 - OR Vx, Vy
Set Vx = Vx OR Vy.
*/
void exe8xy1(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->V[cpu->decoded->x] |= cpu->V[cpu->decoded->y];
}

/*
8xy2 - AND Vx, Vy
Set Vx = Vx AND Vy.
*/
void exe8xy2(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->V[cpu->decoded->x] &= cpu->V[cpu->decoded->y];
}

/*
8xy3 - XOR Vx, Vy
Set Vx = Vx XOR Vy.
*/
void exe8xy3(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->V[cpu->decoded->x] ^= cpu->V[cpu->decoded->y];
}

/*
8xy4 - ADD Vx, Vy
Set Vx = Vx + Vy, set VF = carry.
*/
void exe8xy4(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    uint16_t temp = cpu->V[cpu->decoded->x] + cpu->V[cpu->decoded->y];

    if (temp > 255)
        cpu->V[0xf] = 1;
    else
        cpu->V[0xf] = 0;

    cpu->V[cpu->decoded->x] = temp & 0x00ff;
}

/*
8xy5 - SUB Vx, Vy
Set Vx = Vx - Vy, set VF = NOT borrow.
*/
void exe8xy5(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    if (cpu->V[cpu->decoded->x] > cpu->V[cpu->decoded->y])
        cpu->V[0xf] = 1;
    else
        cpu->V[0xf] = 0;
    cpu->V[cpu->decoded->x] -= cpu->V[cpu->decoded->y];
}

/*
8xy6 - SHR Vx {, Vy}
Set Vx = Vx SHR 1.
*/
void exe8xy6(Chip8* chip) {
    Cpu* cpu = chip->cpu;

    if (cpu->V[cpu->decoded->x] & 0x01 == 0x01)
        cpu->V[0xf] = 1;

    cpu->V[cpu->decoded->x] >>= 1;
}

/*
8xy7 - SUBN Vx, Vy
Set Vx = Vy - Vx, set VF = NOT borrow.
*/
void exe8xy7(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    if (cpu->V[cpu->decoded->y] > cpu->V[cpu->decoded->x])
        cpu->V[0xf] = 1;
    else
        cpu->V[0xf] = 0;
    cpu->V[cpu->decoded->x] = cpu->V[cpu->decoded->y] - cpu->V[cpu->decoded->x];
}

/*
8xyE - SHL Vx {, Vy}
Set Vx = Vx SHL 1.
*/
void exe8xyE(Chip8* chip) {
    Cpu* cpu = chip->cpu;

    if (cpu->V[cpu->decoded->x] & 0x80 == 0x80)
        cpu->V[0xf] = 1;

    cpu->V[cpu->decoded->x] <<= 1;
}

/*
9xy0 - SNE Vx, Vy
Skip next instruction if Vx != Vy.
*/
void exe9xy0(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    if (cpu->V[cpu->decoded->x] != cpu->V[cpu->decoded->y])
        cpu->pc += 2;
}

/*
Annn - LD I, addr
Set I = nnn.
*/
void exeAnnn(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->I = cpu->decoded->nnn;
}

/*
Bnnn - JP V0, addr
Jump to location nnn + V0.
*/
void exeBnnn(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->pc += cpu->decoded->nnn + cpu->V[0];
}

/*
Cxkk - RND Vx, byte
Set Vx = random byte AND kk.
*/
void exeCxkk(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->V[cpu->decoded->x] = (uint8_t)(rand()%256) & cpu->decoded->kk;
}

/*
Dxyn - DRW Vx, Vy, nibble
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

The interpreter reads n bytes from memory, starting at the address stored in I.
These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
Sprites are XORed onto the existing screen.
If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
If the sprite is positioned so part of it is outside the coordinates of the display,
it wraps around to the opposite side of the screen.
*/
void exeDxyn(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    uint8_t width = 8;
    uint8_t height = cpu->decoded->n;

    uint8_t x = cpu->V[cpu->decoded->x];
    uint8_t y = cpu->V[cpu->decoded->y];

    for (unsigned int row_y = 0; row_y < height; row_y++) {
        uint8_t sprite = chip->ram[cpu->I + row_y];
        for (unsigned int col_x = 0; col_x < width; col_x++) {
            // we check if the pixel at place col_x is to be set or not
            // if yes, we set it
            if ((sprite & (0x80 >> col_x)) != 0) {
                uint8_t warped_x = (x + col_x)%DISPLAY_WIDTH;
                uint8_t warped_y = (y + row_y)%DISPLAY_HEIGHT;

                if (chip->graphics[warped_y][warped_x] == 1)
                    cpu->V[0xF] = 1;

                // we toggle the pixel
                chip->graphics[warped_y][warped_x] ^= 1;
            }
        }
    }
    chip->draw_flag = 1;
}

/*
Ex9E - SKP Vx
Skip next instruction if key with the value of Vx is pressed.
*/
void exeEx9E(Chip8* chip) {
    printf("Skip if pressed: \n");
    for (int i = 0; i < KEYPAD_SIZE; i++)
        printf("keypad[%X] -> %i\n", i, chip->input->keypad[i]);
    printf("Skip if key pressed: 0x%X\n", chip->cpu->decoded->x);
    Cpu* cpu = chip->cpu;
    Input* input = chip->input;
    if (input->keypad[cpu->V[cpu->decoded->x]] != 0)
        cpu->pc += 2;
}

/*
ExA1 - SKNP Vx
Skip next instruction if key with the value of Vx is not pressed.
*/
void exeExA1(Chip8* chip) {
    printf("Skip if not pressed: \n");
    for (int i = 0; i < KEYPAD_SIZE; i++)
        printf("keypad[%X] -> %i\n", i, chip->input->keypad[i]);
    Cpu* cpu = chip->cpu;
    Input* input = chip->input;
    if (input->keypad[cpu->V[cpu->decoded->x]] == 0)
        cpu->pc += 2;
}

/*
Fx07 - LD Vx, DT
Set Vx = delay timer value.
*/
void exeFx07(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->V[cpu->decoded->x] = cpu->dt;
}

/*
Fx0A - LD Vx, K
Wait for a key press, store the value of the key in Vx.
*/
void exeFx0A(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    Input* input = chip->input;
    uint8_t isKeyPressed = 0;
    cpu->V[cpu->decoded->x] = INPUT_waitForKeyPressed(input);
    // We check to see if we are quitting the game
    // since we stop the cpu and enter an infinite loop
    // This is the only way I found to easily quit the emulator
    if (cpu->V[cpu->decoded->x] == 0xff)
        chip->isQuitting = 1;
}

/*
Fx15 - LD DT, Vx
Set delay timer = Vx.
*/
void exeFx15(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->V[cpu->decoded->x] = cpu->dt;
}

/*
Fx18 - LD ST, Vx
Set sound timer = Vx.
*/
void exeFx18(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->st = cpu->V[cpu->decoded->x];
}

/*
Fx1E - ADD I, Vx
Set I = I + Vx.
*/
void exeFx1E(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->I += cpu->V[cpu->decoded->x];
}

/*
Fx29 - LD F, Vx
Set I = location of sprite for digit Vx.
*/
void exeFx29(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    cpu->I = 5 * cpu->V[cpu->decoded->x];
}

/*
Fx33 - LD B, Vx
Store BCD representation of Vx in memory locations I, I+1, and I+2.
*/
void exeFx33(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    chip->ram[cpu->I] = cpu->V[cpu->decoded->x] / 100;
    chip->ram[cpu->I + 1] = (cpu->V[cpu->decoded->x] / 10) % 10;
    chip->ram[cpu->I + 2] = (cpu->V[cpu->decoded->x] % 100) % 10;  
}

/*
Fx55 - LD [I], Vx
Store registers V0 through Vx in memory starting at location I.
*/
void exeFx55(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    for (uint8_t i = 0; i <= cpu->decoded->x; ++i) {
        chip->ram[cpu->I + i] = cpu->V[i];
    }
}

/*
Fx65 - LD Vx, [I]
Read registers V0 through Vx from memory starting at location I.
*/
void exeFx65(Chip8* chip) {
    Cpu* cpu = chip->cpu;
    for (uint8_t i = 0; i <= cpu->decoded->x; ++i) {
        cpu->V[i] = chip->ram[cpu->I + i];
    }
}


/*
When the cpu sees an opcode that it doesn't know, we throw this error
*/
void exeXXXX(Chip8* chip) {
    uint16_t code = chip->cpu->decoded->op | chip->cpu->decoded->nnn;
    fprintf(stderr, "Unknown op code 0x%X \n", code);
}