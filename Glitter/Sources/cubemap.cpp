#include "cubemap.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

CubeMap::CubeMap() {
	currentBackground = -1;
	textureId = -1;
}

CubeMap::~CubeMap() {
}


unsigned int CubeMap::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

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
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int CubeMap::getTextureByCurrentBackground() {
	std::vector<std::string> faces;
	switch (this->currentBackground)
	{
	case 0:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/right.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/left.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/top.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/bottom.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/front.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/back.jpg",
		};
		break;
	case 1:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/darkskies/darkskies_ft.tga",
			PROJECT_SOURCE_DIR "/resources/skyboxes/darkskies/darkskies_bk.tga",
			PROJECT_SOURCE_DIR "/resources/skyboxes/darkskies/darkskies_up.tga",
			PROJECT_SOURCE_DIR "/resources/skyboxes/darkskies/darkskies_dn.tga",
			PROJECT_SOURCE_DIR "/resources/skyboxes/darkskies/darkskies_rt.tga",
			PROJECT_SOURCE_DIR "/resources/skyboxes/darkskies/darkskies_lf.tga",
		};
		break;
	case 2:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/NissiBeach/posx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/NissiBeach/negx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/NissiBeach/posy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/NissiBeach/negy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/NissiBeach/posz.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/NissiBeach/negz.jpg",
		};
		break;
	case 3:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/SaintPetersSquare/posx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SaintPetersSquare/negx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SaintPetersSquare/posy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SaintPetersSquare/negy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SaintPetersSquare/posz.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SaintPetersSquare/negz.jpg",
		};
		break;
	case 4:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/SantaMariaDeiMiracoli/posx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SantaMariaDeiMiracoli/negx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SantaMariaDeiMiracoli/posy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SantaMariaDeiMiracoli/negy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SantaMariaDeiMiracoli/posz.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/SantaMariaDeiMiracoli/negz.jpg",
		};
		break;
	case 5:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/Storforsen/posx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Storforsen/negx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Storforsen/posy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Storforsen/negy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Storforsen/posz.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Storforsen/negz.jpg",
		};
		break;
	case 6:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/Tenerife/posx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Tenerife/negx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Tenerife/posy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Tenerife/negy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Tenerife/posz.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Tenerife/negz.jpg",
		};
		break;
	case 7:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/Yokohama/posx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Yokohama/negx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Yokohama/posy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Yokohama/negy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Yokohama/posz.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/Yokohama/negz.jpg",
		};
		break;
	case 8:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/GoldenGateBridge/posx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/GoldenGateBridge/negx.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/GoldenGateBridge/posy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/GoldenGateBridge/negy.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/GoldenGateBridge/posz.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/GoldenGateBridge/negz.jpg",
		};
		break;
	default:
		faces = {
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/right.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/left.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/top.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/bottom.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/front.jpg",
			PROJECT_SOURCE_DIR "/resources/skyboxes/skybox/back.jpg",
		};
		break;
	}
	return this->loadCubemap(faces);
}