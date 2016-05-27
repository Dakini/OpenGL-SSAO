#ifndef TEXTUREREFACTOR_H
#define TEXTUREREFACTOR_H

#include "opengl.h"


namespace tdogl{

class textureRefactor{
public:

	~textureRefactor();
	//Return Texture objects handle, if its not intialiszed return GL_ZERO
	GLuint getHandle() const;
	//return target which texture is associated this could be gl_texture2d etc...
	GLuint getTarget() const;

	//return the internal format of the texture
	GLuint getFormat() const;

	//return texture width 
	GLsizei getWidth() const;
	//return texture height 
	GLsizei getHeight() const;
	GLsizei getDepth() const;
	//const accesss to texture width/height/depth as an array of 3
	const GLsizei * getSize() const;

	//return texelWidth, height and depth;
	GLfloat getTexelWidth() const;
	GLfloat getTexelHeight() const;
	GLfloat getTexelDepth() const;

	const GLfloat* getTexelSize() const;
	//return texture Param
	GLenum getMinFilter()const;
	GLenum getMagFilter()const;

	//set tex param
	void setMinFilter(GLenum);
	void setMagFilter(GLenum);

	GLfloat getMinLOD() const;
	GLfloat getMaxLOD() const;

	void setMinLOD(GLfloat);
	void setMaxLOD(GLfloat);

	GLint getBaseLevel() const;
	GLint getMaxLevel() const;

	void setBaseLevel(GLint);
	void setMaxLevel(GLint);

	GLenum getWrapS() const;
	GLenum getWrapT() const;
	GLenum getWrapR() const;

	void setWrapS(GLenum);
	void setWrapT(GLenum);
	void setWrapR(GLenum);
	void setWrap(GLenum);
	//return texture param for GL_TEXTURE_COMPARE_MODE
	GLenum getCompareMode() const;
	GLenum getCompareFunc() const;
	void setCompareMode(GLenum);
	void setCompareFunc(GLenum);

	void bind() const;
	static void unbind(GLenum target);
	void generateMipMap();
	//move ownership of a texture from src to dst
	static void Move(textureRefactor *src, textureRefactor * dst);

protected:

	textureRefactor();

	GLuint handle_;
	GLenum target_;

	GLint format_;
	GLsizei size_[3];
	GLfloat texelSize_[3];

	void init(GLenum target);
	void updateParameters();
private:
	explicit textureRefactor(const textureRefactor&);
	textureRefactor &operator = (const textureRefactor&);
};


inline void textureRefactor::bind()const{
	assert(handle_!=GL_ZERO);

	glBindTexture(target_, handle_);
}
inline void textureRefactor::unbind(GLenum target){
	glBindTexture(target, 0);
}



inline void textureRefactor::setCompareFunc(GLenum v){
	assert(handle_ !=GL_ZERO);
	assert(
		(v ==GL_LEQUAL) ||
		(v ==GL_GEQUAL) ||
		(v ==GL_LESS) ||
		(v ==GL_GREATER) ||
		(v ==GL_EQUAL) ||
		(v ==GL_NOTEQUAL) ||
		(v ==GL_ALWAYS) ||
		(v ==GL_NEVER) );
}
inline void textureRefactor::setCompareMode(GLenum v){
	assert(handle_ !=GL_ZERO);
	assert((v==GL_COMPARE_REF_TO_TEXTURE) || (v==GL_NONE));
	glTexParameteri(target_, GL_TEXTURE_COMPARE_MODE, (GLint)v);
}
inline GLenum textureRefactor::getCompareFunc() const {
	assert(handle_ !=GL_ZERO);
	GLint result;
	glGetTexParameteriv(target_, GL_TEXTURE_COMPARE_FUNC, &result);
	return (GLenum)result;
}
inline GLenum textureRefactor::getCompareMode() const {
	assert(handle_ !=GL_ZERO);
	GLint result;
	glGetTexParameteriv(target_, GL_TEXTURE_COMPARE_MODE, &result);
	return (GLenum)result;
}
inline void textureRefactor::setWrap(GLenum v){
	setWrapT(v);
	setWrapS(v);
	setWrapR(v);
}
inline void textureRefactor::setWrapR(GLenum v){
	assert(handle_!=GL_ZERO);
	assert(
		(v==GL_CLAMP_TO_EDGE) ||
		(v ==GL_MIRRORED_REPEAT) ||
		(v == GL_REPEAT) ||
		(v==GL_CLAMP_TO_BORDER));
	glTexParameteri(target_, GL_TEXTURE_WRAP_R, (GLint)v);
}

inline void textureRefactor::setWrapT(GLenum v){
	assert(handle_!=GL_ZERO);
	assert(
		(v==GL_CLAMP_TO_EDGE) ||
		(v ==GL_MIRRORED_REPEAT) ||
		(v == GL_REPEAT) ||
		(v==GL_CLAMP_TO_BORDER));
	glTexParameteri(target_, GL_TEXTURE_WRAP_T, (GLint)v);
}

inline void textureRefactor::setWrapS(GLenum v){
	assert(handle_!=GL_ZERO);
	assert(
		(v==GL_CLAMP_TO_EDGE) ||
		(v ==GL_MIRRORED_REPEAT) ||
		(v == GL_REPEAT) ||
		(v==GL_CLAMP_TO_BORDER));
	glTexParameteri(target_, GL_TEXTURE_WRAP_S, (GLint)v);
}

inline GLenum textureRefactor::getWrapR() const{
	assert(handle_ !=GL_ZERO);
	GLint result;
	glGetTexParameteriv(target_, GL_TEXTURE_WRAP_R, &result);
	return (GLenum)result;
}

inline GLenum textureRefactor::getWrapT() const{
	assert(handle_ !=GL_ZERO);
	GLint result;
	glGetTexParameteriv(target_, GL_TEXTURE_WRAP_T, &result);
	return (GLenum)result;
}

inline GLenum textureRefactor::getWrapS() const{
	assert(handle_ !=GL_ZERO);
	GLint result;
	glGetTexParameteriv(target_, GL_TEXTURE_WRAP_S, &result);
	return (GLenum)result;
}

inline void textureRefactor::setMaxLevel(GLint v){
	assert(handle_ !=GL_ZERO);
	glTexParameteri(target_, GL_TEXTURE_MAX_LEVEL, v);
}
inline void textureRefactor::setBaseLevel(GLint v){
	assert(handle_ !=GL_ZERO);
	glTexParameteri(target_, GL_TEXTURE_BASE_LEVEL, v);
}
inline GLint textureRefactor::getBaseLevel() const{
	assert(handle_ !=GL_ZERO);
	GLint result;
	glGetTexParameteriv(target_, GL_TEXTURE_BASE_LEVEL, &result);
	return result;
}
inline GLint textureRefactor::getMaxLevel() const{
	assert(handle_ !=GL_ZERO);
	GLint result;
	glGetTexParameteriv(target_, GL_TEXTURE_MAX_LEVEL, &result);
	return result;
}

inline void textureRefactor::setMaxLOD(GLfloat v) {
	assert(handle_!=GL_ZERO);
	glTexParameterf(target_, GL_TEXTURE_MAX_LOD, v);
}

inline void textureRefactor::setMinLOD(GLfloat v) {
	assert(handle_!=GL_ZERO);
	glTexParameterf(target_, GL_TEXTURE_MIN_LOD, v);
}

inline GLfloat textureRefactor::getMaxLOD() const{
	assert(handle_!=GL_ZERO);
	GLfloat result;
	glGetTexParameterfv(target_, GL_TEXTURE_MAX_LOD,&result);
	return result;
}
inline GLfloat textureRefactor::getMinLOD() const{
	assert(handle_!=GL_ZERO);
	GLfloat result;
	glGetTexParameterfv(target_, GL_TEXTURE_MIN_LOD,&result);
	return result;
}
inline void textureRefactor::setMagFilter(GLenum v){
	assert(handle_!=GL_ZERO);
	assert((v==GL_NEAREST) ||
		(v==GL_LINEAR));
	glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, (GLint)v);
}

inline void textureRefactor::setMinFilter(GLenum v){
	assert(handle_!=GL_ZERO);
	assert((v==GL_NEAREST) ||
		   (v==GL_LINEAR) ||
		   (v== GL_NEAREST_MIPMAP_NEAREST) ||
		   (v== GL_LINEAR_MIPMAP_NEAREST) ||
		   (v==GL_NEAREST_MIPMAP_LINEAR) ||
		   (v==GL_LINEAR_MIPMAP_LINEAR));

	glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, (GLint)v);
}
inline GLenum textureRefactor::getMagFilter() const{
	assert(handle_ !=GL_ZERO);
	GLint result;
	glGetTexParameteriv(target_, GL_TEXTURE_MAG_FILTER, &result);
	return (GLenum) result;
}

inline GLenum textureRefactor::getMinFilter() const{
	assert(handle_ !=GL_ZERO);
	GLint result;
	glGetTexParameteriv(target_, GL_TEXTURE_MIN_FILTER, &result);
	return (GLenum) result;
}
inline const GLfloat* textureRefactor::getTexelSize() const{
	return texelSize_;
}
 inline GLfloat textureRefactor::getTexelDepth() const{
	return texelSize_[2];
}
 inline  GLfloat textureRefactor::getTexelHeight() const{
	return texelSize_[1];
}
 inline   GLfloat textureRefactor::getTexelWidth() const{
	return texelSize_[0];
}


inline const GLsizei* textureRefactor::getSize() const{
	return size_;
}
 inline GLsizei textureRefactor::getDepth() const{
	return size_[2];
}
 inline GLsizei textureRefactor::getHeight() const{
	return size_[1];
}
inline  GLsizei textureRefactor::getWidth() const{
	return size_[0];
}

inline GLuint textureRefactor::getHandle() const{
	return handle_;
}
inline GLuint textureRefactor::getTarget() const{
	return target_;
}
inline GLuint textureRefactor::getFormat() const{
	return format_;
}

inline textureRefactor::textureRefactor():
handle_(GL_ZERO),
target_(GL_NONE),
format_(GL_NONE){
}
inline textureRefactor::~textureRefactor(){
	if(handle_!=GL_ZERO){
		glDeleteTextures(1, &handle_);

	}
}
}

#endif