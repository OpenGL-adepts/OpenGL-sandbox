#include "shader.hpp"
#include <cassert>
#include <fstream>
#include <memory>


void Shader::activate() const
{
	glUseProgram(mProgram);
}


void Shader::bind(unsigned int location, float value) const
{
	glUniform1f(location, value);
}


void Shader::bind(unsigned int location, int value) const
{
	glUniform1i(location, value);
}


void Shader::bind(unsigned int location, glm::vec3 const & vector) const
{
	glUniform3f(location, vector.x, vector.y, vector.z);
}


void Shader::bind(unsigned int location, glm::mat4 const & matrix) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


Shader& Shader::attach(std::string const & filename)
{
	// Load GLSL Shader Source from File
	std::string path = "";
	std::ifstream fd(path + filename);

	auto src = std::string(std::istreambuf_iterator<char>(fd),
						  (std::istreambuf_iterator<char>()));

	// Create a Shader Object
	const char * source = src.c_str();
	auto shader = create(filename);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &mStatus);

	// Display the Build Log on Error
	if (mStatus == false)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::unique_ptr<char[]> buffer(new char[length]);
		glGetShaderInfoLog(shader, length, nullptr, buffer.get());
		fprintf(stderr, "%s\n%s", filename.c_str(), buffer.get());
	}

	// Attach the Shader and Free Allocated Memory
	glAttachShader(mProgram, shader);
	glDeleteShader(shader);
	return *this;
}


GLuint Shader::create(std::string const & filename)
{
	auto index = filename.rfind(".");
	auto ext = filename.substr(index + 1);

		 if (ext == "comp") return glCreateShader(GL_COMPUTE_SHADER);
	else if (ext == "frag") return glCreateShader(GL_FRAGMENT_SHADER);
	else if (ext == "geom") return glCreateShader(GL_GEOMETRY_SHADER);
	else if (ext == "vert") return glCreateShader(GL_VERTEX_SHADER);
	else					return false;
}


Shader& Shader::link()
{
	glLinkProgram(mProgram);
	glGetProgramiv(mProgram, GL_LINK_STATUS, &mStatus);

	if(mStatus == false)
	{
		GLint length;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length);
		std::unique_ptr<char[]> buffer(new char[length]);
		glGetProgramInfoLog(mProgram, length, nullptr, buffer.get());
		fprintf(stderr, "%s", buffer.get());
	}

	assert(mStatus == true);
	return *this;
}

