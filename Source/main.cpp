#include "Window.h"
#include "Renderer.h"
#include "RigLoader.h"
#include "Rig.h"
#include "AnimationLoader.h"

int main(int argc, char** argv)
{
    Window window;
    Renderer renderer;

	//loading the test rig.
    std::shared_ptr<Rig> rig = RigLoader::LoadRig("../Assets/Idle.fbx");
    std::shared_ptr<AnimationClip> anim = AnimationLoader::LoadAnimation("../Assets/Idle.fbx");
	

    window.create("Skeletal Animator", 800, 600);
    renderer.SetupQuad();

    while (window.isOpen())
    {    	
        renderer.update();
        
        window.render();
        window.update();
    }

    return 0;
}
