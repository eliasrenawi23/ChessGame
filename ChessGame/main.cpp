#include <iostream>
#include <SDL3/SDL.h>
#include "Window.h"

int main(int argc, char* args[]) {
    Window screen;

    if (!screen.init()) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    else {
        std::cout << "SDL initialized successfully." << std::endl;
    }

    bool running = true;
    while (running) {
        screen.updateRender();
        running = screen.processEvents();
    }

    screen.close();
    return 0;
}
