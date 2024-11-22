#include <cstring>
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>

#include "Emu.h"

EmuConsole::EmuConsole(char* filename) {
  _filename = filename;
  mmu = EmuMmu();
  cpu = EmuCpu(&mmu);
  gpu = EmuGpu(&mmu);
  io = EmuIO(&mmu);
}

void EmuConsole::runProgram() {

  // Open file and load contents
  openFile();

  // load data and call setup
  loadAndSetup();

  // Initialize SDL Window
  gpu.initializeSDL();

  // run loop
  runLoop();
}

void EmuConsole::loadAndSetup() {
  // EmuCpu *cpu = cpu_ptr;
  // EmuMmu *mmu = mmu_ptr;

    // load ROM file
  for (int i = 0; i < 0x8000; i++) {
    (&mmu)->mem_write((i + 0x8000), (uint8_t)(_contents[i]));
  }

  // get addr to setup and loop
  cpu.set_pc(0x81e0);
  setup_addr = (uint16_t)cpu.instr_fetch();

  cpu.set_pc(0x81e4);
  loop_addr = (uint16_t)cpu.instr_fetch();

  cpu.set_pc(0x81ec);
  load_addr = (uint16_t)cpu.instr_fetch();

  cpu.set_pc(0x81e8);
  pdata_addr = (uint16_t)cpu.instr_fetch();

  cpu.set_pc(0x81f0);
  pdata_size = (uint16_t)cpu.instr_fetch();

  // clear ram
  (&mmu)->mem_clear();

  // load data section
  for (int i = 0; i < pdata_size; i++) {
    uint8_t to_write = (&mmu)->mem_read(i + pdata_addr);
    (&mmu)->mem_write(i + load_addr, to_write);
  }

  // set stack pointer
  cpu.reg_write(29, 0x6000);

  // prepare to run setup
  cpu.set_pc(0xfffc);
  cpu.instr_execute(((0x8800 << 16) | (setup_addr / 4)));
  
  // run setup
  while (1) {
    if (cpu.get_pc() == 0 || cpu.get_pc() < 0x8000) {
      break;
    }
    uint32_t instr = cpu.instr_fetch();
    cpu.instr_execute(instr);
  }
}

void EmuConsole::runLoop() {
  // EmuCpu *cpu = cpu_ptr;
  // EmuGpu *gpu = gpu_ptr;
  // EmuIO *io = io_ptr;

  while (1) {
    cpu.reg_write(29, 0x6000);
    cpu.set_pc(0xfffc);
    cpu.instr_execute(((0x8800 << 16) | (loop_addr / 4)));
    while (1) {
      if (cpu.get_pc() == 0 || cpu.get_pc() < 0x8000) {
        break;
      }
      uint32_t instr = cpu.instr_fetch();
      cpu.instr_execute(instr);
    }

    // Update SDL Screen
    gpu.prepareScene();
    io.readIO();
    gpu.presentScene();
    SDL_Delay(16.667);
  }
}

void EmuConsole::openFile() {
  size_t file_size;

  // Open the file
  std::ifstream file(_filename, std::ios::binary | std::ios::ate);

  // Check if file is opened successfully
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << _filename << std::endl;
    exit(EXIT_FAILURE);
  }

  // Get the size of the file
  file_size = file.tellg();

  if (file_size > 0) {
    // Reset file pointer to the beginning
    file.seekg(0, std::ios::beg);

    // Allocate memory to store the _contents of the file
    _contents = std::make_unique<char[]>(file_size);

    // Read the contents of the file into the allocated memory
    file.read(_contents.get(), file_size);
  } else {
    _contents = std::make_unique<char[]>(1);
    _contents[0] = '\0';
  }
  // Close the file
  file.close();
}
