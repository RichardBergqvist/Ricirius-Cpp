#ifndef MODEL_H
#define MODEL_H

#include "vertex.h"
#include "../components/referenceCounter.h"
#include <string>
#include <map>

class ModelData : public ReferenceCounter {
public:
	ModelData(int indexSize);
	virtual ~ModelData();
	
	inline unsigned int getVBO() { return m_vbo; }
	inline unsigned int getIBO() { return m_ibo; }
	inline int getSize() { return m_size; }
protected:	
private:
	ModelData(ModelData& other) {}
	void operator=(ModelData& other) {}

	unsigned int m_vbo;
	unsigned int m_ibo;
	int m_size;
};

class Model {
public:
	Model(const std::string& fileName = "cube.obj");
	Model(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals);
	Model(const Model& model);
	virtual ~Model();

	void render() const;
protected:
private:
	static std::map<std::string, ModelData*> s_resourceMap;
	
	void calcNormals(Vertex* vertices, int vertSize, int* indices, int indexSize) const;
	void initModel(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals = true);

	std::string m_fileName;
	ModelData* m_modelData;

	void operator=(Model& model) {}
};

#endif
