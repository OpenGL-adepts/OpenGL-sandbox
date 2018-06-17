#pragma once
#include <vector>
#include <string>


class CubeMap
{
public:
	CubeMap();
	~CubeMap();
	bool loadCubemap(const std::vector<std::string>& faces);
	void bind();
	bool loadTextureById(int _id);

protected:
	unsigned int m_textureId;
	int m_currentBackground;
};
