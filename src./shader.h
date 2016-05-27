#ifndef SHADER_H
#define SHADER_H

#pragma once
#include "opengl.h"

//using namespaxce tdogl
namespace tdogl{

	//shader class
	class shader{
		
	public:
		//createa a shader from a file (txt or glsl etc)
		//need file path, and shader type
		//throw exception if an error happens 
		static shader shaderFromFile(const string& filepath, GLenum shaderType);
		//create a shader from a string of shader source code
		//the sourcecode of the shader and shader type
		shader(const string& filepath, GLenum shaderType);
		//the result of the object ID returned as GLcreateShader
		GLuint object() const;
		//tdogl::shader objects can be copied and assigned becasue they are referece like a shared pointed
		shader(const shader& other);
		shader& operator =(const shader& other);
		~shader();
	private:
		GLuint _object;
		unsigned *_refCount;

		void _release();
		void _retain();
	};

}


#endif