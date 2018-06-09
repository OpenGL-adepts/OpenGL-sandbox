#include "mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <limits>


Mesh::Mesh()
{
	glGenVertexArrays(1, & mVertexArray);
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, & mVertexArray);
}


Mesh::Mesh(const std::string& filename)
	: Mesh()
{
	// Load a Model from File
	Assimp::Importer loader;
	const aiScene* scene = loader.ReadFile(
		filename,
		aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph | aiProcess_FlipUVs);

	// Walk the Tree of Scene Nodes
	auto index = filename.find_last_of("/");

	constexpr float posInf = std::numeric_limits<float>::infinity();
	mMinCoord = glm::vec3(posInf);
	mMaxCoord = glm::vec3(-posInf);

	if (!scene)
		fprintf(stderr, "%s\n", loader.GetErrorString());
	else
		parse(filename.substr(0, index), scene->mRootNode, scene);
}


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<std::pair<Texture, std::string>> textures)
	: mIndices(std::move(indices))
	, mVertices(std::move(vertices))
	, mTextures(std::move(textures))
{
	// Bind a Vertex Array Object
	glGenVertexArrays(1, & mVertexArray);
	glBindVertexArray(mVertexArray);

	// Copy Vertex Buffer Data
	glGenBuffers(1, & mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices.front(), GL_STATIC_DRAW);

	// Copy Index Buffer Data
	glGenBuffers(1, & mElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices.front(), GL_STATIC_DRAW);

	// Set Shader Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, uv));
	glEnableVertexAttribArray(0); // Vertex Positions
	glEnableVertexAttribArray(1); // Vertex Normals
	glEnableVertexAttribArray(2); // Vertex UVs

	// Cleanup Buffers
	glBindVertexArray(0);
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mElementBuffer);
}


void Mesh::draw(GLuint shader)
{
	unsigned int unit = 0, diffuse = 0, specular = 0;

	for (auto& i : mSubMeshes)
		i->draw(shader);

	for (auto &i : mTextures)
	{
		// Set Correct Uniform Names Using Texture Type (Omit ID for 0th Texture)
		std::string uniform = i.second;

			 if (i.second == "diffuse")	 uniform += (diffuse++	> 0) ? std::to_string(diffuse)	: "";
		else if (i.second == "specular") uniform += (specular++ > 0) ? std::to_string(specular) : "";

		// Bind Correct Textures and Vertex Array Before Drawing
		glActiveTexture(GL_TEXTURE0 + unit);
		i.first.bind();
		glUniform1f(glGetUniformLocation(shader, uniform.c_str()), ++unit);
	}
	
	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}


glm::vec3 Mesh::getCenter() const
{
	return mMinCoord + 0.5f * (mMaxCoord - mMinCoord);
}


float Mesh::getMaxAxisSize() const
{
	const auto size = mMaxCoord - mMinCoord;
	return std::max({size.x, size.y, size.z});
}


glm::mat4 Mesh::centerAtAndNormalize(const glm::vec3& _pos) const
{
	glm::mat4 model(1.f);
	model = glm::translate(model, _pos);
	model = glm::scale(model, glm::vec3(1.f / getMaxAxisSize()));
	model = glm::translate(model, -getCenter());
	return model;
}


void Mesh::parse(const std::string& path, const aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
		parse(path, scene->mMeshes[node->mMeshes[i]], scene);

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		parse(path, node->mChildren[i], scene);
}


void Mesh::parse(const std::string& path, const aiMesh* mesh, const aiScene* scene)
{
	// Create Vertex Data from Mesh Node
	std::vector<Vertex> vertices; Vertex vertex;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		if (mesh->mTextureCoords[0])
			vertex.uv		= glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal	= glm::vec3(mesh->mNormals[i].x,  mesh->mNormals[i].y,	mesh->mNormals[i].z);
		vertices.push_back(vertex);

		mMaxCoord.x = std::max(mMaxCoord.x, vertex.position.x);
		mMaxCoord.y = std::max(mMaxCoord.y, vertex.position.y);
		mMaxCoord.z = std::max(mMaxCoord.z, vertex.position.z);
		
		mMinCoord.x = std::min(mMinCoord.x, vertex.position.x);
		mMinCoord.y = std::min(mMinCoord.y, vertex.position.y);
		mMinCoord.z = std::min(mMinCoord.z, vertex.position.z);
	}

	// Create Mesh Indices for Indexed Drawing
	std::vector<GLuint> indices;

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		indices.push_back(mesh->mFaces[i].mIndices[j]);

	// Load Mesh Textures into VRAM
	std::vector<std::pair<Texture, std::string>> textures;
	process(path, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, textures);
	process(path, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, textures);
	mSubMeshes.push_back(std::make_unique<Mesh>(std::move(vertices), std::move(indices), std::move(textures)));
}


void Mesh::process(const std::string& path, aiMaterial* material, aiTextureType type, std::vector<std::pair<Texture, std::string>>& outTextures)
{
	for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		// Load the Texture Image from File
		aiString str;
		material->GetTexture(type, i, &str);

		Texture texture(path + "/" + str.C_Str());
		std::string mode;

		switch(type)
		{
		case aiTextureType_DIFFUSE:  mode = "diffuse";	break;
		case aiTextureType_SPECULAR: mode = "specular"; break;
		}

		outTextures.emplace_back(std::move(texture), std::move(mode));
	}
}
