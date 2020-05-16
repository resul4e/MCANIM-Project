﻿#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "RigLoader.h"

int main(int argc, char** argv)
{
    Window window;
    Renderer renderer;
    RigLoader::LoadRig("../Assets/Idle.fbx");

    // renderer.SetupQuad();

    window.create("Skeletal Animator", 800, 600);

    Shader shader("../Assets/shader.shader");
    Texture texture("../Assets/Universiteit-Utrecht.png");

    while (window.isOpen())
    {    	
        renderer.update();
        
        window.render();
        window.update();
    }

    return 0;
}
