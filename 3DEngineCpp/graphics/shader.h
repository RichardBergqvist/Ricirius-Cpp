#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <vector>
#include <string>

#include "../components/referenceCounter.h"
#include "../util/math3d.h"
#include "material.h"
#include "graphicsEngine.h"
#include "lighting.h"
#include "../core/transformer.h"

struct TypedData {
	std::string name;
	std::string type;
};

struct UniformStruct {
	std::string name;
	std::vector<TypedData> memberNames;
};

class ShaderData : public ReferenceCounter {
public:
	ShaderData(const std::string& fileName);
	virtual ~ShaderData();
	
	inline int getProgram() { return m_program; }
	inline std::vector<int>& getShaders() { return m_shaders; }
	inline std::vector<std::string>& getUniformNames() { return m_uniformNames; }
	inline std::vector<std::string>& getUniformTypes() { return m_uniformTypes; }
	inline std::map<std::string, unsigned int>& getUniformMap() { return m_uniformMap; }
private:
	void addVertexShader(const std::string& text);
	void addGeometryShader(const std::string& text);
	void addFragmentShader(const std::string& text);
	void addProgram(const std::string& text, int type);
	
	void addAllAttributes(const std::string& vertexShaderText);
	void addShaderUniforms(const std::string& shaderText);
	void addUniform(const std::string& uniformName, const std::string& uniformType, const std::vector<UniformStruct>& structs);
	void compileShader();

	int m_program;
	std::vector<int> m_shaders;
	std::vector<std::string> m_uniformNames;
	std::vector<std::string> m_uniformTypes;
	std::map<std::string, unsigned int> m_uniformMap;
};

class Shader {
public:
	Shader(const std::string& fileName);
	virtual ~Shader();

	void bind();
	virtual void updateUniforms(const Transformer& transform, const Material& material, GraphicsEngine* graphicsEngine);

	void setUniformi(const std::string& uniformName, int value);
	void setUniformf(const std::string& uniformName, float value);
	void setUniformMatrix4f(const std::string& uniformName, const Matrix4f& value);
	void setUniformVector3f(const std::string& uniformName, const Vector3f& value);
protected:
private:
	static std::map<std::string, ShaderData*> s_resourceMap;

	ShaderData* m_shaderData;
	std::string m_fileName;
	
	void setUniformDirectionalLight(const std::string& uniformName, const DirectionalLight& value);
	void setUniformPointLight(const std::string& uniformName, const PointLight& value);
	void setUniformSpotLight(const std::string& uniformName, const SpotLight& value);
};

#endif
