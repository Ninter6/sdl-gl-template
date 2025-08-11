//
// Created by Ninter6 on 2025/8/5.
//

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL3/SDL.h>

#include <string>

class Window {
public:
    Window(int width, int height, std::string_view title, bool logic_size = false);

    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void close();
    [[nodiscard]] bool running() const;

    void next_frame(int delay = 20) const;

    [[nodiscard]] SDL_Window* get() const;

private:
    SDL_Window* _window = nullptr;
    bool _running = true;
};

#endif //WINDOW_HPP
