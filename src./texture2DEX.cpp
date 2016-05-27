#include "texture2DEX.h"

namespace tdogl{
//constructor
texture2DEX::texture2DEX(){
	textureRefactor::init(GL_TEXTURE_2D);
}
//constructor
texture2DEX::texture2DEX(GLint format, GLsizei width, GLsizei height, GLenum srcDataFormat, GLenum srcdataType, const GLvoid*srcData)
{
	textureRefactor::init(GL_TEXTURE_2D);
	//bind image data
	image(0,format, width, height, srcDataFormat,srcdataType, srcData);
}
GLboolean texture2DEX::proxy(GLint format, GLsizei width ,GLsizei height, GLboolean mipmap){
	glTexImage2D(GL_PROXY_TEXTURE_2D, (GLint)mipmap, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	GLint proxyFormat;
	GLsizei proxyWidth, proxyHeight;
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &proxyFormat);
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &proxyWidth);
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &proxyHeight);

	if((proxyFormat !=format) || (proxyWidth !=width) || (proxyHeight !=height))
		return GL_FALSE;

	return GL_TRUE;
}
void texture2DEX::image(GLint level, GLint format, GLsizei width, GLsizei height, GLenum srcDataFormat, GLenum srcdataType,const GLvoid*srcData){
	assert(handle_ !=GL_ZERO);
	//for dpeth textures
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
	glTexImage2D(target_, 0, format, width, height, 0, srcDataFormat, srcdataType, srcData);
	updateParameters();
	}

void texture2DEX::subImage(GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum srcDataFormat,GLenum srcDataType,const GLvoid *srcData){
	assert(handle_ !=GL_ZERO);
	assert(xOffset>= 0 && xOffset<=size_[0]);
	assert(yOffset>= 0 && yOffset<=size_[1]);
	assert((width+xOffset) <=size_[0]);
	assert((height+xOffset) <=size_[1]);
	glTexSubImage2D(target_, level, xOffset, yOffset, width, height, srcDataFormat, srcDataType, srcData);
}
};