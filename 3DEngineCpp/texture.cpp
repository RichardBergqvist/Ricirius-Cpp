#include "texture.h"
#include "stb_image.h"
#include <iostream>
#include <cassert>

std::map<std::string, TextureData*> Texture::s_resourceMap;

TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters) {
	m_textureID = new GLuint[numTextures];
	m_textureTarget = textureTarget;
	m_width = width;
	m_height = height;
	m_numTextures = numTextures;

	initTextures(data, filters);
}

void TextureData::initTextures(unsigned char** data, GLfloat* filters) {
	glGenTextures(m_numTextures, m_textureID);
	for (int i = 0; i < m_numTextures; i++) {
		glBindTexture(m_textureTarget, m_textureID[i]);

		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, filters[i]);

		glTexImage2D(m_textureTarget, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
	}
}

TextureData::~TextureData() {
	if (*m_textureID) glDeleteTextures(m_numTextures, m_textureID);
	if (m_textureID) delete[] m_textureID;
}

void TextureData::bind(int textureNum) {
	glBindTexture(m_textureTarget, m_textureID[textureNum]);
}

Texture::Texture(const std::string& fileName, GLenum textureTarget, GLfloat filter) {
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

		m_textureData = new TextureData(textureTarget, x, y, 1, &data, &filter);
		stbi_image_free(data);
		
		s_resourceMap.insert(std::pair<std::string, TextureData*>(fileName, m_textureData));
	}
}

Texture::Texture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter) {
	m_fileName = "";
	m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter);
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

}
