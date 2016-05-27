#include "textureCube.h"

using namespace tdogl;

textureCube::textureCube(){

}

textureCube::~textureCube(){

}
void textureCube::textureCubeLoad(const char* directory, string posX, string negX,string posY, string negY, string posZ, string negZ){
	//glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, &_object);

	assert(load(_object, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, negZ)); 
	assert(load(_object, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, posZ)); 
	assert(load(_object, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, negY)); 
	assert(load(_object, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, posY)); 
	assert(load(_object, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, negX)); 
	assert(load(_object, GL_TEXTURE_CUBE_MAP_POSITIVE_X, posX)); 
	
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


}
bool textureCube::load(GLuint texture, GLenum side_target, string file_name){

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	ILuint imgId;
	int width, height, channels;
	ilGenImages(1,&imgId);
	ilBindImage(imgId);
	

	ILboolean success = ilLoadImage(file_name.c_str());
	if(!success){
		cout<<"image not found"<<endl;
	}
	width = (int)ilGetInteger(IL_IMAGE_WIDTH);
	height = (int)ilGetInteger(IL_IMAGE_HEIGHT);
	channels = (int)ilGetInteger(IL_IMAGE_CHANNELS);
	unsigned char* pixels = ilGetData();
	if((width &width-1) !=0 || (height &height-1)!=0){
		cout<<"warning: image is no a power of 2 dimension"<<endl;
	}
	glTexImage2D(side_target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	ilDeleteImages(1, &imgId);
	return true;
}
GLuint textureCube::object() const{
	return _object;
}