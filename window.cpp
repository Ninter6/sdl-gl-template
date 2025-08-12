//
// Created by Ninter6 on 2025/8/5.
//

#include <iostream>

#include "window.hpp"

// Function to calculate window size based on target resolution and display scale
void calculateWindowSize(int targetWidth, int targetHeight, unsigned displayIndex,
                         int& logicalWidth, int& logicalHeight) {
    float scaleFactor = SDL_GetDesktopDisplayMode(displayIndex)->pixel_density;

    // Calculate logical size
    logicalWidth = static_cast<int>(targetWidth / scaleFactor);
    logicalHeight = static_cast<int>(targetHeight / scaleFactor);

    std::cout << "Display scale: " << scaleFactor << "x\n";
    std::cout << "Target resolution: " << targetWidth << "x" << targetHeight << " pixels\n";
    std::cout << "Logical window size: " << logicalWidth << "x" << logicalHeight << " points\n";
}

Window::Window(int width, int height, std::string_view title, bool logic_size) {
    // Create window
    int logic_width = width, logic_height  = height;
    if (!logic_size)
        calculateWindowSize(width, height,
                            SDL_GetPrimaryDisplay(),
                            logic_width, logic_height);
    _window = SDL_CreateWindow("SDL GL Template",
                               logic_width, logic_height,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!_window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        std::terminate();
    }

    // Create OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(_window);
    if (!context) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
        std::terminate();
    }
}

Window::~Window() {
    SDL_DestroyWindow(_window);
}

void Window::close() {
    _running = false;
}


bool Window::running() const {
    return _running;
}

void Window::next_frame(int delay) const {
    SDL_GL_SwapWindow(_window);
    if (delay) SDL_Delay(delay);
}

SDL_Window* Window::get() const {
    return _window;
}

