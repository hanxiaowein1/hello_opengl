#ifndef __CHARLES_OPENGL_TRIANGLES_SHOWER_H__
#define __CHARLES_OPENGL_TRIANGLES_SHOWER_H__

#include <vector>
#include <exception>
#include <string>
#include <unordered_map>
#include <cmath>

#include "opengl_chaos.h"
#include "chaos_camera.h"
#include "chaos_shader.h"
#include "chaos_shower.h"

template <typename V, typename T>
class DisplayInfo
{
public:
    std::vector<V> vertices;
    std::vector<V> normals;
    std::vector<T> triangles;
    float r, g, b;
};

extern Camera g_camera;
extern float deltaTime;  // Time between current frame and last frame
extern float lastFrame;  // Time of last frame

extern float g_last_x;
extern float g_last_y;
extern bool g_first_mouse;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput2(GLFWwindow* window);

template <typename V>
auto triangle_area(const V& v0, const V& v1, const V& v2) -> decltype(V::x)
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
std::vector<OpenGLVertex> convert_vertex(const std::vector<V>& vertices, const std::vector<V>& normals, const std::vector<T>& triangles)
{
    std::vector<OpenGLVertex> opengl_vertices;
    std::unordered_map<int, decltype(V::x)> triangle_area_map;
    std::unordered_map<int, std::vector<int>> vertex_surrounding_triangles;
    auto add_surrounding_triangle = [&](int vertex_index, int triangle_index) {
        if (vertex_surrounding_triangles.contains(vertex_index))
        {
            vertex_surrounding_triangles.at(vertex_index).emplace_back(triangle_index);
        }
        else
        {
            std::vector<int> surrounding_triangles{ triangle_index };
            vertex_surrounding_triangles.emplace(vertex_index, surrounding_triangles);
        }
    };
    for(int i = 0; i < triangles.size(); i++)
    {
        // compute triangle area
        const T& triangle = triangles[i];
        const V& v0 = vertices[triangle[0]];
        const V& v1 = vertices[triangle[1]];
        const V& v2 = vertices[triangle[2]];
        decltype(V::x) area = triangle_area(v0, v1, v2);
        triangle_area_map.emplace(i, area);

        add_surrounding_triangle(triangle[0], i);
        add_surrounding_triangle(triangle[1], i);
        add_surrounding_triangle(triangle[2], i);
    }

    std::vector<V> interpolated_norms;

    for(int i = 0; i < vertices.size(); i++)
    {
        auto surrounding_triangles = vertex_surrounding_triangles.at(i);
        V interpolated_norm;
        interpolated_norm.x = 0;
        interpolated_norm.y = 0;
        interpolated_norm.z = 0;
        decltype(V::x) total_area = 0.0f;
        for(int j = 0; j < surrounding_triangles.size(); j++)
        {
            auto normal = normals[surrounding_triangles[j]];
            auto area = triangle_area_map.at(surrounding_triangles[j]);
            interpolated_norm = interpolated_norm + normal * area;
            total_area = total_area + area;
        }
        interpolated_norm = interpolated_norm / total_area;
        interpolated_norms.emplace_back(interpolated_norm);
    }

    for(int i = 0; i < vertices.size(); i++)
    {
        auto vertex = vertices[i];
        auto normal = interpolated_norms[i];
        OpenGLVertex opengl_vertex;
        opengl_vertex.x = vertex.x;
        opengl_vertex.y = vertex.y;
        opengl_vertex.z = vertex.z;

        opengl_vertex.norm_x = normal.x;
        opengl_vertex.norm_y = normal.y;
        opengl_vertex.norm_z = normal.z;

        opengl_vertices.emplace_back(opengl_vertex);
    }

    return opengl_vertices;
}

template <typename T>
std::vector<unsigned int> convert_triangle(std::vector<T>& triangles)
{
    std::vector<unsigned int> indices;
    for(const auto& triangle: triangles)
    {
        for(const auto& vertex_index: triangle)
        {
            indices.emplace_back((unsigned int)vertex_index);
        }
    }
    return indices;
}

template <typename V, typename T>
void show_triangles(std::vector<DisplayInfo<V, T>> &mul_display_info)
{
    for (auto& display_info : mul_display_info)
    {
        if (display_info.normals.size() != display_info.triangles.size())
        {
            throw std::exception("normals triangles size doesn't equal!");
        }
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
    std::vector<ChaosShower> chaos_showers;
    for(auto display_info: mul_display_info)
    {
        std::vector<OpenGLVertex> opengl_vertices = convert_vertex(display_info.vertices, display_info.normals, display_info.triangles);
        std::vector<unsigned int> indices = convert_triangle(display_info.triangles);
        ChaosShower chaos_shower(opengl_vertices, indices, self_shader);
        chaos_showers.emplace_back(chaos_shower);
    }
    // std::vector<OpenGLVertex> vertices1{
	// 	{0, 0, 0, 0, 0, 1}, 
	// 	{0.5f, 0, 0, 0, 0, 1},
	// 	{0, 0.5f, 0, 0, 0, 1}
    // };
    // std::vector<unsigned int> indices1{
    //     0, 1, 2,
    // };
    // ChaosShower shower1(vertices1, indices1, self_shader);

    // std::vector<OpenGLVertex> vertices2{
	// 	{0.5f, 0, 0, 0.05f, 0.05f, -0.45f},
	// 	{0, 0.5f, 0, 0.05f, 0.05f, -0.45f},
	// 	{0.7f, 0.7f, 0.1f, 0.05f, 0.05f, -0.45f}
    // };
    // std::vector<unsigned int> indices2{
    //     0, 1, 2,
    // };
    // ChaosShower shower2(vertices2, indices2, self_shader);

	while (!glfwWindowShouldClose(window))
	{
		processInput2(window);

		glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = g_camera.get_view_matrix();
        auto projection = g_camera.get_projection_matrix();
        glm::mat4 model = glm::mat4(1.0f);

        self_shader.set_mat4("view", view);
        self_shader.set_mat4("projection", projection);
        self_shader.set_mat4("model", model);
		// shower1.shader.set_uniform3("objectColor", 1.0f, 0.0f, 0.0f);
		// shader.set_uniform3("objectColor", 0.5f, 0.5f, 0.5f);
        self_shader.set_uniform3("lightColor", 1.0f, 1.0f, 1.0f);
        self_shader.set_vec3("viewPos", g_camera.m_position);
        self_shader.set_uniform3("lightPos", 1.2f, 1.0f, 2.0f);
        for(int i = 0; i < chaos_showers.size(); i++)
        {
            auto& chaos_shower = chaos_showers[i];
            self_shader.set_uniform3("objectColor", mul_display_info[i].r, mul_display_info[i].g, mul_display_info[i].b);
            chaos_shower.show();
        }
		// shower1.show();
		// shower1.shader.set_uniform3("objectColor", 0.0f, 1.0f, 0.0f);
		// shower2.show();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

#endif