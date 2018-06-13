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
	loadFromFile(filename);
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
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, tangent));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(0); // Vertex Positions
	glEnableVertexAttribArray(1); // Vertex Normals
	glEnableVertexAttribArray(2); // Vertex UVs
	glEnableVertexAttribArray(3); // Vertex Tangents
	glEnableVertexAttribArray(4); // Vertex Bitangents

	// Cleanup Buffers
	glBindVertexArray(0);
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mElementBuffer);
}


bool Mesh::loadFromFile(const std::string& _filename)
{
	Assimp::Importer loader;
	
	if(const aiScene* scene = loader.ReadFile(_filename, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph | aiProcess_FlipUVs | aiProcess_CalcTangentSpace))
	{
		// Walk the Tree of Scene Nodes
		auto index = _filename.find_last_of("/");

		constexpr float posInf = std::numeric_limits<float>::infinity();
		mMinCoord = glm::vec3(posInf);
		mMaxCoord = glm::vec3(-posInf);
		parse(_filename.substr(0, index), scene->mRootNode, scene);
		return true;
	}

	fprintf(stderr, "%s\n", loader.GetErrorString());
	return false;
}


void Mesh::draw(GLuint shader)
{
	unsigned int unit = 0, diffuse = 0, specular = 0, normal = 0;

	for (auto& i : mSubMeshes)
		i->draw(shader);

	for(int i = 0; i < 16; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	for (auto &i : mTextures)
	{
		// Set Correct Uniform Names Using Texture Type (Omit ID for 0th Texture)
		std::string uniform = "texture_" + i.second;

			 if (i.second == "diffuse")	 uniform += (diffuse++	> 0) ? std::to_string(diffuse)	: "";
		else if (i.second == "specular") uniform += (specular++ > 0) ? std::to_string(specular) : "";
		else if (i.second == "normal")	 uniform += (normal++	> 0) ? std::to_string(normal)	: "";

		// Bind Correct Textures and Vertex Array Before Drawing
		glActiveTexture(GL_TEXTURE0 + unit);
		i.first.bind();

		auto uniformloc = glGetUniformLocation(shader, uniform.c_str());

		if(uniformloc != -1)
			glUniform1i(uniformloc, unit++);
	}

	if(diffuse == 0)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		bindTexturePlaceholder();
		glUniform1f(glGetUniformLocation(shader, "texture_diffuse"), unit++);
	}

	if(specular == 0)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		bindTexturePlaceholder();
		glUniform1f(glGetUniformLocation(shader, "texture_specular"), unit++);
	}
	
	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	glActiveTexture(GL_TEXTURE0);
}


glm::vec3 Mesh::getCenter() const
{
	return 0.5f * (mMaxCoord + mMinCoord);
}


float Mesh::getMaxAxisSize() const
{
	const auto size = mMaxCoord - mMinCoord;
	return std::max({size.x, size.y, size.z});
}


glm::mat4 Mesh::centerAndNormalize() const
{
	return glm::scale(glm::mat4(1.f), glm::vec3(1.f / getMaxAxisSize())) * glm::translate(glm::mat4(1.f), -getCenter());
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
		vertex.uv		 = mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.f);
		vertex.position  = glm::vec3(mesh->mVertices[i].x,	 mesh->mVertices[i].y,	 mesh->mVertices[i].z);
		vertex.normal	 = glm::vec3(mesh->mNormals[i].x,	 mesh->mNormals[i].y,	 mesh->mNormals[i].z);

		if(mesh->mTangents && mesh->mBitangents)
		{
			vertex.tangent	 = glm::vec3(mesh->mTangents[i].x,	 mesh->mTangents[i].y,	 mesh->mTangents[i].z);
			vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		else
			vertex.tangent = vertex.bitangent = glm::vec3(0.f);

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
	process(path, scene->mMaterials[mesh->mMaterialIndex], aiTextureType_HEIGHT, textures);
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
		case aiTextureType_HEIGHT:	 mode = "normal";	break;
		}

		outTextures.emplace_back(std::move(texture), std::move(mode));
	}
}


void Mesh::bindTexturePlaceholder()
{
	if(!m_solidColor)
	{
		m_solidColor = std::make_unique<Texture>();
		m_solidColor->createColorPlaceholder();
	}

	m_solidColor->bind();
}
