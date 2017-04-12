#ifndef TEXTURE_H
#define TEXTURE_H

#include "../util/referenceCounter.h"
#include <GL/glew.h>
#include <string>
#include <map>

class TextureData : public ReferenceCounter {
public:
	TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp, GLenum* attachments);

	void bind(int textureNum) const;
	void bindAsRenderTarget() const;

	inline int getWidth()  const { return m_width; }
	inline int getHeight() const { return m_height; }

	virtual ~TextureData();
protected:
private:
	TextureData(TextureData& other) {}
	void operator=(TextureData& other) {}

	void initTextures(unsigned char** data, GLfloat* filter, GLenum* internalFormat, GLenum* format, bool clamp);
	void initRenderTargets(GLenum* attachments);

	GLuint* m_textureID;
	GLenum m_textureTarget;
	GLuint m_frameBuffer;
	GLuint m_renderBuffer;
	int m_numTextures;
	int m_width;
	int m_height;
};

class Texture {
public:
	Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clamp = false, GLenum attachment = GL_NONE);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clamp = false, GLenum attachment = GL_NONE);
	Texture(const Texture& texture);
	void operator=(Texture texture);
	virtual ~Texture();

	void bind(unsigned int unit = 0) const;
	void bindAsRenderTarget() const;

	inline int getWidth()  const { return m_textureData->getWidth(); }
	inline int getHeight() const { return m_textureData->getWidth(); }

	bool operator==(const Texture& texture) const { return m_textureData == texture.m_textureData; }
	bool operator!=(const Texture& texture) const { return !operator==(texture); }
protected:
private:
	static std::map<std::string, TextureData*> s_resourceMap;

	TextureData* m_textureData;
	std::string m_fileName;
};

#endif