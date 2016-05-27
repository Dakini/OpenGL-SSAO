#include "texture.h"

using namespace tdogl;

static GLenum textureFormatforBitmap(BitMap::Format format,bool rgb)
{
switch(format){

case BitMap::Format::Format_GrayScale: return GL_LUMINANCE;
case BitMap::Format::Format_GrayScaleAlpha: return GL_LUMINANCE_ALPHA;
case BitMap::Format::Format_RGB: return (rgb? GL_RGB:GL_RGB);
case BitMap::Format::Format_RGBA: return(rgb? GL_RGBA:GL_RGB);
default: throw runtime_error("Unrecognised bitmap format");

}

}

Texture::Texture(const BitMap& bitmap, GLint minMagFilter,GLint wrapMode): _orgWidth((GLfloat)bitmap.width()), _orgHeight((GLfloat)bitmap.height()){
	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_2D, _object);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,minMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrapMode);
	glTexImage2D(GL_TEXTURE_2D, 0,
		textureFormatforBitmap(bitmap.format(),true), 
		(GLsizei)bitmap.width(), 
		(GLsizei)bitmap.height(), 
		0,
		textureFormatforBitmap(bitmap.format(),false), 
		GL_UNSIGNED_BYTE,
		bitmap.pixelBuffer());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);

}

Texture::~Texture(){
	glDeleteTextures(1,&_object);
}

GLuint Texture::object() const{
	return _object;
}

GLfloat Texture::orgWidth() const{
	return _orgWidth;
}

GLfloat Texture::orgHeight() const{
	return _orgHeight;
}
