#include "triangles_shower.h"

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