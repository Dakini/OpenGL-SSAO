#ifndef TEXTURE3D_H
#define TEXTURE3D_H
#include "opengl.h"

#include "texture3DEX.h"
#include "textureRefactor.h"
#include "itexture.h"


class tex3D: public iTexture, public tdogl::texture3DEX{
	public:
		//load from file, if file is already loaded then dont load
		static tex3D * loadfromFile(
			const string &filename,
			unsigned int optionMask =0);
		//create a render texture or upload from memory
		static tex3D * create(
			GLint format, 
			int volsize, 
			GLenum srcDataformat = GL_RGB, 
			GLenum srcDataType = GL_UNSIGNED_BYTE, 
			unsigned char* srcData = (const unsigned char) 0, 
			unsigned optionMask = 0
			);
		virtual bool reload(string filename);

private:
	tex3D(const string &filename, unsigned optionMask);
};
inline tex3D::tex3D(const string &filename, unsigned int optionMask): iTexture(filename, optionMask){

}

#endif