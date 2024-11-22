#include "Emu.h"

EmuMmu::EmuMmu() { _mem.fill(0); }

uint8_t EmuMmu::mem_read(int addr) {
  if (addr == 0x7100) {
    uint8_t value = 0;
    std::cin >> value;
    return value;
  }
  return _mem[addr];
}

void EmuMmu::mem_write(int addr, uint8_t value) {
  if (addr == 0x7110) {
    std::cout << value;
  }
  if (addr == 0x7120) {
    std::cerr << value;
  }
  if (addr == 0x7200) {
    exit(0);
  }
  _mem[addr] = value;
}

void EmuMmu::mem_clear(void) {
  for (int i = 0; i < RAM_BASE; i++) {
    mem_write(i, 0);
  }
}