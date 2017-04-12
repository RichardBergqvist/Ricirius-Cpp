#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <vector>
#include <string>

#include "../util/referenceCounter.h"
#include "../util/math3d.h"
#include "material.h"
#include "../components/transformer.h"
#include "../components/camera.h"

class GraphicsEngine;
class DirectionalLight;
class PointLight;
class SpotLight;

class TypedData {
public:
	TypedData(const std::string& name, const std::string& type) : m_name(name), m_type(type) {}

	inline const std::string& getName() const { return m_name; }
	inline const std::string& getType() const { return m_type; }
private:
	std::string m_name;
	std::string m_type;
};

class UniformStruct {
public:
	UniformStruct(const std::string& name, const std::vector<TypedData>& memberNames) : m_name(name), m_memberNames(memberNames) {}

	inline const std::string& getName() const { return m_name; }
	inline const std::vector<TypedData>& getMemberNames() const { return m_memberNames; }
private:
	std::string m_name;
	std::vector<TypedData> m_memberNames;
};

class ShaderData : public ReferenceCounter {
public:
	ShaderData(const std::string& fileName, bool useNewShaderSystem);
	virtual ~ShaderData();
	
	inline int getProgram() const { return m_program; }
	inline const std::vector<int>& getShaders() const { return m_shaders; }
	inline const std::vector<std::string>& getUniformNames() const { return m_uniformNames; }
	inline const std::vector<std::string>& getUniformTypes() const { return m_uniformTypes; }
	inline const std::map<std::string, unsigned int>& getUniformMap() const { return m_uniformMap; }
private:
	void addVertexShader(const std::string& text);
	void addGeometryShader(const std::string& text);
	void addFragmentShader(const std::string& text);
	void addProgram(const std::string& text, int type);

	void addAllAttributes(const std::string& vertexShaderText, const std::string& attributeKeyword);
	void addShaderUniforms(const std::string& shaderText);
	void addUniform(const std::string& uniformName, const std::string& uniformType, const std::vector<UniformStruct>& structs);
	void compileShader() const;
	void convertVertexShaderToGLSL150(std::string* shaderText);
	void convertFragmentShaderToGLSL150(std::string* shaderText);

	static int s_supportedOpenGLLevel;
	static std::string s_glslVersion;
	int m_program;
	std::vector<int>                    m_shaders;
	std::vector<std::string>            m_uniformNames;
	std::vector<std::string>            m_uniformTypes;
	std::map<std::string, unsigned int> m_uniformMap;
};

class Shader {
public:
	Shader(const std::string& fileName = "basicShader", bool useNewShaderSystem = false);
	Shader(const Shader& shader);
	virtual ~Shader();

	void bind() const;
	virtual void updateUniforms(const Transformer& transform, const Material& material, const GraphicsEngine& graphicsEngine, const Camera& camera) const;

	void setUniformi(const std::string& uniformName, int value) const;
	void setUniformf(const std::string& uniformName, float value) const;
	void setUniformMatrix4f(const std::string& uniformName, const Matrix4f& value) const;
	void setUniformVector3f(const std::string& uniformName, const Vector3f& value) const;
protected:
private:
	static std::map<std::string, ShaderData*> s_resourceMap;

	ShaderData* m_shaderData;
	std::string m_fileName;

	void setUniformDirectionalLight(const std::string& uniformName, const DirectionalLight& value) const;
	void setUniformPointLight(const std::string& uniformName, const PointLight& value) const;
	void setUniformSpotLight(const std::string& uniformName, const SpotLight& value) const;

	void operator=(const Shader& other) {}
};

#endif
