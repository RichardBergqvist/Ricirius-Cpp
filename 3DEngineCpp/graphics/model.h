#ifndef MODEL_H
#define MODEL_H

#include "../util/math3d.h"
#include "../util/referenceCounter.h"
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>

class IndexedModel {
public:
	IndexedModel() {}
	IndexedModel(const std::vector<unsigned int> indices, const std::vector<Vector3f>& positions, const std::vector<Vector2f>& texCoords, const std::vector<Vector3f>& normals = std::vector<Vector3f>(), const std::vector<Vector3f>& tangents = std::vector<Vector3f>()) : m_indices(indices), m_positions(positions), m_texCoords(texCoords), m_normals(normals), m_tangents(tangents) {}

	bool isValid() const;
	void calcNormals();
	void calcTangents();
	
	IndexedModel finalize();
	
	void addVertex(const Vector3f& vert);
	inline void AddVertex(float x, float y, float z) { addVertex(Vector3f(x, y, z)); }
	
	void addTexCoord(const Vector2f& texCoord);
	inline void AddTexCoord(float x, float y) { addTexCoord(Vector2f(x, y)); }
	
	void addNormal(const Vector3f& normal);
	inline void AddNormal(float x, float y, float z) { addNormal(Vector3f(x, y, z)); }
	
	void addTangent(const Vector3f& tangent);
	inline void AddTangent(float x, float y, float z) { addTangent(Vector3f(x, y, z)); }
	
	void addFace(unsigned int vertIndex0, unsigned int vertIndex1, unsigned int vertIndex2);

	inline const std::vector<unsigned int>& getIndices() const { return m_indices; }
	inline const std::vector<Vector3f>& getPositions()   const { return m_positions; }
	inline const std::vector<Vector2f>& getTexCoords()   const { return m_texCoords; }
	inline const std::vector<Vector3f>& getNormals()     const { return m_normals; }
	inline const std::vector<Vector3f>& getTangents()    const { return m_tangents; }
private:
	std::vector<unsigned int> m_indices;
	std::vector<Vector3f> m_positions;
	std::vector<Vector2f> m_texCoords;
	std::vector<Vector3f> m_normals;
	std::vector<Vector3f> m_tangents;
};

class ModelData : public ReferenceCounter {
public:
	ModelData(const IndexedModel& model);
	virtual ~ModelData();
	
	void render() const;
protected:	
private:
	ModelData(ModelData& other) {}
	void operator=(ModelData& other) {}

	enum {
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		TANGENT_VB,

		INDEX_VB,

		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	int m_renderCount;
};

class Model {
public:
	Model(const std::string& fileName = "cube.obj");
	Model(const std::string& modelName, const IndexedModel& model);
	Model(const Model& model);
	virtual ~Model();

	void render() const;
protected:
private:
	static std::map<std::string, ModelData*> s_resourceMap;

	std::string m_fileName;
	ModelData* m_modelData;

	void operator=(Model& model) {}
};

#endif
