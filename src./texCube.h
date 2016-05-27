#ifndef TEXTURE_CUBES_H
#define TEXTURE_CUBES_H


#include "opengl.h"
#include "textureRefactor.h"

namespace tdogl{

class texCube: public textureRefactor{
	public:
		// default constructor to construct a texutre handle and associate a texture
		texCube();
		texCube(GLint format, GLsizei width, GLsizei height, GLenum srcDataFormat = GL_RGBA, GLenum srcDataType = GL_UNSIGNED_BYTE, const GLvoid *srcDataXp = (const GLvoid*) 0, const GLvoid *srcDataXn = (const GLvoid*) 0, const GLvoid *srcDataYp = (const GLvoid*) 0, const GLvoid *srcDataYn= (const GLvoid*) 0, const GLvoid *srcDataZp = (const GLvoid*) 0, const GLvoid *srcDataZn = (const GLvoid*) 0);
		//test viability of cubemap texture with given params
		static GLboolean proxy(GLint format, GLsizei width, GLsizei height, GLboolean mipmap);
		//unbind the texture object
		static void unbind();
		//allocate storage and intialise the texture data
		void image(GLenum face,GLint level, GLint format, GLsizei width, GLsizei height, GLenum srcDataFormat = GL_RGBA,GLenum srcDataType = GL_UNSIGNED_BYTE, const GLvoid *srcData = (const GLvoid*) 0);
		//upload to a subset of texture data
		void subImage(GLenum face, GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum srcDataFormat,GLenum srcDataType,const GLvoid *srcData);

private:
		explicit texCube(const texCube&);
		texCube& operator=(const texCube&);
};
	inline void texCube::unbind(){
	textureRefactor::unbind(GL_TEXTURE_CUBE_MAP);
	}

}


#endif