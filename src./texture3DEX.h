#ifndef TEXTURE3DEX_H
#define TEXTURE3DEX_H

#include "opengl.h"
#include "textureRefactor.h"

#define WIDTH 4
#define HEIGHT 4
#define DEPTH 4
#define BYTES_PER_TEXEL 3
#define LAYER(r) (WIDTH*HEIGHT*r*BYTES_PER_TEXEL)

//2->1 dimension mapping function
#define TEXEL2(s,t) (BYTES_PER_TEXEL* (s*WIDTH+t))
#define TEXEL3(s, t, r) (TEXEL2(s,t)+LAYER(r))

//header of the .vol file
struct volumeHeader{
	char magic[4];
	int version;
	char texName[256];
	bool wrap;
	int volsize;
	int numchannels;
	int bytesperchannel;
	};
namespace tdogl{

class texture3DEX: public textureRefactor{
	public:
		//intial contructor , generate handle name and associate the texture with GL_TEXTURE_2D
		texture3DEX();
		texture3DEX(GLint format, int volsize, GLenum srcDataFormat = GL_RGBA, 
		GLenum srcDataType = GL_UNSIGNED_BYTE, 
		const unsigned char*  srcData = (const unsigned char* ) "");
		//test viability of 2D texture with given params
		static GLboolean proxy(GLint format, int volsize);
		//unbind texture
		static void unbind();
		//allocate storage and intialise texture data
		void image(GLint level, GLint format, int volsize, GLenum srcDataFormat = GL_RGBA,GLenum srcDataType = GL_UNSIGNED_BYTE, const unsigned char* srcData = (const unsigned char* ) "");
		//upload to a subset of texture data
		void subImage(GLint level, GLint xOffset, GLint yOffset, GLint zoffset, int volsize, GLenum srcDataFormat,GLenum srcDataType,const unsigned char* srcData);

private:
		explicit texture3DEX(const texture3DEX&);
		texture3DEX& operator=(const texture3DEX&);
};
	inline void texture3DEX::unbind(){
	textureRefactor::unbind(GL_TEXTURE_3D);
	}

}

#endif