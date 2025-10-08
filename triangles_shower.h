#ifndef __CHARLES_OPENGL_TRIANGLES_SHOWER_H__
#define __CHARLES_OPENGL_TRIANGLES_SHOWER_H__

#include <vector>
#include <exception>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

extern float g_yaw;
extern float g_pitch;

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_control_distance(GLFWwindow* window, double xoffset, double yoffset);
void mouse_move_rotate(GLFWwindow* window, double xpos, double ypos);

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

    std::unordered_set<int> reduntant_vertices;
    std::vector<V> interpolated_norms;
    for(int i = 0; i < vertices.size(); i++)
    {
        V interpolated_norm;
        if(!vertex_surrounding_triangles.contains(i))
        {
            // reduntant vertex, no face has this vertex
            reduntant_vertices.emplace(i);
            // emplace an empty interpolated norm which will never be used
            interpolated_norms.emplace_back(interpolated_norm);
            continue;
        }
        auto surrounding_triangles = vertex_surrounding_triangles.at(i);
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

/**
 * @brief this is more useful to game scene, not many useful to model viewer.
 * 
 * @tparam V 
 * @tparam T 
 * @param mul_display_info 
 */
template <typename V, typename T>
void show_triangles_with_game_player(std::vector<DisplayInfo<V, T>> &mul_display_info)
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
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
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
        "D:\\Projects\\hello_opengl\\shader\\6.multiple_lights_vs.glsl",
        "D:\\Projects\\hello_opengl\\shader\\6.multiple_lights_fs.glsl"
    );
    std::vector<ChaosShower> chaos_showers;
    for(auto display_info: mul_display_info)
    {
        std::vector<OpenGLVertex> opengl_vertices = convert_vertex(display_info.vertices, display_info.normals, display_info.triangles);
        std::vector<unsigned int> indices = convert_triangle(display_info.triangles);
        ChaosShower chaos_shower(opengl_vertices, indices, self_shader);
        chaos_showers.emplace_back(chaos_shower);
    }

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.0f,  -100.0f, 0.0f),
        glm::vec3(0, 50, 100),
        glm::vec3(87, 50, -50),
        glm::vec3(-87,  50, -50)
    };

	while (!glfwWindowShouldClose(window))
	{
		processInput2(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = g_camera.get_view_matrix();
        auto projection = g_camera.get_projection_matrix();
        glm::mat4 model = glm::mat4(1.0f);

        self_shader.set_mat4("view", view);
        self_shader.set_mat4("projection", projection);
        self_shader.set_mat4("model", model);


        // self_shader.set_uniform3("lightColor", 1.0f, 1.0f, 1.0f);
        self_shader.set_vec3("viewPos", g_camera.m_position);
        // self_shader.set_uniform3("lightPos", 1.2f, 1.0f, 2.0f);


        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index 
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        self_shader.set_vec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        self_shader.set_vec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        self_shader.set_vec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        self_shader.set_vec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        self_shader.set_vec3("pointLights[0].position", pointLightPositions[0]);
        self_shader.set_vec3("pointLights[0].ambient", 0.2f, 0.2f, 0.2f);
        self_shader.set_vec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        self_shader.set_vec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        self_shader.set_uniform1("pointLights[0].constant", 1.0f);
        self_shader.set_uniform1("pointLights[0].linear", 0.09f);
        self_shader.set_uniform1("pointLights[0].quadratic", 0.032f);
        // point light 2
        self_shader.set_vec3("pointLights[1].position", pointLightPositions[1]);
        self_shader.set_vec3("pointLights[1].ambient", 0.2f, 0.2f, 0.2f);
        self_shader.set_vec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        self_shader.set_vec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        self_shader.set_uniform1("pointLights[1].constant", 1.0f);
        self_shader.set_uniform1("pointLights[1].linear", 0.09f);
        self_shader.set_uniform1("pointLights[1].quadratic", 0.032f);
        // point light 3
        self_shader.set_vec3("pointLights[2].position", pointLightPositions[2]);
        self_shader.set_vec3("pointLights[2].ambient", 0.2f, 0.2f, 0.2f);
        self_shader.set_vec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        self_shader.set_vec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        self_shader.set_uniform1("pointLights[2].constant", 1.0f);
        self_shader.set_uniform1("pointLights[2].linear", 0.09f);
        self_shader.set_uniform1("pointLights[2].quadratic", 0.032f);
        // point light 4
        self_shader.set_vec3("pointLights[3].position", pointLightPositions[3]);
        self_shader.set_vec3("pointLights[3].ambient", 0.2f, 0.2f, 0.2f);
        self_shader.set_vec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        self_shader.set_vec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        self_shader.set_uniform1("pointLights[3].constant", 1.0f);
        self_shader.set_uniform1("pointLights[3].linear", 0.09f);
        self_shader.set_uniform1("pointLights[3].quadratic", 0.032f);
        // spotLight
        self_shader.set_vec3("spotLight.position", g_camera.m_position);
        self_shader.set_vec3("spotLight.direction", g_camera.m_front);
        self_shader.set_vec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        self_shader.set_vec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        self_shader.set_vec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        self_shader.set_uniform1("spotLight.constant", 1.0f);
        self_shader.set_uniform1("spotLight.linear", 0.09f);
        self_shader.set_uniform1("spotLight.quadratic", 0.032f);
        self_shader.set_uniform1("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        self_shader.set_uniform1("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

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

/**
 * @brief in model viewer, all we need is rotate model, and use scroll to move close and far
 * 
 * @tparam V 
 * @tparam T 
 * @param mul_display_info 
 */
template <typename V, typename T>
void show_triangles_with_model_viewer(std::vector<DisplayInfo<V, T>> &mul_display_info)
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
	glfwSetCursorPosCallback(window, mouse_move_rotate);
	glfwSetScrollCallback(window, scroll_control_distance);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
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

    auto print_mat4 = [](const glm::mat4& mat) {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout << mat[i][j] << " ";
            }
            std::cout << std::endl;
        }
    };

    auto print_vec3 = [](const glm::vec3& vec) {
        for (int i = 0; i < 3; i++)
        {
            std::cout << vec[i] << " ";
        }
    };

    auto print_vec4 = [](const glm::vec4& vec) {
        for (int i = 0; i < 4; i++)
        {
            std::cout << vec[i] << " ";
        }
    };

	while (!glfwWindowShouldClose(window))
	{
		processInput2(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = g_camera.get_view_matrix();
        // std::cout << "view:" << std::endl;
        // print_mat4(view);
        // std::cout << std::endl;
        auto projection = g_camera.get_projection_matrix();
        glm::mat4 model = glm::mat4(1.0f);

        // auto view_inverse = glm::inverse(view);
        // std::cout << "view inverse:" << std::endl;
        // print_mat4(view_inverse);
        // std::cout << std::endl;

        // auto yaw_vec_temp = view_inverse * glm::vec4(g_camera.m_up.x, g_camera.m_up.y, g_camera.m_up.z, 0.0f);
        // std::cout << "yaw_vec_temp:" << std::endl;
        // print_vec4(yaw_vec_temp);
        // std::cout << std::endl;

        // glm::vec3 yaw_vec(yaw_vec_temp.x, yaw_vec_temp.y, yaw_vec_temp.z);
        // std::cout << "yaw_vec:" << std::endl;
        // print_vec3(yaw_vec);
        // std::cout << std::endl;

        //model = glm::rotate(model, glm::radians(g_yaw), yaw_vec);
        // model = glm::rotate(model, glm::radians(g_yaw), g_camera.m_up);
        // auto pitch_vec_temp = view_inverse * glm::vec4(g_camera.m_right.x, g_camera.m_right.y, g_camera.m_right.z, 0.0f);
        // std::cout << "pitch_vec_temp:" << std::endl;
        // print_vec4(pitch_vec_temp);
        // std::cout << std::endl;
        // glm::vec3 pitch_vec(pitch_vec_temp.x, pitch_vec_temp.y, pitch_vec_temp.z);
        //model = glm::rotate(model, glm::radians(g_pitch), pitch_vec);

        // model = glm::rotate(model, glm::radians(g_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        // std::cout << "g_camera.m_right:" << std::endl;
        // print_vec3(g_camera.m_right);
        // std::cout << std::endl;
        // model = glm::rotate(model, glm::radians(g_pitch), g_camera.m_right);

        glm::mat4 rotation_yaw_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(g_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotation_pitch_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(g_pitch), g_camera.m_right);
        model = rotation_pitch_matrix * rotation_yaw_matrix * model;

        self_shader.set_mat4("view", view);
        self_shader.set_mat4("projection", projection);
        self_shader.set_mat4("model", model);

        self_shader.set_uniform3("lightColor", 1.0f, 1.0f, 1.0f);
        self_shader.set_vec3("viewPos", g_camera.m_position);
        //self_shader.set_vec3("lightPos", light_pos);
        // self_shader.set_uniform3("lightPos", 1.2f, 1.0f, 2.0f);

        for(int i = 0; i < chaos_showers.size(); i++)
        {
            auto& chaos_shower = chaos_showers[i];
            self_shader.set_uniform3("objectColor", mul_display_info[i].r, mul_display_info[i].g, mul_display_info[i].b);
            // chaos_shower.show();
            chaos_shower.shader.use();
            glBindVertexArray(chaos_shower.vao);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, chaos_shower.indices.size(), GL_UNSIGNED_INT, 0);

            // draw edges
            self_shader.set_uniform3("objectColor", 0.0f, 0.0f, 0.0f);
            glLineWidth(2.5);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, chaos_shower.indices.size(), GL_UNSIGNED_INT, 0);
        }

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

#endif