#ifndef _CHARLES_OPENGL_CHAOS_H__
#define _CHARLES_OPENGL_CHAOS_H__
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void check_shader_compile_status(unsigned int shader_id, const char* append_shader_info);

void check_program_link_status(unsigned int program_id, const char* append_info);


#endif