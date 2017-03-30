#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "../util/math3d.h"
#include "../core/mappedValues.h"
#include <map>

class Material : public MappedValues {
public:
	Material() {}
	Material(Texture* diffuse, float specularIntensity, float specularPower, Texture* normalMap = new Texture("default_normal.jpg"), Texture* dispMap = new Texture("default_disp.png"), float dispMapScale = 0.0F, float dispMapOffset = 0.0F) {
		setTexture("diffuse", diffuse);
		setFloat("specularIntensity", specularIntensity);
		setFloat("specularPower", specularPower);
		setTexture("normalMap", normalMap);
		setTexture("dispMap", dispMap);
		
		float baseBias = dispMapScale / 2.0F;
		setFloat("dispMapScale", dispMapScale);
		setFloat("dispMapBias", -baseBias + baseBias * dispMapOffset);
	}
protected:
private:
	Material(const Material& other) {}
	void operator=(const Material& other) {}
};

#endif
