#include "tex3d.h"


tex3D * tex3D::loadfromFile(const string &filename,unsigned int optionMask ){
	assert(filename.c_str());
	//search for existing texture instance
	for(int i =0; i<nInstances(); i++){
		if(instance(i).getFileName()){
			if(strcmp(instance(i).getFileName(), filename.c_str())==0)
			{
				return static_cast<tex3D*>(&instance(i));
			}
		}
	}
	
	tex3D *result = new(std::nothrow) tex3D(filename,optionMask);
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
tex3D * tex3D::create(
			GLint format, 
			int volsize,
			GLenum srcDataformat, 
			GLenum srcDataType, 
			unsigned char*  srcData , 
			unsigned optionMask 
			){
				//create inital texture
				tex3D * result = new(std::nothrow) tex3D("", optionMask);
				assert(result);
				if(result->checkOptionMask(TEX_MIPMAP)){
				result->setMinFilter(GL_LINEAR);
				}else{
					result->setMinFilter(GL_LINEAR);
				}
				result->setWrap(GL_REPEAT);
				result->image(0, format, volsize, srcDataformat, srcDataType, srcData);

				return result;
}
unsigned char * loadData(string filename, int *volSize){
	//open .vol file
	FILE* fin = fopen(filename.c_str(), "rb");
	if(!fin){
		cout<<"couldn't open file"<<endl;
	}
	//create and read buffer
	char buffer[4096];
	fread(buffer, 4096,1,fin);

	volumeHeader * header = (volumeHeader*)buffer;
	//read the header of the .volume file
	if((header->magic[0]!='V') ||
		(header->magic[1]!='O') ||
		(header->magic[2]!='L') ||
		(header->magic[3]!='U') ||
		(header->bytesperchannel!=1)||
		(header->version !=4)){
	cout<<"Bad Header"<<endl;		
	}
	//
	*volSize = header->volsize;
	//get volume size and number of channels
	int numChannels = header->numchannels;
	if(numChannels!=3 && numChannels!=4){
		cout<<"only rgb or RGBA samples excepted"<<endl;
	}
	//allocate memory and read in the file in bytes
	int volsize = header->volsize;
	int volBytes = volsize* volsize*volsize * numChannels;

	vector<GLubyte> volIn(volBytes);
	fread(&volIn[0], volBytes,1,fin);
	unsigned char *texels = (BYTE *)malloc(volBytes);
	fclose(fin);
	//send the volume data to a unsigned char * so it can be uplaoded to the GPU
	for(int iz= 0; iz<volsize;iz++){
		int offz = volsize*volsize*iz;
		for(int iy= 0; iy<volsize;iy++){
			int offy = volsize*iy;
			for(int ix= 0; ix<volsize;ix++){
				int index = ix+offy+offz;
				int indexIn = index*numChannels;
				unsigned char r = volIn[indexIn];
				unsigned char g = volIn[indexIn+1];
				unsigned char b = volIn[indexIn+2];
				int indexOut = index*3;
				texels[indexOut] = r;
				texels[indexOut+1] = g;
				texels[indexOut+2] = b;
			}
		}
	}
	cout<<"loaded a solid texture"<<endl;
	return texels;
}

bool tex3D::reload(string filename){
	//load texture
	assert(filename.c_str());
	volumeHeader header;
	GLint format;
	GLenum srcDataFormat, srcDataType;
	//load texture file
	int volSize;
	//load .vol file
	unsigned char* srcData =loadData(filename, &volSize);
	cout<<volSize<<endl;
	format = srcDataFormat = GL_RGB;
	srcDataType = GL_UNSIGNED_BYTE; //set as unsigned byte

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
	image(0, format,volSize, srcDataFormat, srcDataType, srcData);
	delete[]srcData;
	//generate mipmap if option has been set
	if(checkOptionMask(TEX_MIPMAP))
		generateMipMap();

	return true;

}


/*
Tex3d::Tex3d(){

}

void Tex3d::start(char* file){
	
	int volSize;
	//load .vol file
	unsigned char* voxels =loadFromFile(file, volSize);
	//this is a 3D texture, level 0 (max detail), GL should store it in RGB8 format, its w*h*d as max size
	//it doesnt have a border, were giving it to gl in an rgb format as a series of unsigned bytes and texels is where the data is 
	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_3D, _object);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	//width, height and depth hard coded to 
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, volSize,volSize,volSize, 0, GL_RGB, GL_UNSIGNED_BYTE, voxels);
	glGenerateMipmap(GL_TEXTURE_3D);
	if(glTexImage3D==NULL){
		cout<<"There is no 3D texture created"<<endl;
	}
	delete[] voxels;


}

unsigned char* Tex3d::loadFromFile(char* filename, int & volSize){

	FILE* fin = fopen(filename, "rb");
	if(!fin){
		cout<<"couldn't open file"<<endl;
	}
	char buffer[4096];
	fread(buffer, 4096,1,fin);

	volumeHeader * head = (volumeHeader*)buffer;

	if((head->magic[0]!='V') ||
		(head->magic[1]!='O') ||
		(head->magic[2]!='L') ||
		(head->magic[3]!='U') ||
		(head->bytesperchannel!=1)||
		(head->version !=4)){
	cout<<"Bad Header"<<endl;		
	}

	cout<<head->volsize<<endl;
	volSize = head->volsize;
	int numChannels = head->numchannels;
	cout<<numChannels<<endl;
	if(numChannels!=3 && numChannels!=4){
		cout<<"only rgb or RGBA samples excepted"<<endl;
	}
	int volsize = head->volsize;
	int volBytes = volsize* volsize*volsize * numChannels;
	vector<GLubyte> volIn(volBytes);
	fread(&volIn[0], volBytes,1,fin);
	unsigned char *texels = (BYTE *)malloc(volBytes);
	fclose(fin);

	for(int iz= 0; iz<volsize;iz++){
		int offz = volsize*volsize*iz;
		for(int iy= 0; iy<volsize;iy++){
			int offy = volsize*iy;
			for(int ix= 0; ix<volsize;ix++){
				int index = ix+offy+offz;
				int indexIn = index*numChannels;
				unsigned char r = volIn[indexIn];
				unsigned char g = volIn[indexIn+1];
				unsigned char b = volIn[indexIn+2];
				int indexOut = index*3;
				texels[indexOut] = r;
				texels[indexOut+1] = g;
				texels[indexOut+2] = b;
			}
		}
	}
	cout<<"loaded a solid texture"<<endl;
	return texels;
}

Tex3d::~Tex3d(){
	glDeleteTextures(1,&_object);
}

GLuint Tex3d::object() {
	return _object;
}
*/