#ifndef CHIP_H
#define CHIP_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>


//ACCESSING NIBLETS WITH MACROS
#define NNN (chip->opcode & 0xfff)
#define KK (chip->opcode & 0xff)
#define X ((chip->opcode & 0xf00) >> 8)
#define Y ((chip->opcode & 0xf0) >> 4)
#define P1 ((chip->opcode & 0xf000) >> 12)
#define P4 ((chip->opcode & 0xf))
#define SWAP(x,y) (do{x = x^y; y = y^x; x = x^y;}while(0);)
#define BIT_STATE(n,k) (((n|(1<<k))== n)?1:0)


typedef unsigned char byte;
typedef unsigned short hword;

enum rn
{
    r0, r1, r2, r3, r4,
    r5, r6, r7, r8,
    r9, ra, rb, rc,
    rd, re, rf
};

extern const byte font_arr[];

typedef struct
{
    byte amem[4096];
    byte reg[16];
    byte keys[16];
    hword st[64];
    hword index;
    hword pc;
    hword stp;
    byte delay_t;
    byte sound_t;
    byte display_buffer[32][64];
    hword opcode;
} chip8;

chip8* chip8_new();
void eval_opcode(chip8* chip);
void op_00ep4(chip8* chip);
void op_00e0(chip8* chip);
void op_00ee(chip8* chip);
void op_1nnn(chip8* chip);
void op_2nnn(chip8* chip);
void op_3xkk(chip8* chip);
void op_4xkk(chip8* chip);
void op_5xy0(chip8* chip);
void op_6xkk(chip8* chip);
void op_7xkk(chip8* chip);
void op_8xyp4(chip8* chip);
void op_8xy0(chip8* chip);
void op_8xy1(chip8* chip);
void op_8xy2(chip8* chip);
void op_8xy3(chip8* chip);
void op_8xy4(chip8* chip);
void op_8xy5(chip8* chip);
void op_8xy6(chip8* chip);
void op_8xy7(chip8* chip);
void op_8xye(chip8* chip);
void op_9xy0(chip8* chip);
void op_annn(chip8* chip);
void op_bnnn(chip8* chip);
void op_cxkk(chip8* chip);
void op_dxyp4(chip8* chip);
void op_exkk(chip8* chip);
void op_ex9e(chip8* chip);
void op_exa1(chip8* chip);
void op_fxkk(chip8* chip);
void op_fx07(chip8* chip);
void op_fx0a(chip8* chip);
void op_fx15(chip8* chip);
void op_fx18(chip8* chip);
void op_fx1e(chip8* chip);
void op_fx29(chip8* chip);
void op_fx33(chip8* chip);
void op_fx55(chip8* chip);
void op_fx65(chip8* chip);

void load_file(chip8* chip, char* fname);
void get_op(chip8* chip);
void draw_buffer(chip8* chip, SDL_Renderer* renderer);
void handle_input(chip8* chip);
void debug(chip8* chip);


#endif
