#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H
#include "opengl.h"



namespace tdogl{
	//represents the model class
	//gonna be a work in progress
	class texture2D{
	public:
		//create a texture from the bitmap
		///texture will be uploaded upside down, 
		texture2D();
	void	textureLoad(const char* filename);
	void	textureLoad2(const char* filename);
		//delete texture
		~texture2D();

		void Bind(GLenum textureUnit);
		//textureUnit
		GLuint object() const;
	
	
	private:
		GLuint _object;
		string _fileName;

	};
}


#endif;