#include "model.h"
#include <GL/glew.h>
#include <iostream>

#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::map<std::string, ModelData*> Model::s_resourceMap;

ModelData::ModelData(int indexSize) : ReferenceCounter(), m_size(indexSize) {
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
}

ModelData::~ModelData() { 
	if(m_vbo) glDeleteBuffers(1, &m_vbo);
	if(m_ibo) glDeleteBuffers(1, &m_ibo);
}


Model::Model(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals) : m_fileName("") {
	initModel(vertices, vertSize, indices, indexSize, calcNormals);
}

Model::Model(const std::string& fileName) : m_fileName(fileName), m_modelData(0) {
	std::map<std::string, ModelData*>::const_iterator it = s_resourceMap.find(fileName);
	if(it != s_resourceMap.end()) {
		m_modelData = it->second;
		m_modelData->addReference();
	} else {
		Assimp::Importer importer;
		
		const aiScene* scene = importer.ReadFile(("./res/models/" + fileName).c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		if(!scene) {
			std::cout << "Model load failed!: " << fileName << std::endl;
			assert(0 == 0);
		}
		
		const aiMesh* model = scene->mMeshes[0];
		
		std::vector<Vertex> vertices;
		std::vector<int> indices;

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
		for(unsigned int i = 0; i < model->mNumVertices; i++) {
			const aiVector3D* pPos = &(model->mVertices[i]);
			const aiVector3D* pNormal = &(model->mNormals[i]);
			const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;
			const aiVector3D* pTangent = &(model->mTangents[i]);

			Vertex vert(Vector3f(pPos->x, pPos->y, pPos->z), Vector2f(pTexCoord->x, pTexCoord->y), Vector3f(pNormal->x, pNormal->y, pNormal->z), Vector3f(pTangent->x, pTangent->y, pTangent->z));
			
			vertices.push_back(vert);
		}

		for(unsigned int i = 0; i < model->mNumFaces; i++) {
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
		
		initModel(&vertices[0], vertices.size(), (int*)&indices[0], indices.size(), false);
		
		s_resourceMap.insert(std::pair<std::string, ModelData*>(fileName, m_modelData));
	}
}

Model::Model(const Model& model) : m_fileName(model.m_fileName), m_modelData(model.m_modelData) {
	m_modelData->addReference();
}

Model::~Model() {
	if(m_modelData && m_modelData->removeReference()) {
		if(m_fileName.length() > 0)
			s_resourceMap.erase(m_fileName);
			
		delete m_modelData;
	}
}

void Model::initModel(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals) {
	m_modelData = new ModelData(indexSize);

	if(calcNormals)
		this->calcNormals(vertices, vertSize, indices, indexSize);

	glBindBuffer(GL_ARRAY_BUFFER, m_modelData->getVBO());
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_modelData->getIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(int), indices, GL_STATIC_DRAW);
}

void Model::render() const {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, m_modelData->getVBO());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3f));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f) + sizeof(Vector2f)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3f) + sizeof(Vector2f) + sizeof(Vector3f)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_modelData->getIBO());
	glDrawElements(GL_TRIANGLES, m_modelData->getSize(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void Model::calcNormals(Vertex* vertices, int vertSize, int* indices, int indexSize) const {
	for (int i = 0; i < indexSize; i += 3) {
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		Vector3f v1 = vertices[i1].getPos() - vertices[i0].getPos();
		Vector3f v2 = vertices[i2].getPos() - vertices[i0].getPos();

		Vector3f normal = v1.cross(v2).normalized();

		vertices[i0].setNormal(vertices[i0].getNormal() + normal);
		vertices[i1].setNormal(vertices[i1].getNormal() + normal);
		vertices[i2].setNormal(vertices[i2].getNormal() + normal);
	}

	for (int i = 0; i < vertSize; i++)
		vertices[i].setNormal(vertices[i].getNormal().normalized());
}
