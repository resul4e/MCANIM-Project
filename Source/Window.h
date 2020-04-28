#pragma once

#include <string>

class GLFWwindow;

class Window
{
public:
    void create(std::string title, unsigned int width, unsigned int height);
    void update();
    void render();

    bool isOpen();

private:
    GLFWwindow* window;
};
