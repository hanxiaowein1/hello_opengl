#include "chaos.h"
#include <iostream>
#include <format>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void check_shader_compile_status(unsigned int shader_id, const char* append_shader_info)
{
	int shader_success;
	char shader_info_log[512];
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_success);
	if (!shader_success)
	{
		glGetShaderInfoLog(shader_id, 512, NULL, shader_info_log);
		std::cout << append_shader_info << shader_info_log << std::endl;
		throw std::exception(std::format("sharder id {} compile failed", shader_id).c_str());
	}
}

void check_program_link_status(unsigned int program_id, const char* append_info)
{
	int success;
	char info_log[512];
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program_id, 512, NULL, info_log);
		std::cout << append_info << info_log << std::endl;
		throw std::exception(std::format("sharder id {} link failed", program_id).c_str());
	}
}
