#include "texture2D.h"

using namespace tdogl;

texture2D::texture2D(){

}

texture2D::~texture2D(){
	glDeleteTextures(1, &_object);
}
void texture2D::textureLoad(const char* filename){
	
	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_2D, _object);
	ILuint imgId;
	int width, height, channels;
	ilGenImages(1,&imgId);
	ilBindImage(imgId);

	ILboolean success = ilLoadImage(filename);
	if(!success){
		cout<<"image not found"<<endl;
		   ILenum err = ilGetError() ;
    printf( "the error %d\n", err );
    printf( "string is %s\n", ilGetString( err ) );
	}

	width = (int)ilGetInteger(IL_IMAGE_WIDTH);
	height = (int)ilGetInteger(IL_IMAGE_HEIGHT);
	channels = (int)ilGetInteger(IL_IMAGE_CHANNELS);
	unsigned char* pixels = ilGetData();
	if((width &width-1) !=0 || (height &height-1)!=0){
		cout<<"warning: image is no a power of 2 dimension"<<endl;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	ilDeleteImages(1, &imgId);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


}
void texture2D::textureLoad2(const char* filename){
	ILuint image = ilGenImage();
	ilBindImage(image);
	ILboolean loadSuccess = ilLoadImage(filename);
	if(!loadSuccess){
		cout<<"Failed to load image" <<filename<<endl;
		   ILenum err = ilGetError() ;
			 printf( "the error %d\n", err );
		printf( "string is %s\n", ilGetString( err ) );
	}
	ILboolean convertSuccess = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	if(!convertSuccess){
		cout<<"Failed to convert image" <<filename<<endl;
		   ILenum err = ilGetError() ;
			 printf( "the error %d\n", err );
		printf( "string is %s\n", ilGetString( err ) );
	}

	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_2D, _object);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
	//glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	ilDeleteImage(image);
}
GLuint texture2D::object() const{
	return _object;
}