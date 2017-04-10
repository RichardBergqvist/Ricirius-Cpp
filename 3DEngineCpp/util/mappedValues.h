#ifndef MAPPEDVALUES_H_INCLUDED
#define MAPPEDVALUES_H_INCLUDED

#include <map>
#include <string>
#include "../graphics/texture.h"
#include "math3d.h"

class MappedValues {
public:
	MappedValues() : m_defaultTexture(Texture("defaultTexture.png")), m_defaultVector3f(Vector3f(0, 0, 0)) {}

	inline void setVector3f(const std::string& name, const Vector3f& value) { m_vector3fMap[name] = value; }
	inline void setFloat(const std::string& name, float value) { m_floatMap[name] = value; }
	inline void setTexture(const std::string& name, const Texture& value) { m_textureMap[name] = value; }
	
	const Vector3f& getVector3f(const std::string& name) const;
	float getFloat(const std::string& name) const;
	const Texture& getTexture(const std::string& name) const;
protected:
private:
	std::map<std::string, Vector3f> m_vector3fMap;
	std::map<std::string, float> m_floatMap;
	std::map<std::string, Texture> m_textureMap;

	Texture m_defaultTexture;
	Vector3f m_defaultVector3f;
};

#endif // MAPPEDVALUES_H_INCLUDED
