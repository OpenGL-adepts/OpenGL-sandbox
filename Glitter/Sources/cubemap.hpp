#pragma once

#include <vector>


class CubeMap {
public:
	CubeMap();
	unsigned int loadCubemap(std::vector<std::string> faces);
};
