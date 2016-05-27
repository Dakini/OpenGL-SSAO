#include "texture3DEX.h"


namespace tdogl{

texture3DEX::texture3DEX(){
	textureRefactor::init(GL_TEXTURE_3D);
}
texture3DEX::texture3DEX(GLint format, int volsize, GLenum srcDataFormat, GLenum srcDataType, const unsigned char*srcData){
	textureRefactor::init(GL_TEXTURE_3D);
	image(0,format, volsize,srcDataFormat, srcDataType, srcData);
}

GLboolean texture3DEX::proxy(GLint format, int volsize){
	glTexImage3D(GL_PROXY_TEXTURE_3D, 0, format, volsize,volsize, volsize, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	GLint proxyFormat;
	GLsizei proxyWidth, proxyHeight, proxyDepth;
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_INTERNAL_FORMAT, &proxyFormat);
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &proxyWidth);
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_HEIGHT, &proxyHeight);
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_DEPTH, &proxyDepth);

	if((proxyFormat !=format) || (proxyWidth !=volsize) || (proxyHeight !=volsize) || (proxyDepth !=volsize))
		return GL_FALSE;

	return GL_TRUE;
}
void texture3DEX::image(GLint level, GLint format, int volsize, GLenum srcDataFormat ,GLenum srcDataType, const unsigned char *srcData){
	assert(handle_ !=GL_ZERO);
	//for depth textures
	if(format ==GL_DEPTH_COMPONENT){
		switch(srcDataFormat){
		case GL_DEPTH_COMPONENT:
		case GL_DEPTH_COMPONENT16:
		case GL_DEPTH_COMPONENT24:
		case GL_DEPTH_COMPONENT32:
		case GL_DEPTH_COMPONENT32F:
			break;
		default:
			srcDataFormat = GL_DEPTH_COMPONENT;
			break;
		};
	}
	//bind the tex data and update params
	glTexImage3D(target_, 0, format, volsize,volsize,volsize, 0, srcDataFormat, srcDataType, srcData);
	updateParameters();
}

void texture3DEX::subImage(GLint level, GLint xOffset, GLint yOffset, GLint zoffset, int volsize, GLenum srcDataFormat,GLenum srcDataType,const unsigned char*srcData){
	assert(handle_ !=GL_ZERO);
	assert(xOffset>= 0 && xOffset<=size_[0]);
	assert(yOffset>= 0 && yOffset<=size_[1]);
	assert(zoffset>= 0 && zoffset<=size_[2]);
	assert((volsize+xOffset) <=size_[0]);
	assert((volsize +xOffset) <=size_[1]);
	assert((volsize +zoffset) <=size_[2]);
	glTexSubImage3D(target_, level, xOffset, yOffset,zoffset, volsize , volsize ,volsize , srcDataFormat, srcDataType, srcData);
}
}