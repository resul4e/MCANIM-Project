#include "Renderer.h"
#include <glad/glad.h>

void Renderer::update()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->Bind();
    texture->Bind();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    shader->UnBind();
    texture->UnBind();
}

void Renderer::SetupQuad()
{
    shader = new Shader("../Assets/shader.shader");
    texture = new Texture("../Assets/image.png");

    vertices[0] = 0.5f;
    vertices[1] = 0.75f;
    vertices[2] = 0.0f;
    vertices[3] = 1.0f;
    vertices[4] = 1.0f;

    vertices[5] = 0.5f;
    vertices[6] = -0.75f;
    vertices[7] = 0.0f;
    vertices[8] = 1.0f;
    vertices[9] = -.0f;

    vertices[10] = -0.5f;
    vertices[11] = -0.75f;
    vertices[12] = 0.0f;
    vertices[13] = -.0f;
    vertices[14] = -.0f;

    vertices[15] = -0.5f;
    vertices[16] = 0.75f;
    vertices[17] = 0.0f;
    vertices[18] = 0.0f;
    vertices[19] = 1.0f;

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 3;
    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

