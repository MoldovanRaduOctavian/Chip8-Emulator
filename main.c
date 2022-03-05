#include <stdio.h>
#include <stdlib.h>
#include "chip.h"

#define FRAMECAP 120.0

int main(int argc, char** argv)
{
    chip8* chip = chip8_new();

    SDL_Window* window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, 0);
    /*chip->reg[r1] = 0xfe;
    chip->reg[r2] = 0x0f;
    chip->opcode = 0x812e;
    printf("%u %u %u\n", chip->reg[r1], chip->reg[rf], chip->pc);
    eval_opcode(chip);
    printf("%u %u %u\n", chip->reg[r1], chip->reg[rf], chip->pc);
    load_file(chip,"c8_test.c8");
    chip->pc = 0x200;
    do
    {
        get_op(chip);
        printf("%x ", chip->opcode);
        chip->pc += 2;
    }while (chip->opcode != 0);

   /* for (int i=0;i<5;i++)
        printf("%d ", rand());
    chip->reg[r0] = 0;
    chip->reg[r1] = 0;*/
   load_file(chip, "IBM Logo.ch8");
    //chip->pc = 0x392;
    //get_op(chip);
    //printf("%x", chip->opcode);
   /* chip->opcode = 0x60ff;
    eval_opcode(chip);
    printf("%u", chip->reg[0]);
    chip->opcode = 0xf015;
    eval_opcode(chip);
    printf("\n%u", chip->reg[0]);*/
    chip->pc = 0x200;
    uint16_t first, last, delta;
    for (;;)
    {
        first = SDL_GetTicks();
        delta = first - last;
        if (delta > 1000/FRAMECAP)
        {
            //printf("FPS: %d\n", 1000/delta);
            last = first;
            get_op(chip);
            eval_opcode(chip);

            handle_input(chip);
            draw_buffer(chip, renderer);

            if (chip->delay_t)
                chip->delay_t--;
            if (chip->sound_t)
                chip->sound_t--;

        }
    }
    return 0;
}
