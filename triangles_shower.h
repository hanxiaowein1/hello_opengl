#ifndef __CHARLES_OPENGL_TRIANGLES_SHOWER_H__
#define __CHARLES_OPENGL_TRIANGLES_SHOWER_H__

#include <vector>
#include <exception>
#include <string>
#include <unordered_map>

#include "opengl_chaos.h"
#include "chaos_camera.h"
#include "chaos_shader.h"
#include "chaos_shower.h"

extern Camera g_camera;
extern float deltaTime;  // Time between current frame and last frame
extern float lastFrame;  // Time of last frame

extern float g_last_x;
extern float g_last_y;
extern bool g_first_mouse;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput2(GLFWwindow* window);

template <typename V, typename VType>
VType triangle_area(const V& v0, const V& v1, const V& v2)
{
    V vec0 = v1 - v0;
    V vec1 = v2 - v0;
    V normal = vec1.cross(vec0);
    decltype(V::x) area = 0.5f * normal.len();
    return area;
}

/**
 * @brief 
 * 
 * @tparam V vertex type(must has access of x/y/z, cross(), len(), operator-() functions)
 * @tparam T triangle type(recommend std::vector<int>)
 * @param vertices 
 * @param normals 
 * @param triangles index of vertex, start from 0
 * @return std::vector<OpenGLVertex> 
 */
template <typename V, typename T>
std::vector<OpenGLVertex> convert_vertex(std::vector<V>& vertices, std::vector<V>& normals, std::vector<T>& triangles)
{
    std::vector<OpenGLVertex> ret;
    std::unordered_map<int, decltype(V::x)> triangle_area_map;
    std::unordered_map<int, std::vector<int>> vertex_surrounding_triangle;
    for(int i = 0; i < triangles.size(); i++)
    {
        // compute triangle area
        const T& triangle = triangles[i];
        const V& v0 = vertices[triangle[0]];
        const V& v1 = vertices[triangle[1]];
        const V& v2 = vertices[triangles[2]];
        decltype(V::x) area = triangle_area<V, decltype<V::x>>(v0, v1, v2);
        triangle_area_map.emplace(i, area);

        int v0_idx = triangle[0];
        vertex_surrounding_triangle.emplace(v0_idx, i);
        int v1_idx = triangle[1];
        vertex_surrounding_triangle.emplace(v1_idx, i);
        int v2_idx = triangle[2];
        vertex_surrounding_triangle.emplace(v2_idx, i);
    }

    for(int i = 0; i < vertices.size(); i++)
    {
        // TODO: calculate normal vector for each vertex
    }
}

template <typename V, typename T>
void show_triangles(std::vector<V>& vertices, std::vector<V>& normals, std::vector<T>& triangles)
{
    if(normals.size() != triangles.size())
    {
        throw std::exception("normals triangles size doesn't equal!")
    }
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "LearnOpenGL", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	if (window == NULL)
	{
        std::string err_msg = "Failed to create GLFW window";
		std::cout << err_msg << std::endl;
		glfwTerminate();
        throw std::exception(err_msg.c_str());
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
        std::string err_msg = "Failed to initialize GLAD";
		std::cout << err_msg << std::endl;
		throw std::exception(err_msg.c_str());
	}
	glEnable(GL_DEPTH_TEST);

	Shader self_shader(
        "D:\\Projects\\hello_opengl\\shader\\phong_lighting_vs.glsl",
        "D:\\Projects\\hello_opengl\\shader\\phong_lighting_fs.glsl"
    );
    std::vector<OpenGLVertex> vertices1{
		{0, 0, 0, 0, 0, 1}, 
		{0.5f, 0, 0, 0, 0, 1},
		{0, 0.5f, 0, 0, 0, 1}
    };
    std::vector<unsigned int> indices1{
        0, 1, 2,
    };
    ChaosShower shower1(vertices1, indices1, self_shader);

    std::vector<OpenGLVertex> vertices2{
		{0.5f, 0, 0, 0.05f, 0.05f, -0.45f},
		{0, 0.5f, 0, 0.05f, 0.05f, -0.45f},
		{0.7f, 0.7f, 0.1f, 0.05f, 0.05f, -0.45f}
    };
    std::vector<unsigned int> indices2{
        0, 1, 2,
    };
    ChaosShower shower2(vertices2, indices2, self_shader);

	while (!glfwWindowShouldClose(window))
	{
		processInput2(window);

		glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = g_camera.get_view_matrix();
        auto projection = g_camera.get_projection_matrix();
        glm::mat4 model = glm::mat4(1.0f);

        shower1.shader.set_mat4("view", view);
        shower1.shader.set_mat4("projection", projection);
        shower1.shader.set_mat4("model", model);
		// shower1.shader.set_uniform3("objectColor", 1.0f, 0.0f, 0.0f);
		shower1.shader.set_uniform3("objectColor", 0.5f, 0.5f, 0.5f);
		shower1.shader.set_uniform3("lightColor", 1.0f, 1.0f, 1.0f);
		shower1.shader.set_vec3("viewPos", g_camera.m_position);
		shower1.shader.set_uniform3("lightPos", 1.2f, 1.0f, 2.0f);
		shower1.show();
        // glDrawArrays(GL_TRIANGLES, 0, 36);
		shower1.shader.set_uniform3("objectColor", 0.0f, 1.0f, 0.0f);
		shower2.show();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

#endif