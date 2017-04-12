#include "model.h"
#include "../util/profiling.h"
#include <GL/glew.h>
#include <iostream>

#include <vector>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::map<std::string, ModelData*> Model::s_resourceMap;

bool IndexedModel::isValid() const {
	return m_positions.size() == m_texCoords.size() && m_texCoords.size() == m_normals.size() && m_normals.size() == m_tangents.size();
}

void IndexedModel::addVertex(const Vector3f& vert) {
	m_positions.push_back(vert);
}

void IndexedModel::addTexCoord(const Vector2f& texCoord) {
	m_texCoords.push_back(texCoord);
}

void IndexedModel::addNormal(const Vector3f& normal) {
	m_normals.push_back(normal);
}

void IndexedModel::addTangent(const Vector3f& tangent) {
	int result = m_tangents.size();
	m_tangents.push_back(tangent);
}

IndexedModel IndexedModel::finalize() {
	if (isValid())
		return *this;

	if (m_texCoords.size() == 0)
		for (unsigned int i = m_texCoords.size(); i < m_positions.size(); i++)
			m_texCoords.push_back(Vector2f(0.0f, 0.0f));

	if (m_normals.size() == 0)
		calcNormals();

	if (m_tangents.size() == 0)
		calcTangents();

	return *this;
}

void IndexedModel::addFace(unsigned int vertIndex0, unsigned int vertIndex1, unsigned int vertIndex2) {
	m_indices.push_back(vertIndex0);
	m_indices.push_back(vertIndex1);
	m_indices.push_back(vertIndex2);
}

void IndexedModel::calcNormals() {
	m_normals.clear();
	m_normals.reserve(m_positions.size());

	for (unsigned int i = 0; i < m_positions.size(); i++)
		m_normals.push_back(Vector3f(0, 0, 0));

	for (unsigned int i = 0; i < m_indices.size(); i += 3) {
		int i0 = m_indices[i];
		int i1 = m_indices[i + 1];
		int i2 = m_indices[i + 2];

		Vector3f v1 = m_positions[i1] - m_positions[i0];
		Vector3f v2 = m_positions[i2] - m_positions[i0];

		Vector3f normal = v1.cross(v2).normalized();

		m_normals[i0] = m_normals[i0] + normal;
		m_normals[i1] = m_normals[i1] + normal;
		m_normals[i2] = m_normals[i2] + normal;
	}

	for (unsigned int i = 0; i < m_normals.size(); i++)
		m_normals[i] = m_normals[i].normalized();
}

void IndexedModel::calcTangents() {
	m_tangents.clear();
	m_tangents.reserve(m_positions.size());

	for (unsigned int i = 0; i < m_positions.size(); i++)
		m_tangents.push_back(Vector3f(0, 0, 0));

	for (unsigned int i = 0; i < m_indices.size(); i += 3) {
		int i0 = m_indices[i];
		int i1 = m_indices[i + 1];
		int i2 = m_indices[i + 2];

		Vector3f edge1 = m_positions[i1] - m_positions[i0];
		Vector3f edge2 = m_positions[i2] - m_positions[i0];

		float deltaU1 = m_texCoords[i1].getX() - m_texCoords[i0].getX();
		float deltaU2 = m_texCoords[i2].getX() - m_texCoords[i0].getX();
		float deltaV1 = m_texCoords[i1].getY() - m_texCoords[i0].getY();
		float deltaV2 = m_texCoords[i2].getY() - m_texCoords[i0].getY();

		float dividend = (deltaU1 * deltaV2 - deltaU2 * deltaV1);
		float f = dividend == 0.0f ? 0.0f : 1.0f / dividend;

		Vector3f tangent = Vector3f(0, 0, 0);

		tangent.setX(f * (deltaV2 * edge1.getX() - deltaV1 * edge2.getX()));
		tangent.setY(f * (deltaV2 * edge1.getY() - deltaV1 * edge2.getY()));
		tangent.setZ(f * (deltaV2 * edge1.getZ() - deltaV1 * edge2.getZ()));

		m_tangents[i0] += tangent;
		m_tangents[i1] += tangent;
		m_tangents[i2] += tangent;
	}

	for (unsigned int i = 0; i < m_tangents.size(); i++)
		m_tangents[i] = m_tangents[i].normalized();
}

ModelData::ModelData(const IndexedModel& model) : ReferenceCounter(), m_renderCount(model.getIndices().size()) {
	if (!model.isValid()) {
		std::cout << "Error: Invalid mesh! Must have same number of positions, texCoords, normals, and tangents! "
			<< "(Maybe you forgot to Finalize() your IndexedModel?)" << std::endl;
		assert(0 != 0);
	}

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.getPositions().size() * sizeof(model.getPositions()[0]), &model.getPositions()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.getTexCoords().size() * sizeof(model.getTexCoords()[0]), &model.getTexCoords()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.getNormals().size() * sizeof(model.getNormals()[0]), &model.getNormals()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.getTangents().size() * sizeof(model.getTangents()[0]), &model.getTangents()[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getIndices().size() * sizeof(model.getIndices()[0]), &model.getIndices()[0], GL_STATIC_DRAW);
}

ModelData::~ModelData() { 
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void ModelData::render() const {
	glBindVertexArray(m_vertexArrayObject);
	
	#if PROFILING_DISABLE_MODEL_RENDERING == 0
		glDrawElements(GL_TRIANGLES, m_renderCount, GL_UNSIGNED_INT, 0);
	#endif
}

Model::Model(const std::string& fileName) : m_fileName(fileName), m_modelData(0) {
	std::map<std::string, ModelData*>::const_iterator it = s_resourceMap.find(fileName);
	if (it != s_resourceMap.end()) {
		m_modelData = it->second;
		m_modelData->addReference();
	} else {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(("./res/models/" + fileName).c_str(),
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace);

		if (!scene) {
			std::cout << "Model load failed!: " << fileName << std::endl;
			assert(0 == 0);
		}

		const aiMesh* model = scene->mMeshes[0];

		std::vector<Vector3f> positions;
		std::vector<Vector2f> texCoords;
		std::vector<Vector3f> normals;
		std::vector<Vector3f> tangents;
		std::vector<unsigned int> indices;

		const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
		for (unsigned int i = 0; i < model->mNumVertices; i++) {
			const aiVector3D pos = model->mVertices[i];
			const aiVector3D normal = model->mNormals[i];
			const aiVector3D texCoord = model->HasTextureCoords(0) ? model->mTextureCoords[0][i] : aiZeroVector;
			const aiVector3D tangent = model->mTangents[i];

			positions.push_back(Vector3f(pos.x, pos.y, pos.z));
			texCoords.push_back(Vector2f(texCoord.x, texCoord.y));
			normals.push_back(Vector3f(normal.x, normal.y, normal.z));
			tangents.push_back(Vector3f(tangent.x, tangent.y, tangent.z));
		}

		for (unsigned int i = 0; i < model->mNumFaces; i++) {
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		m_modelData = new ModelData(IndexedModel(indices, positions, texCoords, normals, tangents));
		s_resourceMap.insert(std::pair<std::string, ModelData*>(fileName, m_modelData));
	}
}

Model::Model(const std::string& modelName, const IndexedModel& model) : m_fileName(modelName) {
	std::map<std::string, ModelData*>::const_iterator it = s_resourceMap.find(modelName);
	if (it != s_resourceMap.end()) {
		std::cout << "Error adding model " << modelName << ": there is already an existing model with this name!" << std::endl;
		assert(0 != 0);
	} else {
		m_modelData = new ModelData(model);
		s_resourceMap.insert(std::pair<std::string, ModelData*>(modelName, m_modelData));
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

void Model::render() const {
	m_modelData->render();
}