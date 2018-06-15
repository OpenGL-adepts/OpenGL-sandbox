#include "texture.hpp"


Texture::Texture()
{
	glGenTextures(1, &m_textureId);
}


Texture::Texture(const std::string& _file)
	: Texture()
{
	loadFromFile(_file);
}


Texture::Texture(Texture&& _move)
	: m_textureId(_move.m_textureId)
{
	_move.m_textureId = 0;
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_textureId);
}


void Texture::createColorPlaceholder()
{
	bind();
	unsigned char color[3] = { 255, 255, 255 };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, color);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::changeColor(glm::vec3 color)
{
	bind();
	unsigned char c[3] = { color.r, color.g, color.b };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, c);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::loadFromFile(const std::string& _file)
{
	int width, height, channels;

	if(auto* data = stbi_load(_file.c_str(), &width, &height, &channels, 0))
	{
		GLenum format = GL_RED;

		switch (channels)
		{
			case 1 : format = GL_ALPHA;		break;
			case 2 : format = GL_LUMINANCE; break;
			case 3 : format = GL_RGB;		break;
			case 4 : format = GL_RGBA;		break;
		}

		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		std::cerr << "Failed to load texture \"" << _file << "\"\n";

	return true;
}


void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}
