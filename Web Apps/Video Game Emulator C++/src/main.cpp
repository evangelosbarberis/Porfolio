#include <cstring>
#include <iostream>
#include <SDL2/SDL.h>

#include "Emu.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Please Specify File\n Usage: ./banana_slug [filename]"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  EmuConsole console = EmuConsole(argv[1]);
  console.runProgram();

  return 0;
}