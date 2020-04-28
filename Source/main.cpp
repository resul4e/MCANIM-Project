#include "Window.h"

int main(int argc, char** argv)
{
    Window window;

    window.create("Skeletal Animator", 800, 600);

    while (window.isOpen())
    {
        //glClearColor(1, 0, 0, 1);
        //glClear(GL_COLOR_BUFFER_BIT);

        window.render();
        window.update();
    }

    return 0;
}
