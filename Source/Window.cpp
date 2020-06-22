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

void glfwOnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	w->OnMouseScrollEvent(xoffset, yoffset);
}

void Window::Create(std::string _title, unsigned int _width, unsigned int _height, bool _fullScreen)
{
	if (!glfwInit())
	{
		// Initialization failed
		glfwTerminate();
		std::cout << "GLFW init failed" << std::endl;
	}

	// Request an OpenGL 3.3 context in core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	GLFWmonitor* monitor = _fullScreen ? glfwGetPrimaryMonitor() : nullptr;
	window = glfwCreateWindow(_width, _height, _title.c_str(), monitor, nullptr);
	if (!window)
	{
		// Window or OpenGL context creation failed
		glfwTerminate();
		std::cout << "GLFW window failed" << std::endl;
	}

	// Create the OpenGL context
	glfwMakeContextCurrent(window);

	// Load OpenGL functions
	if (!gladLoadGL()) {
		glfwTerminate();
		printf("Something went wrong!\n");
		exit(-1);
	}

	// Turn on V-Sync
	glfwSwapInterval(1);

	// Set up input callbacks
	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, glfwOnResizeEvent);
	glfwSetMouseButtonCallback(window, glfwOnMouseClick);
	glfwSetCursorPosCallback(window, glfwOnMouseMovement);
	glfwSetScrollCallback(window, glfwOnMouseScroll);
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

void Window::AddMouseListener(MouseListener* mouseListener)
{
	mouseListeners.push_back(mouseListener);
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

void Window::OnMouseScrollEvent(float xOffset, float yOffset)
{
	for (MouseListener* listener : mouseListeners)
	{
		listener->OnMouseScroll(xOffset, yOffset);
	}
}

bool Window::IsOpen()
{
	return !glfwWindowShouldClose(window);
}
