#pragma once
#include "texture.hpp"
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <vector>


// Vertex Format
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv; // texture coords
	glm::vec3 tangent; // for normal mapping
	glm::vec3 bitangent; // for normal mapping
};


class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(const std::string& filename);
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<std::pair<Texture, std::string>> textures);

	bool loadFromFile(const std::string& _filename);
	void draw(GLuint shader);

	glm::vec3 getCenter() const;
	float getMaxAxisSize() const;
	glm::mat4 centerAtAndNormalize(const glm::vec3& _pos) const;

private:
	// Disable Copying and Assignment
	Mesh(Mesh const &) = delete;
	Mesh & operator=(Mesh const &) = delete;

	// Private Member Functions
	void parse(const std::string& path, const aiNode* node, const aiScene* scene);
	void parse(const std::string& path, const aiMesh* mesh, const aiScene* scene);
	void process(const std::string& path, aiMaterial* material, aiTextureType type, std::vector<std::pair<Texture, std::string>>& outTextures);
	void bindTexturePlaceholder();

	// Private Member Containers
	std::vector<std::unique_ptr<Mesh>> mSubMeshes;
	std::vector<GLuint> mIndices;
	std::vector<Vertex> mVertices;
	std::vector<std::pair<Texture, std::string>> mTextures;

	// Private Member Variables
	GLuint mVertexArray;
	GLuint mVertexBuffer;
	GLuint mElementBuffer;

	glm::vec3 mMaxCoord;
	glm::vec3 mMinCoord;

	std::unique_ptr<Texture> m_solidColor;
};

