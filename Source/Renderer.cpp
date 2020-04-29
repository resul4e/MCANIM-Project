#include "Renderer.h"

#include <glad/glad.h>

void Renderer::update()
{
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}
