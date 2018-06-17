#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>


class Shader
{
public:
	unsigned int ID;
	// Implement Custom Constructor and Destructor
	Shader() { mProgram = glCreateProgram(); }
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	~Shader() { glDeleteProgram(mProgram); }

	// Public Member Functions
	void activate() const;
	void clearProgram();
	Shader& attach(std::string const& filename);
	GLuint create(std::string const& filename);
	GLuint get() const { return mProgram; }
	Shader& link();

	// Wrap Calls to glUniform
	void bind(unsigned int location, float value) const;
	void bind(unsigned int location, int value) const;
	void bind(unsigned int location, glm::vec3 const & vector) const;
	void bind(unsigned int location, glm::mat4 const & matrix) const;

	template<typename T>
	const Shader& bind(std::string const & name, T&& value) const
	{
		int location = glGetUniformLocation(mProgram, name.c_str());

		if (location == -1)
			fprintf(stderr, "Missing Uniform: %s\n", name.c_str());
		else
			bind(location, std::forward<T>(value));

		return *this;
	}

	void use();
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const;
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const;
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const;
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
	// Disable Copying and Assignment
	Shader(Shader const &) = delete;
	void checkCompileErrors(GLuint shader, std::string type);
	Shader & operator=(Shader const &) = delete;

	// Private Member Variables
	GLuint mProgram;
	GLint  mStatus;

};
