#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "math3d.h"
#include "mappedValues.h"
#include <map>

class Material : public MappedValues {
public:
	Material() {}
	
	virtual ~Material() {
		std::map<std::string, Texture*>::iterator it;
		for(it = m_textureMap.begin(); it != m_textureMap.end(); it++)
			if(it->second) delete it->second;
	}
	
	Material(Texture* diffuse, float specularIntensity, float specularPower, Texture* normalMap = new Texture("default_normal.jpg"), Texture* dispMap = new Texture("default_disp.png"), float dispMapScale = 0.0F, float dispMapOffset = 0.0F) {
		addTexture("diffuse", diffuse);
		addFloat("specularIntensity", specularIntensity);
		addFloat("specularPower", specularPower);
		addTexture("normalMap", normalMap);
		addTexture("dispMap", dispMap);
		
		float baseBias = dispMapScale / 2.0F;
		addFloat("dispMapScale", dispMapScale);
		addFloat("dispMapBias", -baseBias + baseBias * dispMapOffset);
	}
	
	inline void addTexture(const std::string& name, Texture* value) { m_textureMap.insert(std::pair<std::string, Texture*>(name, value)); }
	
	inline Texture* getTexture(const std::string& name) const {
		std::map<std::string, Texture*>::const_iterator it = m_textureMap.find(name);
		if(it != m_textureMap.end())
			return it->second;
			
		return 0;
	}
protected:
private:
	Material(const Material& other) {}
	void operator=(const Material& other) {}

	std::map<std::string, Texture*> m_textureMap;
};

#endif
