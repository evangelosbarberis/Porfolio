#pragma once

#include <array>
#include <cstdint>
#include <memory>

#define RAM_BASE 0x7000
#define MEM_SIZE 0x10000

class EmuMmu
{
    private:
        std::array<uint8_t, MEM_SIZE> _mem;

    public:
        EmuMmu(void);
        uint8_t mem_read(int addr);
        void mem_write(int addr, uint8_t data);
        void mem_clear(void);
};