#pragma once

#include <string>


class Texture
{
public:
	Texture();
	explicit Texture(const std::string& _file);
	Texture(Texture&& _move);
	~Texture();

	// Forbid copy
	Texture(const Texture& _copy) = delete;
	Texture& operator=(const Texture& _copy) = delete;

	void createColorPlaceholder();
	bool loadFromFile(const std::string& _file);
	void bind() const;

protected:
	unsigned int m_textureId;

};
