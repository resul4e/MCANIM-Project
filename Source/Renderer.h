#pragma once
#include "Shader.h"
#include "Texture.h"

class Renderer
{
public:
    void update();
    void SetupQuad();
private:
    float vertices[20];
    int indices[6];

    Shader* shader;
    Texture *texture;

    unsigned int VBO, VAO, EBO;
};
