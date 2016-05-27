#include "texCube.h"
namespace tdogl{
texCube::texCube(){
	textureRefactor::init(GL_TEXTURE_CUBE_MAP);
}
//constructor
texCube::texCube(GLint format, GLsizei width, GLsizei height, GLenum srcDataFormat , GLenum srcDataType, const GLvoid *srcDataXp, const GLvoid *srcDataXn , const GLvoid *srcDataYp , const GLvoid *srcDataYn, const GLvoid *srcDataZp, const GLvoid *srcDataZn ){
	textureRefactor::init(GL_TEXTURE_CUBE_MAP);
	//bind the image data to the right side of the cube map
	image(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, srcDataFormat, srcDataType, srcDataXp);
	image(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, srcDataFormat, srcDataType, srcDataXn);
	image(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, srcDataFormat, srcDataType, srcDataYp);
	image(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, srcDataFormat, srcDataType, srcDataYn);
	image(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width, height, srcDataFormat, srcDataType, srcDataZp);
	image(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width, height, srcDataFormat, srcDataType, srcDataZn);
}
		
GLboolean texCube::proxy(GLint format, GLsizei width, GLsizei height, GLboolean mipmap){
	glTexImage2D(GL_PROXY_TEXTURE_CUBE_MAP, (GLint)mipmap, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	GLint proxyFormat;
	GLsizei proxyWidth, proxyHeight;
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_CUBE_MAP, 0, GL_TEXTURE_INTERNAL_FORMAT, &proxyFormat);
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_CUBE_MAP, 0, GL_TEXTURE_WIDTH, &proxyWidth);
	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_CUBE_MAP, 0, GL_TEXTURE_HEIGHT, &proxyHeight);

	if((proxyFormat !=format) || (proxyWidth != width) || (proxyHeight!=height))
		return GL_FALSE;
	return GL_TRUE;
}
void texCube::image(
	GLenum face,
	GLint level,
	GLint format,
	GLsizei width,
	GLsizei height,
	GLenum srcDataFormat,
	GLenum srcDataType,
	const GLvoid *srcData
) {

	assert(handle_ !=GL_ZERO);
	assert((face==GL_TEXTURE_CUBE_MAP_POSITIVE_X) ||
		(face==GL_TEXTURE_CUBE_MAP_NEGATIVE_X) ||
		(face==GL_TEXTURE_CUBE_MAP_POSITIVE_Y) ||
		(face==GL_TEXTURE_CUBE_MAP_NEGATIVE_Y) ||
		(face==GL_TEXTURE_CUBE_MAP_POSITIVE_Z) ||
		(face==GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) );

	if(format == GL_DEPTH_COMPONENT){
		switch (srcDataFormat)
		{
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

	//bind and update params

	glTexImage2D(face, 0, format, width, height, 0,  srcDataFormat, srcDataType, srcData);
	updateParameters();

}

void texCube::subImage(GLenum face, GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum srcDataFormat,GLenum srcDataType,const GLvoid *srcData){
	assert(handle_ !=GL_ZERO);
	assert(xOffset >=0 && xOffset <= size_[0]);
	assert(yOffset >=0 && yOffset <= size_[1]);
	assert((xOffset + width ) <= size_[0]);
	assert((yOffset +height ) <= size_[1]);
	assert((face==GL_TEXTURE_CUBE_MAP_POSITIVE_X) ||
		(face==GL_TEXTURE_CUBE_MAP_NEGATIVE_X) ||
		(face==GL_TEXTURE_CUBE_MAP_POSITIVE_Y) ||
		(face==GL_TEXTURE_CUBE_MAP_NEGATIVE_Y) ||
		(face==GL_TEXTURE_CUBE_MAP_POSITIVE_Z) ||
		(face==GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) );
	glTexSubImage2D(face, level, xOffset, yOffset, width, height, srcDataFormat, srcDataType, srcData);
}
}