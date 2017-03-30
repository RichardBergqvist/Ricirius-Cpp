#include "sampling.glh"

bool inRange(float val) {
	return val >= 0.0 && val <= 1.0;
}

float calcShadowAmount(sampler2D shadowMap, vec4 initialShadowMapCoords) {
	vec3 shadowMapCoords = (initialShadowMapCoords.xyz / initialShadowMapCoords.w);
	
	if (inRange(shadowMapCoords.z) && inRange(shadowMapCoords.x) && inRange(shadowMapCoords.y)) {
		return sampleVarianceShadowMap(shadowMap, shadowMapCoords.xy, shadowMapCoords.z, R_shadowVarianceMin, R_shadowLightBleedingReduction);
	} else {
		return 1.0;
	}
}

void main() {
	vec3 directionToEye = normalize(C_eyePos - worldPos0);
	vec2 texCoords = calcParallaxTexCoords(dispMap, tbnMatrix, directionToEye, texCoord0, dispMapScale, dispMapBias);
	
	vec3 normal = normalize(tbnMatrix * (255.0/128.0 * texture2D(normalMap, texCoords).xyz - 1));
	
	vec4 lightingAmt = calcLightingEffect(normal, worldPos0) * calcShadowAmount(R_shadowMap, shadowMapCoords0);
    gl_FragColor = texture2D(diffuse, texCoords) * lightingAmt;
}
