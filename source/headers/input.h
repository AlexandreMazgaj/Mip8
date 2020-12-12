#include <stdint.h>
#include "constants.h"

typedef struct input {

    // the keypad of the chip8
    uint8_t keypad[KEYPAD_SIZE];
    
} Input;


void INPUT_init(Input* input);

uint8_t INPUT_waitForKeyPressed(Input* input);

void INPUT_checkKeyPressed(Input* input, uint8_t keys[]);
