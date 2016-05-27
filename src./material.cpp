#include "Material.h"

//creatw new Material 
Material * Material::create(tex2D * diffuse, tex2D * spec, tex2D * normal, tex2D * detail, const string &name){
	
	Material *mat = new(std::nothrow) Material(name);
	assert(mat);
	//bind textures
	mat->diffuse_= diffuse;
	mat->specular_ = spec;
	mat->normal_ = normal;
	mat->detail_ = detail;
	return mat;
}
//enable textures
void Material::enable(){
	glActiveTexture(GL_TEXTURE0);
	diffuse_->bind();
	glActiveTexture(GL_TEXTURE1);
	normal_->bind();
	glActiveTexture(GL_TEXTURE2);
	detail_->bind();
	glActiveTexture(GL_TEXTURE3);
	specular_->bind();

}