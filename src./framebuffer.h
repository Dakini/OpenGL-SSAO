#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "opengl.h"
#include "frameBufferEX.h"

//frame buffer class
class frameBuffer2D: public tdogl::FrameBufferEX{

public:
	//create framebuffer intial one and one with width and height
	static frameBuffer2D* create(const string &name ="");
	 static frameBuffer2D* create(int width, int height, const string &name ="");
	 //destroy frame buffer
	 void destroy(frameBuffer2D** ppFrameBuffer);
private:
	//private contrsuctors
	frameBuffer2D(const string &name);
	frameBuffer2D(int width, int height, const string &name);

};

frameBuffer2D::frameBuffer2D(int width, int height, const string &name):tdogl::FrameBufferEX(width, height){
}
frameBuffer2D::frameBuffer2D(const string &name): tdogl::FrameBufferEX(){
}
//create blank frame buffer
frameBuffer2D* frameBuffer2D::create( const string &name){
	frameBuffer2D * result = new(std::nothrow) frameBuffer2D(name);
	assert(result);
	return result;
}
//create a frame buffer and height
frameBuffer2D* frameBuffer2D::create(int width, int height, const string &name){
	frameBuffer2D * result = new(std::nothrow) frameBuffer2D(width, height,name);
	assert(result);
	return result;
}
//destor of framebuffer
void frameBuffer2D::destroy(frameBuffer2D**ppBuffer){
	assert(ppBuffer !=0);
	assert(*ppBuffer !=0);
	delete *ppBuffer;
	*ppBuffer =0;
}
#endif