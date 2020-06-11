#pragma once

#include <string>
#include <vector>

struct GLFWwindow;

class ResizeListener
{
public:
	virtual void OnResize(int width, int height) = 0;
};

class Window
{
public:
	void Create(std::string title, unsigned int width, unsigned int height, bool fullScreen = false);
	void Update();
	void Render();

	GLFWwindow* GetWindow() { return window; }

	void AddResizeListener(ResizeListener* resizeListener);
	void OnResizeEvent(int width, int height);

	bool IsOpen();

private:
	GLFWwindow* window;

	std::vector<ResizeListener*> resizeListeners;
};
