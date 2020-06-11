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
    void create(std::string title, unsigned int width, unsigned int height, bool fullScreen = false);
    void update();
    void render();

	GLFWwindow* GetWindow() { return window; }

	void AddResizeListener(ResizeListener* resizeListener);
	void OnResizeEvent(int width, int height);

    bool isOpen();

private:
    GLFWwindow* window;

	std::vector<ResizeListener*> resizeListeners;
};
