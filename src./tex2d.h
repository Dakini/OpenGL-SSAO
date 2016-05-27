#ifndef TEX2D_H
#define TEX2D_H

#include "opengl.h"
#include "texture2DEX.h"
#include "itexture.h"

class tex2D: public iTexture, public tdogl::texture2DEX{
	public:
		//load from file, if file is already loaded then dont load
		static tex2D * loadfromFile(
			const string &filename,
			unsigned int optionMask =0);
		//create a render texture or upload from memory
		static tex2D * create(
			GLint format, 
			GLsizei width, 
			GLsizei height, 
			GLenum srcDataformat = GL_RGBA, 
			GLenum srcDataType = GL_UNSIGNED_BYTE, 
			const GLvoid* srcData = (const GLvoid*) 0, 
			unsigned optionMask = 0
			);
		virtual bool reload(string filename);

private:
	tex2D(const string &filename, unsigned optionMask);
};
inline tex2D::tex2D(const string &filename, unsigned int optionMask): iTexture(filename, optionMask){
}

#endif