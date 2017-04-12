#include "common.glh"

#if defined(VS_BUILD)
attribute vec3 position;

void main() {
	gl_Position = vec4(position, 1.0);
}

#elif defined(FS_BUILD)
declareFragOutput(0, vec4);
void main() {
	setFragOutput(0, vec4(1, 1, 1, 1));
}