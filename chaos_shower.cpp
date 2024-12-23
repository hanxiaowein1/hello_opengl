#include "chaos_shower.h"

ChaosShower::ChaosShower(std::vector<float>& vertices, std::vector<unsigned int>& indices, Shader& shader)
{
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->shader = shader;

    // allocate vao
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // allocate vbo
    glGenBuffers(1, &this->vbo);

    // allocate ebo
    glGenBuffers(1, &this->ebo);

    // copy vertices from cpu to gpu
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

    // copy indices from cpu to gpu
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

    // set rule to parse vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void ChaosShower::show()
{
    this->shader.use();
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}