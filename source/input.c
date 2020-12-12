#include "input.h"

#include <SDL2/SDL.h>

void INPUT_init(Input* input) {
    for (unsigned int i = 0; i < KEYPAD_SIZE; ++i)
        input->keypad[i] = 0x0; 

}


uint8_t INPUT_waitForKeyPressed(Input* input) {
    // we create an SDL event to wait for the key pressed
    SDL_Event event;
    
    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            break;

        if (event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
                case SDLK_1: return 1;
                case SDLK_2: return 2;
                case SDLK_3: return 3;
                case SDLK_4: return 0xc;

                case SDLK_a: return 4;
                case SDLK_z: return 5;
                case SDLK_e: return 6;
                case SDLK_r: return 0xd;

                case SDLK_q: return 7;
                case SDLK_s: return 8;
                case SDLK_d: return 9;
                case SDLK_f: return 0xe;

                case SDLK_w: return 0xa;
                case SDLK_x: return 0;
                case SDLK_c: return 0xb;
                case SDLK_v: return 0xf;
            }
        }
    }
    // code to know that we are quitting
    return 0xff;
}

void INPUT_checkKeyPressed(Input* input, uint8_t keys[]) {
    input->keypad[1]   = keys[SDL_SCANCODE_1];
    input->keypad[2]   = keys[SDL_SCANCODE_2];
    input->keypad[3]   = keys[SDL_SCANCODE_3];
    input->keypad[0xc] = keys[SDL_SCANCODE_4];
    input->keypad[4]   = keys[SDL_SCANCODE_A];
    input->keypad[5]   = keys[SDL_SCANCODE_Z];
    input->keypad[6]   = keys[SDL_SCANCODE_E];
    input->keypad[0xd] = keys[SDL_SCANCODE_R];
    input->keypad[7]   = keys[SDL_SCANCODE_Q];
    input->keypad[8]   = keys[SDL_SCANCODE_S];
    input->keypad[9]   = keys[SDL_SCANCODE_D];
    input->keypad[0xe] = keys[SDL_SCANCODE_F];
    input->keypad[0xa] = keys[SDL_SCANCODE_W];
    input->keypad[0]   = keys[SDL_SCANCODE_X];
    input->keypad[0xb] = keys[SDL_SCANCODE_C];
    input->keypad[0xf] = keys[SDL_SCANCODE_V];
}