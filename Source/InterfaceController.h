#pragma once
#include "Window.h"

class InterfaceController
{
public:
	void Setup(Window window);
	void Begin();
	void End();
	void ShutDown();
};