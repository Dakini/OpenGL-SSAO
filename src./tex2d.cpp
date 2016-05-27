#include "tex2d.h"

#include "stb_image.h"


tex2D * tex2D::create(
			GLint format, 
			GLsizei width, 
			GLsizei height, 
			GLenum srcDataformat, 
			GLenum srcDataType, 
			const GLvoid* srcData , 
			unsigned optionMask 
			){
				//create inital texture
				tex2D * result = new(std::nothrow) tex2D("", optionMask);
				assert(result);
				if(result->checkOptionMask(TEX_MIPMAP)){
				result->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
				}else{
					result->setMinFilter(GL_LINEAR);
				}
			
				result->image(0, format, width, height, srcDataformat, srcDataType, srcData);

				return result;
}

tex2D * tex2D::loadfromFile(const string &filename,unsigned int optionMask ){
	assert(filename.c_str());
	//search for existing texture instance
	for(int i =0; i<nInstances(); i++){
		if(instance(i).getFileName()){
			if(strcmp(instance(i).getFileName(), filename.c_str())==0)
			{
				return static_cast<tex2D*>(&instance(i));
			}
		}
	}
	
	tex2D *result = new(std::nothrow) tex2D(filename,optionMask);
	assert(result);
	//set option mask for linear mipmapping and GL_wrap to GL_REPEAT
	if(result->checkOptionMask(TEX_MIPMAP)){
		result->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
		result->setWrap(GL_REPEAT);
	}else{
		result->setMinFilter(GL_LINEAR);
		result->setWrap(GL_REPEAT);
	}
	//load image
	if(result->reload(filename))
		return result;

	delete result;
	return 0;
}


void* loadData2(const string &fileName, GLsizei * width, GLsizei * height, GLsizei *pixelSize, GLint *format, GLenum * srcDataFormat,GLenum * srcDataType,  float**datastuff){
	GLsizei width_, height_, pixelSize_;
	GLint format_;
	GLenum srcData, srcType; //src data and type
	void *result = 0; //pixels
	int cmp; //chanels
	//if file is a .dds
	if(fileName.substr(fileName.find_last_of(".") +1)=="dds"){
	//load via devil library
	ILuint imgId;
	int width, height, channels;
	ilGenImages(1,&imgId);
	ilBindImage(imgId);

	ILboolean success = ilLoadImage(fileName.c_str());
	width_ = (int)ilGetInteger(IL_IMAGE_WIDTH);
	height_ = (int)ilGetInteger(IL_IMAGE_HEIGHT);
	cmp = (int)ilGetInteger(IL_IMAGE_CHANNELS);
	result = ilGetData(); //get pixel data
	
	}else{
	//load via stb_image.h
	result = stbi_load(fileName.c_str(), &width_, &height_, &cmp, 0);
	}
	//if there is an error then exit
	if(!result)
		exit(EXIT_FAILURE);
	//depending on channels change format 
	switch(cmp){
	case 1: format_ = srcData = GL_RED; break;
	case 2: format_ = srcData = GL_RG; break;
	case 3: format_ = srcData = GL_RGB; break;
	case 4: 
	default: srcData = format_ = GL_RGBA; break;
	};
	srcType = GL_UNSIGNED_BYTE; //set as unsigned byte
	pixelSize_ =   cmp* sizeof(unsigned char); 
	//set variables
	*width = width_;
	*height = height_;
	*pixelSize = pixelSize_;
	*format = format_;
	*srcDataFormat = srcData;
	* srcDataType = srcType;

	return result;
}

void flipData(void* data, int width, int height, int pixelSize){
	assert(data);
	int imgSize = width* height * pixelSize;
	int rowSize = width* pixelSize;
	char* img = (char*)data; //access pixel data
	char* tmp = new(std::nothrow) char[rowSize];

	assert(tmp);
	//flip data
	for(int i =0; i<height/2; i++){ //for each row
		int rowStart = i*rowSize; //start of current row
		assert(rowStart<imgSize);
		int rowSwap = imgSize-rowSize-rowStart; //start of row to swap
		for(int j=0; j<rowSize; j++)
			tmp[j] = img[rowStart + j];
		for(int j=0; j<rowSize; j++)
			img[rowStart +j] = img[rowSwap + j];
		for(int j=0; j<rowSize; j++)
			img[rowSwap + j] =tmp[j]; 
	}
	delete[]tmp;
	}
bool tex2D::reload(string filename){
	//load texture
	assert(filename.c_str());
	
	GLsizei width, height, pixelSize;
	GLint format;
	GLenum srcDataFormat, srcDataType;
	float * dataStuff;
	//load texture file
	void* srcData = loadData2(filename, &width, &height, &pixelSize, &format, &srcDataFormat, &srcDataType, &dataStuff);
	//if no data return false
	if(!srcData)
		return false;
	//bind texture
	bind();
	
	//use SRGB if needed
	if(checkOptionMask(TEX_SRGB)){
	if(format==GL_RGB)
		format = GL_SRGB;
	}
	else if (format == GL_SRGB_ALPHA){
		format = GL_SRGB_ALPHA;
	}
	//upload image information
	image(0, format, width, height, srcDataFormat, srcDataType, srcData);
	//delete[]srcData;
	//generate mipmap if option has been set
	if(checkOptionMask(TEX_MIPMAP))
		generateMipMap();

	return true;

}
