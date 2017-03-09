#ifndef TEXTURE_H
#define TEXTURE_H

#include "referenceCounter.h"
#include <GL/glew.h>
#include <string>
#include <map>

class TextureData : public ReferenceCounter {
public:
	TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters);

	void bind(int textureNum);

	virtual ~TextureData();
protected:	
private:
	TextureData(TextureData& other) {}
	void operator=(TextureData& other) {}

	void initTextures(unsigned char** data, GLfloat* filter);

	GLuint* m_textureID;
	GLenum m_textureTarget;
	int m_numTextures;
	int m_width;
	int m_height;
};

class Texture
{
public:
	Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
	virtual ~Texture();

	void bind(unsigned int unit = 0) const;	
	void bindAsRenderTarget();
protected:
private:
	static std::map<std::string, TextureData*> s_resourceMap;

	Texture(Texture& texture) {}
	void operator=(Texture& texture) {}

	TextureData* m_textureData;
	std::string m_fileName;
};

#endif
