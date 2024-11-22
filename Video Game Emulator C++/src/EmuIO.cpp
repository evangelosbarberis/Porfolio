
#include "Emu.h"

#define IO_ADDRESS 0x7000

EmuIO::EmuIO() {}
EmuIO::EmuIO(EmuMmu *mmu) {
    _mmu = mmu;
}

void EmuIO::readIO(void) {
  while (SDL_PollEvent(&_event)) {
    if (_event.type == SDL_QUIT) {
      exit(0);
    }
    if (_event.type == SDL_KEYDOWN) {
      // Pressing Key
      switch (_event.key.keysym.sym) {
        case SDLK_RIGHT:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) | 0x01);
          break;
        case SDLK_LEFT:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) | 0x02);
          break;
        case SDLK_DOWN:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) | 0x04);
          break;
        case SDLK_UP:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) | 0x08);
          break;
        case SDLK_ESCAPE:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) | 0x10);
          break;
        case SDLK_BACKSPACE:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) | 0x20);
          break;
        case SDLK_b:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) | 0x40);
          break;
        case SDLK_a:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) | 0x80);
          break;
      }
    }
    if (_event.type == SDL_KEYUP) {
      // Released Key
      switch (_event.key.keysym.sym) {
        case SDLK_RIGHT:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) & ~(0x01));
          break;
        case SDLK_LEFT:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) & ~(0x02));
          break;
        case SDLK_DOWN:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) & ~(0x04));
          break;
        case SDLK_UP:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) & ~(0x08));
          break;
        case SDLK_ESCAPE:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) & ~(0x010));
          break;
        case SDLK_BACKSPACE:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) & ~(0x20));
          break;
        case SDLK_b:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) & ~(0x40));
          break;
        case SDLK_a:
          _mmu->mem_write(IO_ADDRESS, _mmu->mem_read(IO_ADDRESS) & ~(0x80));
          break;
      }
    }
  }
}