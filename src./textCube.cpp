#include "textCube.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
texCu * texCu::loadfromFile(const string &filename,unsigned int optionMask ){
	assert(filename.c_str());
	//search for instance of cubemap
	for(int i =0; i<nInstances(); i++){
		if(instance(i).getFileName()){
			if(strcmp(instance(i).getFileName(), filename.c_str())==0)
			{
				return static_cast<texCu*>(&instance(i));
			}
		}
	}
	//load texture to cubemao
	texCu *result = new(std::nothrow) texCu(filename,optionMask);
	assert(result);
	//check options 
	if(result->checkOptionMask(TEX_MIPMAP)){
		result->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	}else{
		result->setMinFilter(GL_LINEAR);
	}
	if(result->reload(filename))
		return result;

	delete result;
	return 0;
}

void* loadData(const string &fileName, GLsizei * width, GLsizei * height, GLsizei *pixelSize, GLint *format, GLenum * srcDataFormat,GLenum * srcDataType,  float**datastuff){
	GLsizei width_, height_, pixelSize_;
	GLint format_;
	GLenum srcData, srcType;
	void *result = 0;
	int cmp;
	//load data using float instead of UNSIGNED_BYTE
	result = stbi_loadf(fileName.c_str(), &width_, &height_, &cmp, 0);
	*datastuff = stbi_loadf(fileName.c_str(), &width_, &height_, &cmp, 0);
	//no file then exit
	if(!result)
		exit(EXIT_FAILURE);
	//set the equivalent of 16f
	switch(cmp){
	case 1: format_ = GL_R16F; srcData = GL_RED; break;
	case 2: format_ = GL_RG16F; srcData = GL_RG; break;
	case 3: format_ = GL_RGB16F; srcData = GL_RGB; break;
	case 4: 
	default: srcData = format_ = GL_RGBA; break;
	};
	srcType = GL_FLOAT;
	pixelSize_ = sizeof(float) * cmp;
	//set outputs
	*width = width_;
	*height = height_;
	*pixelSize = pixelSize_;
	*format = format_;
	*srcDataFormat = srcData;
	* srcDataType = srcType;

	return result;
}
bool texCu::reload(string filename){

	if(filename.c_str()){//not a render texture
		GLsizei width, height, pixelSize;
		GLint format;
		int cmp;
		GLenum srcDataType, srcDataFormat;
		float *dataStuff;
		void *srcData = loadData(filename, &width, &height, &pixelSize, &format, &srcDataFormat, & srcDataType, &dataStuff);

		if(!dataStuff)
			return false;
		//bind the texture
		bind();
		int faceWidth = width;
		int faceHeight = height/6; //get height of each side of the cube map
		assert(faceWidth==faceHeight);
		if(checkOptionMask(TEX_SRGB)){
			if(format==GL_RGB)
				format==GL_SRGB;
			else if (format==GL_SRGB_ALPHA)
				format = GL_SRGB_ALPHA;
		}
		int faceSize = faceWidth * faceHeight * 3; //get total memory needed for each side
		//bind the image data to the correct side of the cube map
		//get unknow error when just trying to use srcdata so use datastuff instead
		image(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, faceWidth, faceHeight, srcDataFormat, srcDataType, dataStuff);
		image(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, faceWidth, faceHeight, srcDataFormat, srcDataType, dataStuff + (faceSize));
		image(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, faceWidth, faceHeight, srcDataFormat, srcDataType, dataStuff + (faceSize *2));
		image(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, faceWidth, faceHeight, srcDataFormat, srcDataType, dataStuff + (faceSize *3));
		image(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, faceWidth, faceHeight, srcDataFormat, srcDataType, dataStuff + (faceSize *4));
		image(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, faceWidth, faceHeight, srcDataFormat, srcDataType, dataStuff + (faceSize *5));
		delete[] srcData;
		delete[] dataStuff;
		//gen mipmaps
		if(checkOptionMask(TEX_MIPMAP))
			generateMipMap();
	}
	return true;
	
}

