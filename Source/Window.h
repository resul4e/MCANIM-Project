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
	virtual void OnMouseScroll(float xOffset, float yOffset) = 0;
};

// Local callbacks for passing events to window members functions
void glfwOnResizeEvent(GLFWwindow* window, int width, int height);
void glfwOnMouseClick(GLFWwindow* window, int button, int action, int mods);
void glfwOnMouseMovement(GLFWwindow* window, double xpos, double ypos);
void glfwOnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

class Window
{
public:
	/**
	 * \brief Creates a window, loads OpenGL context and functions and sets up input callbacks
	 * \param _title The title of the window which will be displayed in the title bar
	 * \param _width The desired width of the window
	 * \param _height The desired height of the window
	 * \param _fullscreen Whether the window should be spawned in fullscreen mode or not
	 */
	void Create(std::string _title, unsigned int _width, unsigned int _height, bool _fullScreen = false);

	/**
	 * \brief Polls the window for input updates
	 */
	void Update();

	/**
	 * \brief Swaps the rendering buffers
	 */
	void Render();

	/**
	 * \brief Returns the inner GLFWWindow object
	 * \return the inner GLFWWindow pointer object
	 */
	GLFWwindow* GetWindow() { return window; }

	/**
	 * \brief Returns the width of the current window
	 * \return the width of the current window
	 */
	int GetWidth();

	/**
	 * \brief Returns the height of the current window
	 * \return the height of the current window
	 */
	int GetHeight();

	/**
	 * \brief Loads an image from the given filepath and sets it as the window icon
	 * \param File path to the icon image file
	 */
	void SetWindowIcon(std::filesystem::path _iconPath);

	/**
	 * \brief Adds the given object pointer as a listener to window resize events
	 * \param The object that listens to window resize events
	 */
	void AddResizeListener(ResizeListener* resizeListener);

	/**
	 * \brief Adds the given object pointer as a listener to mouse input events
	 * \param The object that listens to mouse input events
	 */
	void AddMouseListener(MouseListener* mouseListener);

	/**
	 * \return true if the window should stay open, false if the user has requested it to close
	 */
	bool IsOpen();

private:
	void OnResizeEvent(int width, int height);
	void OnMouseClickedEvent(int button, int mods);
	void OnMouseReleasedEvent(int button, int mods);
	void OnMouseMoveEvent(float x, float y);
	void OnMouseScrollEvent(float xOffset, float yOffset);

	friend void glfwOnResizeEvent(GLFWwindow* window, int width, int height);
	friend void glfwOnMouseClick(GLFWwindow* window, int button, int action, int mods);
	friend void glfwOnMouseMovement(GLFWwindow* window, double xpos, double ypos);
	friend void glfwOnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

private:
	GLFWwindow* window;

	std::vector<ResizeListener*> resizeListeners;
	std::vector<MouseListener*> mouseListeners;
};
