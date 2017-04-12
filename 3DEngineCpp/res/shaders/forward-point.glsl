#include "common.glh"
#include "forwardLighting.glh"

#if defined(VS_BUILD)
#include "forwardLighting.vsh"
#elif defined(FS_BUILD)

#include "lighting.glh"

uniform vec3 C_eyePos;
uniform float specularIntensity;
uniform float specularPower;
uniform PointLight R_pointLight;

vec4 calcLightingEffect(vec3 normal, vec3 worldPos) {
	return calcPointLight(R_pointLight, normal, worldPos, specularIntensity, specularPower, C_eyePos);
}

#include "lightingMain.fsh"
#endif