#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void Window::create(std::string title, unsigned int width, unsigned int height, bool fullScreen)
{
    if (!glfwInit())
    {
        // Initialization failed
        glfwTerminate();
        std::cout << "GLFW init failed" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWmonitor* monitor = fullScreen ? glfwGetPrimaryMonitor() : nullptr;
    window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    if (!window)
    {
        // Window or OpenGL context creation failed
        glfwTerminate();
        std::cout << "GLFW window failed" << std::endl;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        glfwTerminate();
        printf("Something went wrong!\n");
        exit(-1);
    }

    glfwSwapInterval(1);
}

void Window::update()
{
    glfwPollEvents();
}

void Window::render()
{
    glfwSwapBuffers(window);
}

bool Window::isOpen()
{
    return !glfwWindowShouldClose(window);
}
