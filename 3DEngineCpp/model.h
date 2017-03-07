#ifndef MODEL_H
#define MODEL_H

#include "vertex.h"
#include "referenceCounter.h"
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
	Model(const std::string& fileName);
	Model(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals);
	virtual ~Model();

	void render() const;
protected:
private:
	static std::map<std::string, ModelData*> s_resourceMap;
	
	Model(Model& model) {}
	void operator=(Model& model) {}
	
	void calcNormals(Vertex* vertices, int vertSize, int* indices, int indexSize);
	void initModel(Vertex* vertices, int vertSize, int* indices, int indexSize, bool calcNormals = true);

	std::string m_fileName;
	ModelData* m_modelData;
};

#endif
