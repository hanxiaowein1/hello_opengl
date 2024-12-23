#include <iostream>

#include "unit_test.h"

#include "chaos.h"
#include "chaos_shader.h"
#include "chaos_shader.h"
#include "chaos_shower.h"
#include "chaos_camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera g_camera(
	CameraScene(45.0f, 800.0f / 600.0f, 0.1f, 100.0f),
	glm::vec3(0.0f, 0.0f, 3.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	-90.0f, 0.0f,
	45.0f);

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float g_last_x = 400, g_last_y = 300;
bool g_first_mouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (g_first_mouse) // initially set to true
	{
		g_last_x = xpos;
		g_last_y = ypos;
		g_first_mouse = false;
	}

	float xoffset = xpos - g_last_x;
	float yoffset = g_last_y - ypos; // reversed: y ranges bottom to top
	g_last_x = xpos;
	g_last_y = ypos;
	g_camera.process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	g_camera.process_mouse_scroll(yoffset);
}

void processInput2(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		g_camera.process_keyboard(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		g_camera.process_keyboard(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		g_camera.process_keyboard(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		g_camera.process_keyboard(CameraMovement::RIGHT, deltaTime);
	}
}

TEST(GlobalTest, camera_move_test)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
        "D:\\Projects\\hello_opengl\\shader\\mvp_test_shader_vs.glsl",
        "D:\\Projects\\hello_opengl\\shader\\mvp_test_shader_fs.glsl"
    );
    std::vector<float> vertices1{
		0, 0, 0,
		0.5f, 0, 0,
		0, 0.5f, 0,
    };
    std::vector<unsigned int> indices1{
        0, 1, 2,
    };
    ChaosShower shower1(vertices1, indices1, self_shader);

    Shader self_shader2(
        "D:\\Projects\\hello_opengl\\shader\\mvp_test_shader_vs.glsl",
        "D:\\Projects\\hello_opengl\\shader\\mvp_test_shader_fs_2.glsl"
    );
    std::vector<float> vertices2{
		0.5f, 0, 0,
		0, 0.5f, 0,
		//0.7f, 0.7f, 0.3f
		//0.7f, 0.7f, -300.0f
		0.7f, 0.7f, 0.1f
    };
    std::vector<unsigned int> indices2{
        0, 1, 2,
    };
    ChaosShower shower2(vertices2, indices2, self_shader2);

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
		shower1.show();
        // glDrawArrays(GL_TRIANGLES, 0, 36);
		shower2.shader.set_mat4("view", view);
		shower2.shader.set_mat4("projection", projection);
		shower2.shader.set_mat4("model", model);
		shower2.show();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}