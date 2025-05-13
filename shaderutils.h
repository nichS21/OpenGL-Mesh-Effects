
#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <string>

#include <GL/glew.h>


GLuint loadProgram( const std::string& vertShaderFile,
		    const std::string& fragShaderFile );

GLuint loadShader( const std::string& fileName, GLenum shaderType );


#endif
