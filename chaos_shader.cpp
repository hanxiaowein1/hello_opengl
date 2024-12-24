#include "chaos_shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <format>
#include "opengl_chaos.h"
#include "chaos_shader.h"

Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path)
{
	// get shader code from file
	std::string vertex_code;
	std::string fragment_code;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		v_shader_file.open(vertex_shader_path);
		f_shader_file.open(fragment_shader_path);
		std::stringstream v_shader_stream, f_shader_stream;
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();
		v_shader_file.close();
		f_shader_file.close();
		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();

	}
	catch (const std::ifstream::failure &e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	// generate vertex shader and fragment shader
	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();
	unsigned int vertex_shader, fragment_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &v_shader_code, NULL);
	glCompileShader(vertex_shader);
	try
	{
		check_shader_compile_status(vertex_shader, "ERROR::SHADER:VERTEX::COMPILATION_FAILED\n");
	}
	catch(const std::exception& e)
	{
		throw std::exception(std::format("vertex shader compile failed, vertex shader path: {}", vertex_shader_path).c_str());
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &f_shader_code, NULL);
	glCompileShader(fragment_shader);
	try
	{
		check_shader_compile_status(vertex_shader, "ERROR::SHADER:FRAGMENT::COMPILATION_FAILED\n");
	}
	catch(const std::exception& e)
	{
		throw std::exception(std::format("fragment shader compile failed, fragment shader path: {}", fragment_shader_path).c_str());
	}

	// shader program
	this->m_program_id = glCreateProgram();
	glAttachShader(this->m_program_id, vertex_shader);
	glAttachShader(this->m_program_id, fragment_shader);
	glLinkProgram(this->m_program_id);
	try
	{
		check_program_link_status(this->m_program_id, "ERROR::SHADER::PROGRAM::LINK_FAILED\n");
	}
	catch(const std::exception& e)
	{
		throw std::exception(std::format("shader program link failed, vertex shader path: {}, fragment shader path: {}", vertex_shader_path, fragment_shader_path).c_str());
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Shader::use()
{
	glUseProgram(this->m_program_id);
}

void Shader::set_uniform1(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->m_program_id, name.c_str()), (int)value);
}
void Shader::set_uniform1(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->m_program_id, name.c_str()), value);
}
void Shader::set_uniform1(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->m_program_id, name.c_str()), value);
}

void Shader::set_uniform3(const std::string& name, bool value1, bool value2, bool value3) const
{
	glUniform3i(glGetUniformLocation(this->m_program_id, name.c_str()), (int)value1, (int)value2, (int)value3);

}
void Shader::set_uniform3(const std::string& name, int value1, int value2, int value3) const
{
	glUniform3i(glGetUniformLocation(this->m_program_id, name.c_str()), value1, value2, value3);
}
void Shader::set_uniform3(const std::string& name, float value1, float value2, float value3) const
{
	glUniform3f(glGetUniformLocation(this->m_program_id, name.c_str()), value1, value2, value3);
}

void Shader::set_uniform4(const std::string& name, bool value1, bool value2, bool value3, bool value4) const
{
	glUniform4i(glGetUniformLocation(this->m_program_id, name.c_str()), (int)value1, (int)value2, (int)value3, (int)value4);

}
void Shader::set_uniform4(const std::string& name, int value1, int value2, int value3, int value4) const
{
	glUniform4i(glGetUniformLocation(this->m_program_id, name.c_str()), value1, value2, value3, value4);
}
void Shader::set_uniform4(const std::string& name, float value1, float value2, float value3, float value4) const
{
	glUniform4f(glGetUniformLocation(this->m_program_id, name.c_str()), value1, value2, value3, value4);
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(this->m_program_id, name.c_str()), 1, &value[0]);
}

void Shader::set_mat4(const std::string& name, const glm::mat4& value) const
{
	unsigned int location = glGetUniformLocation(this->m_program_id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

int shader_class_demo()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// VBO
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// copy vertices to buffer for opengl to use later
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader our_shader("D:\\PHD\\Projects\\hello_opengl\\shader\\6_7_shader.vs",
		"D:\\PHD\\Projects\\hello_opengl\\shader\\6_7_shader.fs");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.2f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		//glUseProgram(shaderProgram);
		our_shader.use();

		// update the uniform color
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		our_shader.set_uniform4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		//int vertexColorLocation = glGetUniformLocation(shaderProgram,
		//	"ourColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}
