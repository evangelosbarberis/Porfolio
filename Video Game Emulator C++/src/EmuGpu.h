#pragma once

#include <SDL2/SDL.h>
#include "Emu.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 64
#define VRAM_SIZE 0x1000

class EmuGpu
{
    private:
        SDL_Renderer *_renderer;
        SDL_Window *_window;
        EmuMmu *_mmu;

    public:
        EmuGpu(void);
        EmuGpu(EmuMmu *mem_ptr);
        void initializeSDL(void);
        void prepareScene(void);
        void presentScene(void);
        void decodePixel(uint16_t &colorAddress, uint8_t &r, uint8_t &g, uint8_t &b);
        void drawFrame(void);
};