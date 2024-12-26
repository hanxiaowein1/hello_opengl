#ifndef __CHARLES_OPENGL_CHAOS_SHADER_H__
#define __CHARLES_OPENGL_CHAOS_SHADER_H__

#include "glad/glad.h"
#include <string>
#include "glm/glm.hpp"
class Shader
{
public:
	unsigned int m_program_id;
	Shader(const char* vertex_shader_path, const char* fragment_shader_path);
	Shader() = default;
	~Shader() = default;
	void use();
	void set_uniform1(const std::string& name, bool value) const;
	void set_uniform1(const std::string& name, int value) const;
	void set_uniform1(const std::string& name, float value) const;
	void set_uniform3(const std::string& name, bool value1, bool value2, bool value3) const;
	void set_uniform3(const std::string& name, int value1, int value2, int value3) const;
	void set_uniform3(const std::string& name, float value1, float value2, float value3) const;
	void set_uniform4(const std::string& name, bool value1, bool value2, bool value3, bool value4) const;
	void set_uniform4(const std::string& name, int value1, int value2, int value3, int value4) const;
	void set_uniform4(const std::string& name, float value1, float value2, float value3, float value4) const;
	void set_vec3(const std::string &name, float x, float y, float z) const;
	void set_vec3(const std::string& name, const glm::vec3& value) const;
	void set_mat4(const std::string& name, const glm::mat4& value) const;
};

#endif
