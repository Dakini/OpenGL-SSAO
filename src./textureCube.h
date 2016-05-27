#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H
#include "opengl.h"



namespace tdogl{
	//represents the model class
	//gonna be a work in progress
	class textureCube{
	public:
		//create a texture from the bitmap
		///texture will be uploaded upside down, 
		textureCube();
	void	textureCubeLoad(const char* directory, string posX, string negX,string posY, string negY, string posZ, string negZ);
		//delete texture
		~textureCube();
		bool load(GLuint texture, GLenum side_target,string file_name);
		void Bind(GLenum textureUnit);
		//textureUnit
		GLuint object() const;
	
	
	private:
		GLuint _object;
		string _fileNames[6];

	};
}


#endif;