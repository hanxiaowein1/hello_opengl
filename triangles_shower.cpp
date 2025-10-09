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
bool g_track_mouse = true;

float g_yaw = -90.0f;
float g_pitch = 0.0f;
float g_sensitivity = 0.1f;

float ModelViewerHandle::s_last_x = 400;
float ModelViewerHandle::s_last_y = 300;
float ModelViewerHandle::s_yaw = -90.0f;
float ModelViewerHandle::s_pitch = 0.0f;
bool ModelViewerHandle::s_track_mouse = true;


std::tuple<float, float> mouse_move_handle(GLFWwindow* window, double xpos, double ypos)
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
	return {xoffset, yoffset};
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!g_track_mouse)
	{
		g_last_x = xpos;
		g_last_y = ypos;
		return;
	}
	auto [xoffset, yoffset] = mouse_move_handle(window, xpos, ypos);
	g_camera.process_mouse_movement(xoffset, yoffset);
}

void mouse_move_rotate(GLFWwindow* window, double xpos, double ypos)
{
	if (!g_track_mouse)
	{
		g_last_x = xpos;
		g_last_y = ypos;
		return;
	}
	auto [xoffset, yoffset] = mouse_move_handle(window, xpos, ypos);
    g_yaw -= xoffset;
    g_pitch -= yoffset;

    // 限制 pitch 范围
    if (g_pitch > 89.0f)
	{
        g_pitch = 89.0f;
	}
    if (g_pitch < -89.0f)
	{
        g_pitch = -89.0f;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	g_camera.process_mouse_scroll(yoffset);
}

void scroll_control_distance(GLFWwindow* window, double xoffset, double yoffset)
{
	g_camera.scorll_control_position(yoffset);
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        // Code to handle right - click press event
		// do not track mouse move event
		g_track_mouse = false;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Code to handle right - click release event
		// start track mouse move event
		g_track_mouse = true;
    }
}

void ModelViewerHandle::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void ModelViewerHandle::mouse_move_rotate(GLFWwindow* window, double xpos, double ypos)
{
	if (!s_track_mouse)
	{
		s_last_x = xpos;
		s_last_y = ypos;
		return;
	}
	auto [xoffset, yoffset] = mouse_move_handle(window, xpos, ypos);
    s_yaw -= xoffset;
    s_pitch -= yoffset;

    // 限制 pitch 范围
    if (s_pitch > 89.0f)
	{
        s_pitch = 89.0f;
	}
    if (s_pitch < -89.0f)
	{
        s_pitch = -89.0f;
	}
}

void ModelViewerHandle::scroll_control_distance(GLFWwindow* window, double xoffset, double yoffset)
{
	g_camera.scorll_control_position(yoffset);
}

void ModelViewerHandle::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        // Code to handle right - click press event
		// do not track mouse move event
		s_track_mouse = false;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // Code to handle right - click release event
		// start track mouse move event
		s_track_mouse = true;
    }
}
