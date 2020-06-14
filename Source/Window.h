#pragma once

#include <string>
#include <vector>
#include <filesystem>

struct GLFWwindow;

class ResizeListener
{
public:
	virtual void OnResize(int width, int height) = 0;
};

class MouseListener
{
public:
	virtual void OnMouseClicked(int button, int mods) = 0;
	virtual void OnMouseReleased(int button, int mods) = 0;
	virtual void OnMouseMove(float x, float y) = 0;
};

class Window
{
public:
	void Create(std::string title, unsigned int width, unsigned int height, bool fullScreen = false);
	void Update();
	void Render();

	GLFWwindow* GetWindow() { return window; }
	int GetWidth();
	int GetHeight();

	void SetWindowIcon(std::filesystem::path _iconPath);
	void AddResizeListener(ResizeListener* resizeListener);
	void OnResizeEvent(int width, int height);
	void OnMouseClickedEvent(int button, int mods);
	void OnMouseReleasedEvent(int button, int mods);
	void OnMouseMoveEvent(float x, float y);

	bool IsOpen();

private:
	GLFWwindow* window;

	std::vector<ResizeListener*> resizeListeners;
	std::vector<MouseListener*> mouseListeners;
};
