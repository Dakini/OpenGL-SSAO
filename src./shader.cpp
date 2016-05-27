#include "shader.h"

#include <stdexcept>
#include <cassert>
#include <string>
#include <sstream>
using namespace tdogl;
shader::shader(const string& shaderCode, GLenum shaderType):_object(0), _refCount(NULL){
	//create the shader object
    _object = glCreateShader(shaderType);
    if(_object == 0)
        throw std::runtime_error("glCreateShader failed");
    
    //set the source code
    const char* code = shaderCode.c_str();
    glShaderSource(_object, 1, (const GLchar**)&code, NULL);
    
    //compile
    glCompileShader(_object);
    
    //throw exception if compile error occurred
    GLint status;
    glGetShaderiv(_object, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in shader:\n");
        
        GLint infoLogLength;
        glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(_object, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;

		cout<<msg<<endl;
        delete[] strInfoLog;
        
        glDeleteShader(_object); _object = 0;
        throw std::runtime_error(msg);
    }
    
    _refCount = new unsigned;
    *_refCount = 1;

}

shader::shader(const shader& other):_object(other._object), _refCount(other._refCount){
	_retain();
}
shader::~shader(){
	//refcount will be NULL if constructor faield and threw and exception
}
GLuint shader::object() const{
	return _object;
}
shader& shader::operator=(const shader& other){
	_release();
	_object = other._object;
	_refCount = other._refCount;
	_retain();
	return *this;
}

void shader::_retain(){
	assert(_refCount);
	*_refCount+=1;
}

void shader::_release(){
	assert(_refCount && *_refCount>0);
	*_refCount -=1;
	if(*_refCount==0){
		glDeleteShader(_object); _object = 0;
		delete _refCount; _refCount=NULL;
	}
}

shader shader::shaderFromFile(const string& filepath, GLenum shaderType){

	FILE* file;
	long fileSize = -1;
	char* glslSource;
	const char *fp = filepath.c_str();
	if(NULL != (file = fopen(fp, "rb")) && 0 == fseek(file, 0, SEEK_END) &&     -1 != (fileSize = ftell(file))){
		rewind(file);
		if(file==NULL)
			printf("error");
	}if (NULL!=(glslSource = (char*)malloc(fileSize+1))){
		if(fileSize==(long)fread(glslSource,sizeof(char),fileSize, file))
		{
			glslSource[fileSize]='\0';
		}
	}
	fclose(file);
	const string meow = glslSource;
	cout<<glslSource<<endl;
	shader shader2(meow, shaderType);
	return shader2;
}