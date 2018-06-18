#include "cubemap.hpp"
#include "config.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


CubeMap::CubeMap()
{
	m_currentBackground = -1;
	glGenTextures(1, &m_textureId);
}


CubeMap::~CubeMap()
{
	glDeleteTextures(1, &m_textureId);
}


bool CubeMap::loadCubemap(const std::vector<std::string>& faces)
{
	bind();

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i].c_str() << std::endl;
			stbi_image_free(data);
			return false;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return true;
}


void CubeMap::bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
}


bool CubeMap::loadTextureById(int _id)
{
	if(_id == m_currentBackground)
		return true;

	std::vector<std::string> faces;

	switch (_id)
	{
	case 0:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/right.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/left.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/top.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/bottom.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/front.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/back.jpg",
		};
		break;
	case 1:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/darkskies/darkskies_ft.tga",
			RELATIVE_PATH_ROOT "/resources/skyboxes/darkskies/darkskies_bk.tga",
			RELATIVE_PATH_ROOT "/resources/skyboxes/darkskies/darkskies_up.tga",
			RELATIVE_PATH_ROOT "/resources/skyboxes/darkskies/darkskies_dn.tga",
			RELATIVE_PATH_ROOT "/resources/skyboxes/darkskies/darkskies_rt.tga",
			RELATIVE_PATH_ROOT "/resources/skyboxes/darkskies/darkskies_lf.tga",
		};
		break;
	case 2:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/NissiBeach/posx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/NissiBeach/negx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/NissiBeach/posy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/NissiBeach/negy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/NissiBeach/posz.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/NissiBeach/negz.jpg",
		};
		break;
	case 3:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/SaintPetersSquare/posx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SaintPetersSquare/negx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SaintPetersSquare/posy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SaintPetersSquare/negy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SaintPetersSquare/posz.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SaintPetersSquare/negz.jpg",
		};
		break;
	case 4:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/SantaMariaDeiMiracoli/posx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SantaMariaDeiMiracoli/negx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SantaMariaDeiMiracoli/posy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SantaMariaDeiMiracoli/negy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SantaMariaDeiMiracoli/posz.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/SantaMariaDeiMiracoli/negz.jpg",
		};
		break;
	case 5:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/Storforsen/posx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Storforsen/negx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Storforsen/posy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Storforsen/negy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Storforsen/posz.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Storforsen/negz.jpg",
		};
		break;
	case 6:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/Tenerife/posx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Tenerife/negx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Tenerife/posy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Tenerife/negy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Tenerife/posz.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Tenerife/negz.jpg",
		};
		break;
	case 7:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/Yokohama/posx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Yokohama/negx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Yokohama/posy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Yokohama/negy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Yokohama/posz.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/Yokohama/negz.jpg",
		};
		break;
	case 8:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/GoldenGateBridge/posx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/GoldenGateBridge/negx.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/GoldenGateBridge/posy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/GoldenGateBridge/negy.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/GoldenGateBridge/posz.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/GoldenGateBridge/negz.jpg",
		};
		break;
	default:
		faces = {
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/right.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/left.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/top.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/bottom.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/front.jpg",
			RELATIVE_PATH_ROOT "/resources/skyboxes/skybox/back.jpg",
		};
		break;
	}

	if(faces.empty())
		return false;

	m_currentBackground = _id;
	return loadCubemap(faces);
}