#ifndef TEXTURE_H
#define TEXTURE_H
#include "opengl.h"


namespace tdogl{
	//represents the OPENGL TEXTURE
	class Texture{
	public:
		//create a texture from the bitmap
		///texture will be uploaded upside down, 
		Texture(const BitMap& bitmap, GLint mingMagFilter = GL_LINEAR, GLint wrapMode = GL_CLAMP_TO_EDGE);
		//delete texture
		~Texture();
		//Texture Object
		GLuint object() const;
		// the original widht in pixels of the bitmap
		GLfloat orgWidth() const;
		// the original widht in pixels of the bitmap
		GLfloat orgHeight() const;
	private:
		GLuint _object;
		GLuint _orgWidth;
		GLuint _orgHeight;
		Texture(const Texture&);
		const Texture& operator =(const Texture&);
	};
}


#endif