#pragma once

#include "Emu.h"

class EmuConsole
{
    private:
      char* _filename;
      EmuMmu mmu;
      EmuCpu cpu;
      EmuGpu gpu;
      EmuIO io;
      std::unique_ptr<char[]> _contents;
      uint16_t setup_addr, loop_addr, load_addr, pdata_addr, pdata_size;

    public:
      EmuConsole(char* filename);
      void openFile(void);
      void runProgram(void);
      void loadAndSetup();
      void runLoop();

};
