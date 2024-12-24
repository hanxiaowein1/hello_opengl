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

ChaosShower::ChaosShower(std::vector<OpenGLVertex>& vertices, std::vector<unsigned int>& indices, Shader& shader)
{
    this->vertices_with_norm = std::move(vertices);
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
    glBufferData(GL_ARRAY_BUFFER, this->vertices_with_norm.size() * sizeof(Vertex), this->vertices_with_norm.data(), GL_STATIC_DRAW);

    // copy indices from cpu to gpu
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

    // set rule to parse vertices
    // rule 1: parse position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // rule 2: parse normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
}


void ChaosShower::show()
{
    this->shader.use();
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}