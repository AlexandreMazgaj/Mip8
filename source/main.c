#include <stdio.h>
#include "chip8.h"
#include <SDL2/SDL.h>



void drawScreen(SDL_Surface *surface, Chip8 *mip)
{
    SDL_LockSurface(surface);
    uint32_t *pixels = (uint32_t *)surface->pixels;
    for (int i = 0; i < DISPLAY_HEIGHT; i++) {
        for (int j = 0; j < DISPLAY_WIDTH; j++) {
            pixels[i*DISPLAY_WIDTH + j] = mip->graphics[i][j] == 0 ? 0 : 0xFFFFFFFF;
        }
    }
    SDL_UnlockSurface(surface);
}


int main(int argc, char** argv) {

    Chip8 mip;

    CHIP8_init(&mip);

    CHIP8_loadRom(&mip, "/Users/mazgajalexandre/workspace/chip8_emu/roms/test_opcode.ch8");


    SDL_Surface *mip8Graphics = NULL;
    SDL_Surface *scaledGraphics = NULL;



    //The window we'll be rendering to
    SDL_Window* window = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;


    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow("MIP8", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if( window == NULL ){
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    } 

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);


    //Get window surface
    screenSurface = SDL_GetWindowSurface( window );

    mip8Graphics = SDL_CreateRGBSurface(0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 32, 0, 0, 0, 0);
    scaledGraphics = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);


    SDL_RenderPresent(renderer);


    // //Update the surface
    // SDL_UpdateWindowSurface( window );

    SDL_Delay( 100 );

    // used for the framerate
    unsigned int t1;
    unsigned int t2;
    unsigned int elapsed;
    int          remaining;
    unsigned int interval = 1000 / TICKS;


    Uint8* keys = SDL_GetKeyboardState(NULL);

    uint8_t emuRun = 0;

    SDL_Event e;


    printf("LAUNCHING MIP8\n");

    while (1) {
        t1 = SDL_GetTicks();

        if ((SDL_PollEvent(&e)) != 0) {
            if (e.type == SDL_QUIT) {
                break;
            }

            if (e.type == SDL_KEYDOWN && keys[SDL_SCANCODE_SPACE]) {
                emuRun ^= 1;
            }

            if (e.type == SDL_KEYDOWN)
                INPUT_checkKeyPressed(mip.input, keys);

            if (emuRun == 0 && e.type == SDL_KEYDOWN && keys[SDL_SCANCODE_P]) {
                // We execute the program step by step
                    CHIP8_clock(&mip);
            }
        }
        
        if (emuRun == 1) {
            CHIP8_clock(&mip);
        }

        if (mip.isQuitting)
            break;
        
        if (mip.draw_flag) {
            drawScreen(mip8Graphics, &mip);
            SDL_BlitScaled(mip8Graphics, NULL, scaledGraphics, NULL);
            // Update the intermediate texture with the contents of the RGBA buffer.
            SDL_UpdateTexture(texture, NULL, scaledGraphics->pixels, scaledGraphics->pitch);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            //Update the surface
            // SDL_UpdateWindowSurface( window );

            mip.draw_flag = 0;
        }


        t2 = SDL_GetTicks();
        elapsed = t2 - t1;
        remaining = interval - elapsed;
        if (remaining > 0) {
            SDL_Delay(remaining/2);
            elapsed = interval;
        }
    }

    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();

    // We finish by destroying the chip
    CHIP8_destroy(&mip);

    return 0;
}