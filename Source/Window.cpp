#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <iostream>

void glfwOnResizeEvent(GLFWwindow* window, int width, int height)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	w->OnResizeEvent(width, height);
}

void glfwOnMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
		w->OnMouseClickedEvent(button, mods);
	if (action == GLFW_RELEASE)
		w->OnMouseReleasedEvent(button, mods);
}

void glfwOnMouseMovement(GLFWwindow* window, double xpos, double ypos)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	w->OnMouseMoveEvent(xpos, ypos);
}

void Window::Create(std::string title, unsigned int width, unsigned int height, bool fullScreen)
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
	glfwSetMouseButtonCallback(window, glfwOnMouseClick);
	glfwSetCursorPosCallback(window, glfwOnMouseMovement);
}

void Window::Update()
{
	glfwPollEvents();
}

void Window::Render()
{
	glfwSwapBuffers(window);
}

int Window::GetWidth()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return width;
}

int Window::GetHeight()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return height;
}

void Window::SetWindowIcon(std::filesystem::path _iconPath)
{
	GLFWimage images[1];
	images[0].pixels = stbi_load(_iconPath.string().c_str(), &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
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

void Window::OnMouseClickedEvent(int button, int mods)
{
	for (MouseListener* listener : mouseListeners)
	{
		listener->OnMouseClicked(button, mods);
	}
}

void Window::OnMouseReleasedEvent(int button, int mods)
{
	for (MouseListener* listener : mouseListeners)
	{
		listener->OnMouseReleased(button, mods);
	}
}

void Window::OnMouseMoveEvent(float x, float y)
{
	for (MouseListener* listener : mouseListeners)
	{
		listener->OnMouseMove(x, y);
	}
}

bool Window::IsOpen()
{
	return !glfwWindowShouldClose(window);
}
