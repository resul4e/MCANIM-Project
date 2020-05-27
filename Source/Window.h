#pragma once

#include <string>

struct GLFWwindow;

class Window
{
public:
    void create(std::string title, unsigned int width, unsigned int height, bool fullScreen = false);
    void update();
    void render();

	GLFWwindow* GetWindow() { return window; }

    bool isOpen();

private:
    GLFWwindow* window;
};
