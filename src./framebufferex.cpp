#include "frameBufferEX.h"
//framebuffer implementation
namespace tdogl{
	struct FrameBufferEX::TEXTURE_ATTACHMENT{
		const textureRefactor*texture;
		GLenum attachment, target;
		GLint level, layer;
		TEXTURE_ATTACHMENT():texture(0), attachment(GL_NONE), target(GL_NONE), level(0), layer(0){}
		TEXTURE_ATTACHMENT(
			const textureRefactor * _texture, GLenum _attachment, GLenum _target, GLint level_, GLint _layer):texture(_texture), attachment(_attachment), target(_target), level(level_), layer(_layer){
		}
	};
	//constructor
	FrameBufferEX::FrameBufferEX(GLenum target): handle_(GL_ZERO), status_(GL_FRAMEBUFFER_UNDEFINED), textureAttachments(0), nTextureAttachments_(0), drawBuffers_(0), nDrawBuffers_(0), maxDrawBuffers_(-1){

		size_[0] = size_[1] = 0;
		glGenFramebuffers(1, &handle_);
		bind(target);
		init(); //set up the internals
	}
	//constructor with height and height
	FrameBufferEX::FrameBufferEX(GLsizei width, GLsizei height, GLenum target): handle_(GL_ZERO), status_(GL_FRAMEBUFFER_UNDEFINED), textureAttachments(0), nTextureAttachments_(0), drawBuffers_(0), nDrawBuffers_(0){
	size_[0] = width;
	size_[1] = height;
	bind(target); //bind the target
	init();//set up the internals
	}
	//destructor
	FrameBufferEX::~FrameBufferEX(){
		if(textureAttachments)
			delete[] textureAttachments;
		if(drawBuffers_)
			delete[] drawBuffers_;

		glDeleteFramebuffers(1, &handle_);

	}
	const textureRefactor* FrameBufferEX::getAttachment(GLenum attachment)const{
		for(GLsizei i = 0; i<nTextureAttachments_; i++){
			if(textureAttachments[i].attachment==attachment){
				return textureAttachments[i].texture;
			}
		}
		return (textureRefactor*)0; //no match found
	}
	void FrameBufferEX::attach(const texture2DEX * texture, GLenum attachment, GLint level){
		assert(handle_ !=GL_ZERO);

		//bind the texture to the framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture->getTarget(), texture->getHandle(), level);
		addTextureAttachment(TEXTURE_ATTACHMENT(texture, attachment, texture->getTarget(), level, 0));
	}
		
	

	void FrameBufferEX::setStatus(){
		status_ = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	}
	void FrameBufferEX::init(){
		//get implementation independent limits
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxTexAttachments_);
		maxTexAttachments_ +=2; //add 2 for depth and stencil
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers_);
		textureAttachments = new(std::nothrow) TEXTURE_ATTACHMENT[maxTexAttachments_];
		assert(textureAttachments!= 0);
		drawBuffers_ = new(std::nothrow) GLenum[maxDrawBuffers_];
		assert(drawBuffers_ !=0);
	}


	void FrameBufferEX::addTextureAttachment(const TEXTURE_ATTACHMENT & texa){
		//check if attachment already exists
		GLsizei attachmentToSet = nTextureAttachments_;
		for(GLsizei i =0; i<nTextureAttachments_; i++){
			if(textureAttachments[i].attachment == texa.attachment){
				attachmentToSet = i;
				break;
			}

		}

		bool newAttach = false; //used to decide to call set draw buffers
		if(attachmentToSet ==nTextureAttachments_){
			newAttach = true;
			assert(nTextureAttachments_ <maxTexAttachments_);
			++nTextureAttachments_;
		}
		textureAttachments[attachmentToSet] = texa; ///actually set attachment

		if(newAttach){
			setDrawBuffers();
		}
		setSize();
		setStatus();

	}

	void FrameBufferEX::setDrawBuffers(){
		assert(textureAttachments !=0);
		assert(drawBuffers_ !=0);
		//get color attachment enums
		nDrawBuffers_  = 0;
		for(GLsizei i=0; i<nTextureAttachments_; i++){
			assert(nDrawBuffers_ <maxDrawBuffers_);
			switch(textureAttachments[i].attachment){
			case GL_DEPTH_ATTACHMENT:
			case GL_STENCIL_ATTACHMENT:
			case GL_DEPTH_STENCIL_ATTACHMENT:
				break;//no color buffer, dont do anything
			default:
				drawBuffers_[nDrawBuffers_] = textureAttachments[i].attachment;
				++nDrawBuffers_;
				break;
			};
		}
		glDrawBuffers(nDrawBuffers_, drawBuffers_);
	}

	void FrameBufferEX::setSize() {
		size_[0] = size_[1] = 0;
		for(GLsizei i =0; i<nTextureAttachments_; i++){
			if(textureAttachments[i].attachment ==GL_NONE)
				continue; //skip null attachements

			//set width and height as min of all attached texture sizes
			GLsizei texWidth = textureAttachments[i].texture->getWidth();
			GLsizei texHeight = textureAttachments[i].texture->getHeight();
			if(size_[0] == 0 || size_[0]>texWidth)
				size_[0] = texWidth;
			if(size_[1] == 0 || size_[1]>texHeight)
				size_[1] = texHeight;
		}
		texelSize_[0] = GLfloat(1) / GLfloat(size_[0]);
		texelSize_[1] = GLfloat(1) / GLfloat(size_[1]);
	}


}