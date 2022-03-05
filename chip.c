#include "chip.h"



const byte font_arr[] = { 0xF0, 0x90, 0x90 ,0x90, 0xF0,
                0x20, 0x60, 0x20 ,0x20, 0x70,
                0xF0, 0x10, 0xF0 ,0x80, 0xF0,
                0xF0, 0x10, 0xF0 ,0x10, 0xF0,
                0x90, 0x90, 0xF0 ,0xF0, 0x10,
                0xF0, 0x80, 0xF0 ,0x10, 0xF0,
                0xF0, 0x80, 0xF0 ,0x90, 0xF0,
                0xF0, 0x10, 0x20 ,0x40, 0x40,
                0xF0, 0x90, 0xF0 ,0x90, 0xF0,
                0xF0, 0x90, 0xF0 ,0x10, 0xF0,
                0xF0, 0x90, 0xF0 ,0x90, 0x90,
                0xE0, 0x90, 0xE0 ,0x90, 0xE0,
                0xF0, 0x80, 0x80 ,0x80, 0xF0,
                0xE0, 0x90, 0x90 ,0x90, 0xE0,
                0xF0, 0x80, 0xF0 ,0x80, 0xF0,
                0xF0, 0x80, 0xF0 ,0x80, 0x80,
            };

chip8* chip8_new()
{
    chip8* chip = (chip8*)malloc(sizeof(chip8));
    chip->index = 0;
    chip->pc = 0x200;
    chip->delay_t = 0;
    chip->sound_t = 0;
    chip->opcode = 0;
    chip->stp = 0;
    srand(time(0));
    int i, j;
    for (i = r0; i<=rf; i++)
        chip->reg[i] = 0;
    for (i = 0; i<4096; i++)
        chip->amem[i] = 0;
    for (i = 0; i<32; i++)
        for (j = 0; j<64; j++)
            chip->display_buffer[i][j] = 0;
    for (i = 0; i<16; i++)
        chip->keys[i] = 0;
    for (i = 0; i<64; i++)
        chip->st[i] = 0;
    memcpy(chip->amem, font_arr, 80);
    return chip;
}

void eval_opcode(chip8* chip)
{
    switch (P1)
    {
        case 0x0: op_00ep4(chip); break;
        case 0x1: op_1nnn(chip); break;
        case 0x2: op_2nnn(chip); break;
        case 0x3: op_3xkk(chip); break;
        case 0x4: op_4xkk(chip); break;
        case 0x5: op_5xy0(chip); break;
        case 0x6: op_6xkk(chip); break;
        case 0x7: op_7xkk(chip); break;
        case 0x8: op_8xyp4(chip); break;
        case 0x9: op_9xy0(chip); break;
        case 0xa: op_annn(chip); break;
        case 0xb: op_bnnn(chip); break;
        case 0xc: op_cxkk(chip); break;
        case 0xd: op_dxyp4(chip); break;
        case 0xe: op_exkk(chip); break;
        case 0xf: op_fxkk(chip); break;
    }
}

void op_00ep4(chip8* chip)
{
    (P4==0xe)?op_00ee(chip):op_00e0(chip);
}

void op_00e0(chip8* chip)
{
    for (int i = 0; i<32; i++)
        for (int j = 0; j<64; j++)
            chip->display_buffer[i][j] = 0;
    chip->pc += 2;
}

//E futut probabil
void op_00ee(chip8* chip)
{
    chip->pc = chip->st[chip->stp];
    chip->stp--;
}

void op_1nnn(chip8* chip)
{
    chip->pc = NNN;
}

//To be tested; E futut probabil
void op_2nnn(chip8* chip)
{
    chip->stp++;
    chip->st[chip->stp] = chip->pc;
    chip->pc = NNN;
}



void op_3xkk(chip8* chip)
{
    if (chip->reg[X] == KK)
        chip->pc += 4;
    else
        chip->pc += 2;
}

void op_4xkk(chip8* chip)
{
    if (chip->reg[X] != KK)
        chip->pc += 4;
    else
        chip->pc += 2;
}

void op_5xy0(chip8* chip)
{
    if (chip->reg[X] == chip->reg[Y])
        chip->pc += 4;
    else
        chip->pc += 2;
}

void op_6xkk(chip8* chip)
{
    chip->reg[X] = KK;
    chip->pc += 2;
}

void op_7xkk(chip8* chip)
{
    chip->reg[X] += KK;
    chip->pc += 2;
}

//8 instructions to be tested
void op_8xy1(chip8* chip)
{
    chip->reg[X] = chip->reg[X] | chip->reg[Y];
    chip->pc += 2;
}

void op_8xy0(chip8* chip)
{
    chip->reg[X] = chip->reg[Y];
    chip->pc += 2;
}

void op_8xy2(chip8* chip)
{
    chip->reg[X] = chip->reg[X] & chip->reg[Y];
    chip->pc += 2;
}

void op_8xy3(chip8* chip)
{
    chip->reg[X] = chip->reg[X] ^ chip->reg[Y];
    chip->pc += 2;
}

void op_8xy5(chip8* chip)
{
    if (chip->reg[X] > chip->reg[Y])
        chip->reg[rf] = 1;
    else
        chip->reg[rf] = 0;
    chip->reg[X] -= chip->reg[Y];
    chip->pc += 2;
}

void op_8xy4(chip8* chip)
{
    unsigned int s = (unsigned int)chip->reg[X] + (unsigned int)chip->reg[Y];
    if (s > 255)
        chip->reg[rf] = 1;
    else
        chip->reg[rf] = 0;
    chip->reg[X] = s & 0xff;
    chip->pc += 2;
}

//To be tested

//v
void op_8xy6(chip8* chip)
{
    if (BIT_STATE(chip->reg[X], 0) == 1)
        chip->reg[rf] = 1;
    else
        chip->reg[rf] = 0;
    chip->reg[X] >>= 1;
    chip->pc += 2;
}

//v?
void op_8xy7(chip8* chip)
{
    if (chip->reg[X] > chip->reg[Y])
        chip->reg[rf] = 1;
    else
        chip->reg[rf] = 0;
    chip->reg[X] = chip->reg[Y] - chip->reg[X];
    chip->pc += 2;

}

//v
void op_8xye(chip8* chip)
{
   if (BIT_STATE(chip->reg[X], 7) == 1)
        chip->reg[rf] = 1;
   else
        chip->reg[rf] = 0;
   chip->reg[X] <<= 1;
   chip->pc += 2;
}

void op_8xyp4(chip8* chip)
{
    switch(P4)
    {
        case 0x0:op_8xy0(chip);break;
        case 0x1:op_8xy1(chip);break;
        case 0x2:op_8xy2(chip);break;
        case 0x3:op_8xy3(chip);break;
        case 0x4:op_8xy4(chip);break;
        case 0x5:op_8xy5(chip);break;
        case 0x6:op_8xy6(chip);break;
        case 0x7:op_8xy7(chip);break;
        case 0xe:op_8xye(chip);break;
    }
}

void op_9xy0(chip8* chip)
{
    if (chip->reg[X] != chip->reg[Y])
        chip->pc += 4;
    chip->pc += 2;
}

void op_annn(chip8* chip)
{
    chip->index = NNN;
    chip->pc += 2;
}

void op_bnnn(chip8* chip)
{
    chip->pc = NNN + chip->reg[r0];
}

void op_cxkk(chip8* chip)
{
    chip->reg[X] = (rand() % 256) & KK;
    chip->pc += 2;
}


//To be tested thoroughly
void op_dxyp4(chip8* chip)
{
    int i, j;
    byte aux, posx, posy, ok = 0;
    byte sprite_hex[16];
    byte sprite_bin[P4][8];
    memcpy(sprite_hex, chip->amem + chip->index, P4);
    for (i=0;i<P4;i++)
    {
        aux = sprite_hex[i];
        for (j=7;j>=0;j--)
        {
            sprite_bin[i][j] = aux%2;
            aux /= 2;
        }
    }

    for (i=0;i<P4;i++)
        for (j=0;j<8;j++)
    {
        if (chip->reg[X] + j > 63)
            posx = chip->reg[X] + j - 64;
        else
            posx = chip->reg[X] + j;

        if (chip->reg[Y] + i > 31)
            posy = chip->reg[Y] + i - 32;
        else
            posy = chip->reg[Y] + i;

        if (ok == 0)
          {
             if (chip->display_buffer[posy][posx] == 1 && sprite_bin[i][j] == 1)
                {
                    chip->reg[rf] = 1;
                    ok = 1;
                }
             else
                chip->reg[rf] = 0;
          }

        chip->display_buffer[posy][posx] ^= sprite_bin[i][j];
    }

    chip->pc += 2;
}

//To be tested F

void op_exkk(chip8* chip)
{
    (KK==0x9e)?op_ex9e(chip):op_exa1(chip);
}

void op_ex9e(chip8* chip)
{
    if (chip->keys[chip->reg[X]] == 1)
        chip->pc += 4;
    else
        chip->pc += 2;
}

void op_exa1(chip8* chip)
{
    if (chip->keys[chip->reg[X]] != 1)
        chip->pc += 4;
    else
        chip->pc += 2;
}

void op_fx07(chip8* chip)
{
    chip->reg[X] = chip->delay_t;
    chip->pc += 2;
}

void op_fx0a(chip8* chip)
{
    if (chip->keys[chip->reg[X]] == 1)
        chip->pc += 2;
}

void op_fx15(chip8* chip)
{
   chip->delay_t = chip->reg[X];
   chip->pc += 2;
}

void op_fx18(chip8* chip)
{
    chip->sound_t = chip->reg[X];
    chip->pc += 2;
}

void op_fx1e(chip8* chip)
{
    chip->index += chip->reg[X];
    chip->pc += 2;
}

void op_fx29(chip8* chip)
{
    chip->index = chip->reg[X] * 5;
    chip->pc += 2;
}

//E futut probabil
void op_fx55(chip8* chip)
{
    for (int i=0;i<=X;i++)
        chip->amem[i + chip->index] = chip->reg[i];
    //chip->index += X;
    chip->pc += 2;
}

//E futut probabil
void op_fx65(chip8* chip)
{
    for (int i=r0;i<=X;i++)
        chip->reg[i] = chip->amem[i + chip->index];
    //chip->index += X;
    chip->pc += 2;
}

void op_fx33(chip8* chip)
{
    chip->amem[chip->index] = chip->reg[X] / 100;
    chip->amem[chip->index + 1] = chip->reg[X] / 10 % 10 ;
    chip->amem[chip->index + 2] = chip->reg[X] % 10;
    chip->pc += 2;
}

void op_fxkk(chip8* chip)
{
    switch(KK)
    {
        case 0x07: op_fx07(chip); break;
        case 0x0a: op_fx0a(chip); break;
        case 0x15: op_fx15(chip); break;
        case 0x18: op_fx18(chip); break;
        case 0x1e: op_fx1e(chip); break;
        case 0x29: op_fx29(chip); break;
        case 0x33: op_fx33(chip); break;
        case 0x55: op_fx55(chip); break;
        case 0x65: op_fx65(chip); break;
    }
}


void load_file(chip8* chip, char* fname)
{
    FILE* rom = fopen(fname, "rb");
    fseek(rom, 0, SEEK_END);
    long len = ftell(rom);
    rewind(rom);

    fread(chip->amem + 0x200, 1, len, rom);
}

void get_op(chip8* chip)
{
    chip->opcode = (chip->amem[chip->pc]<<8) | chip->amem[chip->pc + 1];
}


void draw_buffer(chip8* chip, SDL_Renderer* renderer)
{
    int i, j;
    SDL_Rect tile;
    tile.x = 0;
    tile.y = 0;
    tile.w = 10;
    tile.h = 10;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
    for (i = 0;i < 32;i++)
        for (j = 0;j < 64;j++)
        {
            tile.x = 10 * j;
            tile.y = 10 * i;
            if (chip->display_buffer[i][j] == 1)
            {

                SDL_RenderDrawRect(renderer, &tile);
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    SDL_RenderPresent(renderer);
}


void handle_input(chip8* chip)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case SDLK_KP_0: chip->keys[0] = 1; break;
                    case SDLK_KP_1: chip->keys[1] = 1; break;
                    case SDLK_KP_2: chip->keys[2] = 1; break;
                    case SDLK_KP_3: chip->keys[3] = 1; break;
                    case SDLK_KP_4: chip->keys[4] = 1; break;
                    case SDLK_KP_5: chip->keys[5] = 1; break;
                    case SDLK_KP_6: chip->keys[6] = 1; break;
                    case SDLK_KP_7: chip->keys[7] = 1; break;
                    case SDLK_KP_8: chip->keys[8] = 1; break;
                    case SDLK_KP_9: chip->keys[9] = 1; break;
                    case SDLK_q: chip->keys[10] = 1; break;
                    case SDLK_w: chip->keys[11] = 1; break;
                    case SDLK_e: chip->keys[12] = 1; break;
                    case SDLK_a: chip->keys[13] = 1; break;
                    case SDLK_s: chip->keys[14] = 1; break;
                    case SDLK_d: chip->keys[15] = 1; break;
                    case SDLK_ESCAPE: exit(0); break;
                }; break;

            case SDL_KEYUP:
                switch (e.key.keysym.sym)
                {
                    case SDLK_KP_0: chip->keys[0] = 0; break;
                    case SDLK_KP_1: chip->keys[1] = 0; break;
                    case SDLK_KP_2: chip->keys[2] = 0; break;
                    case SDLK_KP_3: chip->keys[3] = 0; break;
                    case SDLK_KP_4: chip->keys[4] = 0; break;
                    case SDLK_KP_5: chip->keys[5] = 0; break;
                    case SDLK_KP_6: chip->keys[6] = 0; break;
                    case SDLK_KP_7: chip->keys[7] = 0; break;
                    case SDLK_KP_8: chip->keys[8] = 0; break;
                    case SDLK_KP_9: chip->keys[9] = 0; break;
                    case SDLK_q: chip->keys[10] = 0; break;
                    case SDLK_w: chip->keys[11] = 0; break;
                    case SDLK_e: chip->keys[12] = 0; break;
                    case SDLK_a: chip->keys[13] = 0; break;
                    case SDLK_s: chip->keys[14] = 0; break;
                    case SDLK_d: chip->keys[15] = 0; break;
                }; break;
        }
    }
}


void debug(chip8* chip)
{
    int i, j;
    //system("cls");
    printf("\n");
    for (i = 0; i<16; i++)
    {
        if (i % 4 == 0)
            printf("\n");
        printf("v%x = %d ", i, chip->reg[i]);
    }
    printf("\npc = %x", chip->pc);
    printf("\nopcode = %x", chip->opcode);
    printf("\nstackpt = %x", chip->stp);
    //SDL_Delay(1000);
}
