#ifndef MATERIAL_H
#define MATERIAL_H

#include "opengl.h"
#include "tex2d.h"
#include "iResource.h"

class Material: public iResource<Material>{
public:
	//load textures from files (including diffuse, specular, normal and addition detail maps) wont load the same file twice
	static Material *create(
		tex2D *diffuseTEx, tex2D *specTEx, tex2D *normalTEx, tex2D *detailTEx, const string &name);
	// destructor
	static void destroy(Material **ppTexture);
	//enable renderstate, bind texture 
	virtual void enable();
protected:
	//constructor
	Material(const string &name);
	tex2D *diffuse_, *normal_, *detail_, *specular_; //textures

};
inline void Material::destroy(Material**ppmaterial){
	assert(ppmaterial !=0);
	assert(*ppmaterial !=0);
	delete *ppmaterial;
	*ppmaterial =0;
}
inline Material::Material(const string &name): iResource(name){
}
#endif