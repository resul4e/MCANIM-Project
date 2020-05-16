#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "RigLoader.h"

int main(int argc, char** argv)
{
    Window window;
    Renderer renderer;
    RigLoader::LoadRig("../Assets/Idle.fbx");

    // Something is wrong with unlocking opengl functionality above the fixed function pipeline.
    // Texture texture("../Assets/Universiteit-Utrecht.png");
    // Shader shader("../Assets/shader.shader");

    // renderer.SetupQuad();

    window.create("Skeletal Animator", 800, 600);

    while (window.isOpen())
    {    	
        renderer.update();
        
        window.render();
        window.update();
    }

    return 0;
}
