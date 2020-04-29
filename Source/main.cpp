#include "Window.h"
#include "Renderer.h"

int main(int argc, char** argv)
{
    Window window;
    Renderer renderer;

    window.create("Skeletal Animator", 800, 600);

    while (window.isOpen())
    {
        renderer.update();

        window.render();
        window.update();
    }

    return 0;
}
