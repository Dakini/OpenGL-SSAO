#ifndef TEXTURE2DEX_H
#define TEXTURE2DEX_H

#include "opengl.h"
#include "textureRefactor.h"

namespace tdogl{

class texture2DEX: public textureRefactor{
	public:
		//intial contructor , generate handle name and associate the texture with GL_TEXTURE_2D
		texture2DEX();
		texture2DEX(GLint format, GLsizei width, GLsizei height, GLenum srcDataFormat = GL_RGBA, 
		GLenum srcDataType = GL_UNSIGNED_BYTE, 
		const GLvoid *srcData = (const GLvoid*) 0);
		//test viability of 2D texture with given params
		static GLboolean proxy(GLint format, GLsizei width, GLsizei height, GLboolean mipmap);
		//unbind texture
		static void unbind();
		//allocate storage and intialise texture data
		void image(GLint level, GLint format, GLsizei width, GLsizei height, GLenum srcDataFormat = GL_RGBA,GLenum srcDataType = GL_UNSIGNED_BYTE, const GLvoid *srcData = (const GLvoid*) 0);
		//upload to a subset of texture data
		void subImage(GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum srcDataFormat,GLenum srcDataType,const GLvoid *srcData);

private:
		explicit texture2DEX(const texture2DEX&);
		texture2DEX& operator=(const texture2DEX&);
};
	inline void texture2DEX::unbind(){
	textureRefactor::unbind(GL_TEXTURE_2D);
	}

};

#endif