#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "../util/math3d.h"
#include "../util/mappedValues.h"
#include <map>

class MaterialData : public ReferenceCounter, public MappedValues {
public:
private:
};

class Material {
public:
	Material(const std::string& materialName = "");
	Material(const Material& other);
	virtual ~Material();

	Material(const std::string& materialName, const Texture& diffuse, float specularIntensity, float specularPower, const Texture& normalMap = Texture("default_normal.jpg"), const Texture& dispMap = Texture("default_disp.png"), float dispMapScale = 0.0f, float dispMapOffset = 0.0f);

	inline void setVector3f(const std::string& name, const Vector3f& value) { m_materialData->setVector3f(name, value); }
	inline void setFloat(const std::string& name, float value) { m_materialData->setFloat(name, value); }
	inline void setTexture(const std::string& name, const Texture& value) { m_materialData->setTexture(name, value); }

	inline const Vector3f& getVector3f(const std::string& name) const { return m_materialData->getVector3f(name); }
	inline float getFloat(const std::string& name) const { return m_materialData->getFloat(name); }
	inline const Texture& getTexture(const std::string& name) const { return m_materialData->getTexture(name); }
protected:
private:
	static std::map<std::string, MaterialData*> s_resourceMap;
	MaterialData* m_materialData;
	std::string   m_materialName;

	void operator=(const Material& other) {}
};

#endif