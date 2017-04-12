#include "common.glh"

varying vec2 texCoord0;

#if defined(VS_BUILD)
attribute vec3 position;
attribute vec2 texCoord;

uniform mat4 T_MVP;

void main() {
	texCoord0 = texCoord;
	gl_Position = T_MVP * vec4(position, 1.0);
}

#elif defined(FS_BUILD)
uniform sampler2D R_filterTexture;
uniform vec3 R_inverseFilterTextureSize;
uniform float R_fxaaSpanMax;
uniform float R_fxaaReduceMin;
uniform float R_fxaaReduceMul;

declareFragOutput(0, vec4);
void main() {
	vec2 texCoordOffset = R_inverseFilterTextureSize.xy;

	vec3 luma = vec3(0.299, 0.587, 0.114);
	float lumaTL = dot(luma, texture2D(R_filterTexture, texCoord0.xy + (vec2(-1.0, -1.0) * texCoordOffset)).xyz);
	float lumaTR = dot(luma, texture2D(R_filterTexture, texCoord0.xy + (vec2(1.0, -1.0) * texCoordOffset)).xyz);
	float lumaBL = dot(luma, texture2D(R_filterTexture, texCoord0.xy + (vec2(-1.0, 1.0) * texCoordOffset)).xyz);
	float lumaBR = dot(luma, texture2D(R_filterTexture, texCoord0.xy + (vec2(1.0, 1.0) * texCoordOffset)).xyz);
	float lumaM = dot(luma, texture2D(R_filterTexture, texCoord0.xy).xyz);

	vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));

	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * (R_fxaaReduceMul * 0.25), R_fxaaReduceMin);
	float inverseDirAdjustment = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = min(vec2(R_fxaaSpanMax, R_fxaaSpanMax), max(vec2(-R_fxaaSpanMax, -R_fxaaSpanMax), dir * inverseDirAdjustment));

	dir.x = dir.x * step(1.0, abs(dir.x));
	dir.y = dir.y * step(1.0, abs(dir.y));

	dir = dir * texCoordOffset;

	vec3 result1 = (1.0 / 2.0) * (texture2D(R_filterTexture, texCoord0.xy + (dir * vec2(1.0 / 3.0 - 0.5))).xyz + texture2D(R_filterTexture, texCoord0.xy + (dir * vec2(2.0 / 3.0 - 0.5))).xyz);

	vec3 result2 = result1 * (1.0 / 2.0) + (1.0 / 4.0) * (texture2D(R_filterTexture, texCoord0.xy + (dir * vec2(0.0 / 3.0 - 0.5))).xyz + texture2D(R_filterTexture, texCoord0.xy + (dir * vec2(3.0 / 3.0 - 0.5))).xyz);

	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
	float lumaResult2 = dot(luma, result2);

	if (lumaResult2 < lumaMin || lumaResult2 > lumaMax)
		setFragOutput(0, vec4(result1, 1.0));
	else
		setFragOutput(0, vec4(result2, 1.0)); 
}
#endif