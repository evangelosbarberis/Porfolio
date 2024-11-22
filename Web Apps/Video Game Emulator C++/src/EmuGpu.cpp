#include <iostream>

#include "Emu.h"

#define IO_ADDRESS 0x7000
#define WINDOW_SCALE 8

EmuGpu::EmuGpu() {}

EmuGpu::EmuGpu(EmuMmu *mmu) {
  _renderer = NULL;
  _window = NULL;
  _mmu = mmu;
}

void EmuGpu::initializeSDL(void) {
  int rendererFlags = SDL_RENDERER_ACCELERATED;
  int windowFlags = 0;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }

  _window = SDL_CreateWindow(
      "Display Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH * WINDOW_SCALE, SCREEN_HEIGHT * WINDOW_SCALE, windowFlags);
  if (!_window) {
    std::cerr << "Failed to open" << SCREEN_WIDTH << " x " << SCREEN_HEIGHT
              << "window: " << SDL_GetError() << std::endl;
    exit(1);
  }
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  _renderer = SDL_CreateRenderer(_window, -1, rendererFlags);
  if (!_renderer) {
    std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
    exit(1);
  }
}

void EmuGpu::prepareScene(void) {
  SDL_RenderSetScale(_renderer, WINDOW_SCALE, WINDOW_SCALE);
  SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
  SDL_RenderClear(_renderer);
  drawFrame();
}

void EmuGpu::presentScene(void) { SDL_RenderPresent(_renderer); }

int getPixelAddress(int width, int height) {
  int pixel_index = width + (height * 64);
  int pixel_offset = 1 * pixel_index;
  return 0x6000 + pixel_offset;
}

void EmuGpu::decodePixel(uint16_t &colorAddress, uint8_t &r, uint8_t &g,
                         uint8_t &b) {
  uint8_t pixelColor = _mmu->mem_read(colorAddress);
  int redBits = (pixelColor >> 4) & 0x3;
  int greenBits = (pixelColor >> 2) & 0x3;
  int blueBits = pixelColor & 0x3;

  r = redBits * 85;
  g = greenBits * 85;
  b = blueBits * 85;
}

void EmuGpu::drawFrame(void) {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint16_t currentColorAddress;
  for (int i = 0; i < SCREEN_WIDTH; i++) {
    for (int j = 0; j < SCREEN_HEIGHT; j++) {
      currentColorAddress = getPixelAddress(i, j);
      decodePixel(currentColorAddress, red, green, blue);

      SDL_SetRenderDrawColor(_renderer, red, green, blue, 255);
      SDL_RenderDrawPoint(_renderer, i, j);
    }
  }
}