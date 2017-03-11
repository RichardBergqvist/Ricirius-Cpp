#include "texture.h"
#include "stb_image.h"
#include <iostream>
#include <cassert>

std::map<std::string, TextureData*> Texture::s_resourceMap;

TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp, GLenum* attachments) {
	m_textureID = new GLuint[numTextures];
	m_textureTarget = textureTarget;
	m_width = width;
	m_height = height;
	m_numTextures = numTextures;
	m_framebuffer = 0;
	m_renderbuffer = 0;

	initTextures(data, filters, internalFormat, format, clamp);
	initRenderTargets(attachments);
}

TextureData::~TextureData() {
	if (*m_textureID) glDeleteTextures(m_numTextures, m_textureID);
	if (m_framebuffer) glDeleteFramebuffers(1, &m_framebuffer);
	if (m_renderbuffer) glDeleteRenderbuffers(1, &m_renderbuffer);
	if (m_textureID) delete[] m_textureID;
}

void TextureData::initTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp) {
	glGenTextures(m_numTextures, m_textureID);
	for (int i = 0; i < m_numTextures; i++) {
		glBindTexture(m_textureTarget, m_textureID[i]);

		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, filters[i]);

		if (clamp) {
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		glTexImage2D(m_textureTarget, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);
	}
}

void TextureData::initRenderTargets(GLenum* attachments) {
	if (attachments == 0)
		return;

	GLenum drawBuffers[32];
	assert(m_numTextures <= 32);

	bool hasDepth = false;
	for (int i = 0; i < m_numTextures; i++) {
		if (attachments[i] == GL_DEPTH_ATTACHMENT) {
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
		} else
			drawBuffers[i] = attachments[i];

		if (attachments[i] == GL_NONE)
			continue;
		if (m_framebuffer == 0) {
			glGenFramebuffers(1, &m_framebuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);
		}
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachments[i], m_textureTarget, m_textureID[i], 0);
	}
	
	if (m_framebuffer == 0)
		return;

	if (!hasDepth) {
		glGenRenderbuffers(1, &m_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
	}

	glDrawBuffers(m_numTextures, drawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer creation failed." << std::endl;
		assert(false);
	}
}

void TextureData::bind(int textureNum) {
	glBindTexture(m_textureTarget, m_textureID[textureNum]);
}

void TextureData::bindAsRenderTarget() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);
	glViewport(0, 0, m_width, m_height);
}
 
Texture::Texture(const std::string& fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attachment) {
 	m_fileName = fileName;

	std::map<std::string, TextureData*>::const_iterator it = s_resourceMap.find(fileName);
	if (it != s_resourceMap.end()) {
		m_textureData = it->second;
		m_textureData->addReference();
	} else {
		int x, y, bytesPerPixel;
		unsigned char* data = stbi_load(("./res/textures/" + fileName).c_str(), &x, &y, &bytesPerPixel, 4);

		if (data == NULL)
		{
			std::cerr << "Unable to load texture: " << fileName << std::endl;
		}

		m_textureData = new TextureData(textureTarget, x, y, 1, &data, &filter, &internalFormat, &format, clamp, &attachment);
		stbi_image_free(data);
		
		s_resourceMap.insert(std::pair<std::string, TextureData*>(fileName, m_textureData));
	}
}

Texture::Texture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attachment) {
	m_fileName = "";
	m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, clamp, &attachment);
}

Texture::~Texture() {
	if (m_textureData && m_textureData->removeReference()) {
		if (m_fileName.length() > 0)
			s_resourceMap.erase(m_fileName);
			
		delete m_textureData;
	}
}

void Texture::bind(unsigned int unit) const {
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	m_textureData->bind(0);
}

void Texture::bindAsRenderTarget() {
	m_textureData->bindAsRenderTarget();
}
