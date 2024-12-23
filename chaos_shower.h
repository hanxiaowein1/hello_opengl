#ifndef __CHARLES_CHAOS_SHOWER_H__
#define __CHARLES_CHAOS_SHOWER_H__

#include <vector>
#include "chaos_shader.h"

class ChaosShower
{
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Shader shader;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
public:
    ChaosShower(std::vector<float>& vertices, std::vector<unsigned int>& indices, Shader& shader);
    void show();
};


#endif