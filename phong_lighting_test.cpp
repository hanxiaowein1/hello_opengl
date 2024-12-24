#include "unit_test.h"

#include "opengl_chaos.h"
#include "chaos_shader.h"
#include "chaos_camera.h"
#include "chaos_shower.h"

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

TEST(GlobalTest, phong_lighting_test)
{

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