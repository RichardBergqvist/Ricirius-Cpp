#version 120
#include "lighting.fsh"

uniform DirectionalLight R_directionalLight;

vec4 calcLightingEffect(vec3 normal, vec3 worldPos)
{
	return calcDirectionalLight(R_directionalLight, normal, worldPos);
}

#include "lightingMain.fsh"
