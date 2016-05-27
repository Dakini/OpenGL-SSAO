#ifndef FRAMEBUFFEREX_H
#define FRAMEBUFFEREX_H

#include "opengl.h"
#include "textureRefactor.h"
#include "texture2DEX.h"
namespace tdogl{

	class FrameBufferEX{
	public:
		//constructors
		FrameBufferEX(GLenum target = GL_FRAMEBUFFER);
		FrameBufferEX(GLsizei width, GLsizei height, GLenum target = GL_FRAMEBUFFER);
		//destructor
		~FrameBufferEX();
		//return attached texture
		//get texture attachment
		const textureRefactor* getAttachment(GLenum attachment) const;
		//get handle and status of framebuffer
		GLuint getHandle() const;
		GLenum getStatus() const;
		//get/set width and height
		GLsizei getWidth() const;
		GLsizei getHeight() const;
		const GLsizei * getSize() const;
		void setSize(GLsizei width, GLsizei height);
		//get and set texel size
		GLfloat getTexelWidth() const;
		GLfloat getTexelHeight() const;
		const GLfloat* getTexelSize() const;
		void setTexelSize() const;
		//bind the frame buffer
		void bind(GLenum target = GL_FRAMEBUFFER)const;
		//unbind the framebuffer
		static void unbind(GLenum target = GL_FRAMEBUFFER);
		//bind readframebuffer
		void bindRead() const;
		//gl bind draw framebuffer
		void bindDraw() const;
		//attach a 2D texture
		void attach(const texture2DEX* tex, GLenum Attachment, GLint level=0);
	private:
		
		GLuint handle_; //frame buffer handle
		GLenum status_;//framebuffer status
		GLsizei size_[2]; //frame buffer width and height
		GLfloat texelSize_[2]; //frame buffer texel sizes

		struct TEXTURE_ATTACHMENT;
		TEXTURE_ATTACHMENT * textureAttachments;
		GLsizei nTextureAttachments_; //num of texture attachments
		GLint maxTexAttachments_; //max amount

		GLenum * drawBuffers_; 
		GLsizei nDrawBuffers_;
		GLint maxDrawBuffers_;

		//explicit constructor and operator
		explicit FrameBufferEX(const FrameBufferEX&);
		FrameBufferEX& operator = (const FrameBufferEX&);
		void init();
		void addTextureAttachment(const TEXTURE_ATTACHMENT &texa);
		void setDrawBuffers();
		void setSize();
		void setStatus();
	};

inline 	GLuint FrameBufferEX::getHandle() const{
		return handle_; 
	}
inline 		GLenum FrameBufferEX::getStatus() const{
			return status_; 
	}
inline 	GLsizei FrameBufferEX::getWidth() const{
		return size_[0];
	}
inline 	GLsizei FrameBufferEX::getHeight() const{
		return size_[1];
	}
inline const	GLsizei* FrameBufferEX::getSize() const{
		return size_;
	}
inline void FrameBufferEX::setSize(GLsizei width, GLsizei height){
	assert(handle_ !=GL_ZERO); //make sure there is a handle
	size_[0] = width;
	size_[1]= height;
	texelSize_[0] = 1/width;
	texelSize_[1] = 1/height;
}

inline GLfloat FrameBufferEX::getTexelWidth() const{
	return texelSize_[0];
	}
inline GLfloat FrameBufferEX::getTexelHeight() const{
	return texelSize_[1];
}

inline const GLfloat* FrameBufferEX::getTexelSize() const{
	return texelSize_;
}


inline void FrameBufferEX::bind(GLenum target) const{ 
	assert((target == GL_READ_FRAMEBUFFER) || //make sure the target one of the framebuffers
		(target == GL_DRAW_FRAMEBUFFER) ||
		(target == GL_FRAMEBUFFER));
	glBindFramebuffer(target, handle_); //bind that target to handle
}

inline void FrameBufferEX::bindRead() const{
	bind(GL_READ_FRAMEBUFFER); 
}

inline void FrameBufferEX::bindDraw() const{
	bind(GL_DRAW_FRAMEBUFFER);
}
inline void FrameBufferEX::unbind(GLenum target) {
	assert((target == GL_READ_FRAMEBUFFER) ||
		(target == GL_DRAW_FRAMEBUFFER) ||
		(target == GL_FRAMEBUFFER));
	glBindFramebuffer(target, 0);
}



}

#endif
