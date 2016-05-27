#include "textureRefactor.h"

namespace tdogl{
	//gen mip map
	void textureRefactor::generateMipMap(){
		//make sure there is a handle
		assert(handle_!=GL_ZERO);
		//assert that the target is one of these 
		assert(
			(target_ == GL_TEXTURE_2D) ||
			(target_ == GL_TEXTURE_1D) ||
			(target_==  GL_TEXTURE_3D) ||
			(target_==GL_TEXTURE_1D_ARRAY) ||
			(target_==GL_TEXTURE_CUBE_MAP) ||
			(target_==GL_TEXTURE_2D_ARRAY));
		//gen mipmap for the target_
		glGenerateMipmap(target_);
		
	}
	//move the texture from one to the other
	void textureRefactor::Move(textureRefactor*src, textureRefactor *dst){
		assert(src->handle_!=GL_ZERO);
		assert(dst->handle_==GL_ZERO);
		//copy data across
		dst->handle_ = src->handle_;
		dst->target_ = src->target_;
		dst->format_ = src->format_;
		dst->size_[0] = src->size_[0];
		dst->size_[1] = src->size_[1];
		dst->size_[2] = src->size_[2];

		dst->texelSize_[0] = src->texelSize_[0];
		dst->texelSize_[1] = src->texelSize_[1];
		dst->texelSize_[2] = src->texelSize_[2];

	
		src->handle_ = GL_ZERO;
		src->target_ = GL_NONE;
	}
//intialisers
void textureRefactor::init(GLenum target){
		//set texture target as target(i.e. GL_TEXTURE_2D)
		target_ = target;
		//set sizes to 1
		size_[0] = size_[1] = size_[2] = GLsizei(1);
		texelSize_[0] = texelSize_[1] = texelSize_[2] = GLfloat(1);
		//gen texture to handle and bind it 
		glGenTextures(1, &handle_);
		bind();
	}

	void textureRefactor::updateParameters(){
		GLenum tg = target_;
		//get target and update texture params
		if(tg ==GL_TEXTURE_CUBE_MAP){
			tg = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		}
		glGetTexLevelParameteriv(tg, 0, GL_TEXTURE_INTERNAL_FORMAT, &format_);
		glGetTexLevelParameteriv(tg, 0, GL_TEXTURE_WIDTH, &size_[0]);
		glGetTexLevelParameteriv(tg, 0, GL_TEXTURE_HEIGHT, &size_[1]);

		glGetTexLevelParameteriv(tg, 0, GL_TEXTURE_DEPTH, &size_[2]);

		texelSize_[0] = GLfloat(1)/ GLfloat(size_[0]);
		texelSize_[1] = GLfloat(1)/ GLfloat(size_[1]);
		texelSize_[2] = GLfloat(1)/ GLfloat(size_[2]);
	}
}