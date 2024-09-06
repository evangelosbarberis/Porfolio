#pragma once

#include <SDL2/SDL.h>
#include "Emu.h"

class EmuIO
{
    private:
        SDL_Event _event;
        EmuMmu *_mmu;

    public:
        EmuIO();
        EmuIO(EmuMmu *mem_ptr);
        void readIO(void);
};