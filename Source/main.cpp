#include "Window.h"
#include "Renderer.h"
#include "RigLoader.h"


int main(int argc, char** argv)
{
    Window window;
    Renderer renderer;
    RigLoader::LoadRig("../Assets/Idle.fbx");
	

    window.create("Skeletal Animator", 800, 600);

    while (window.isOpen())
    {    	
        renderer.update();

        window.render();
        window.update();
    }

    return 0;
}
