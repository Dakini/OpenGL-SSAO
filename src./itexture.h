#ifndef ITEXTURE_H
#define ITEXTURE_H
#include "opengl.h"
#include "iResource.h"


//base class for textures. mixed in with tex2D etc. to get objects which work as textures and resources
class iTexture: public iResource<iTexture>{

public: 
	//mask for options
	enum optionMasks{
		TEX_SRGB = 1,
		TEX_MIPMAP=2,
		TEX_SRGB_MIPMAP =3
	};
	//destory this texture instance
	static void destroy(iTexture **ppTexture);
	//set and get filename
	void setFileName(const string &);
	const char* getFileName();
	//get set and check option mask
	unsigned getOptionMask() const;
	void setOptionMask(optionMasks);
	bool checkOptionMask(optionMasks);
	// reload the texture object
	virtual bool reload(string filename) =0;
	//reload all texture instances
	static bool reloadAll();
protected:
	string fileName_; //filename
	unsigned optionMask_; //options for option mask

	//constructor
	iTexture(const string &filename, unsigned optionMask);

	//destructor
	virtual ~iTexture();
};

inline const char* iTexture::getFileName(){
	return fileName_.c_str();
}
inline void iTexture::setFileName(const string & fileName){
	assert(fileName.c_str());
	fileName_ = fileName;
}
inline unsigned iTexture::getOptionMask() const{
	return optionMask_;
}
inline void iTexture::setOptionMask(optionMasks mask){
	optionMask_ = mask;
}
inline bool iTexture::checkOptionMask(optionMasks opt){
	return opt & optionMask_;
}

inline iTexture::iTexture(const string &fileName, unsigned optionMask): iResource(fileName), fileName_(fileName), optionMask_(optionMask){
}

inline iTexture::~iTexture(){

}



#endif