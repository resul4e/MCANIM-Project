#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void glfwOnResizeEvent(GLFWwindow* window, int width, int height)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	w->OnResizeEvent(width, height);
}

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

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, glfwOnResizeEvent);
}

void Window::update()
{
	glfwPollEvents();
}

void Window::render()
{
	glfwSwapBuffers(window);
}

void Window::AddResizeListener(ResizeListener* resizeListener)
{
	resizeListeners.push_back(resizeListener);
}

void Window::OnResizeEvent(int width, int height)
{
	for (ResizeListener* listener : resizeListeners)
	{
		listener->OnResize(width, height);
	}
}

bool Window::isOpen()
{
	return !glfwWindowShouldClose(window);
}
