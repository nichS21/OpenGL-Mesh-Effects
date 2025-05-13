
#include "shaderutils.h"

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include <cstdlib>

#include <GL/glew.h>


void showShaderErrorLog( GLint shader, const std::string& fileName );
void showProgramErrorLog( GLint program );


GLuint loadProgram( const std::string& vertShaderFile,
		    const std::string& fragShaderFile )
{
  // create vertex shader object
  GLuint vertShader = loadShader( vertShaderFile, GL_VERTEX_SHADER );
  
  // create fragment shader object
  GLuint fragShader = loadShader( fragShaderFile, GL_FRAGMENT_SHADER );
  
  // create shader program with the shaders
  GLuint program = glCreateProgram();
  glAttachShader( program, vertShader );
  glAttachShader( program, fragShader );

  // link program and show error log if did not succeed
  glLinkProgram( program );

  showProgramErrorLog( program );

  return program;
}


GLuint loadShader( const std::string& fileName, GLenum shaderType )
{
  // initialize input stream
  std::ifstream inFile( fileName.c_str(), std::ios::binary );

  // determine shader file length and reserve space to read it in
  inFile.seekg( 0, std::ios::end );
  int fileLength = inFile.tellg();
  std::vector<char> fileContent( fileLength + 1 );
	
  // read in shader file, set last character to NULL, close input stream
  inFile.seekg( 0, std::ios::beg );
  inFile.read( fileContent.data(), fileLength );
  fileContent[fileLength] = '\0';
  inFile.close();

  // create the shader from the source in the given file
  GLuint shader = glCreateShader( shaderType );
  const char* fileContentRaw = fileContent.data();
  glShaderSource( shader, 1, &fileContentRaw, NULL );

  // compile the shader and show error log if compilation failed
  glCompileShader( shader );
  showShaderErrorLog( shader, fileName );

  return shader;
}

void showShaderErrorLog( GLint shader, const std::string& fileName )
{
  GLint isCompiled = 0;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled );

  if ( isCompiled == GL_TRUE ) {
    return;
  }

  GLint logLength = 0;
  glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
  
  std::vector<GLchar> logText( logLength );
  glGetShaderInfoLog( shader, logLength, &logLength, logText.data() );
  
  std::cout << "\nSHADER COMPILE ERROR for " << fileName << "\n--" << std::endl;
  std::cout << logText.data() << std::endl;
  
  glDeleteShader( shader );
  
  exit(-1);
}


void showProgramErrorLog( GLint program )
{
  GLint isLinked = 0;
  glGetProgramiv( program, GL_LINK_STATUS, &isLinked );
  
  if ( isLinked == GL_TRUE ) {
    return;
  }
  
  GLint logLength = 0;
  glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
  
  std::vector<GLchar> logText( logLength );
  glGetProgramInfoLog( program, logLength, &logLength, logText.data() );
  
  std::cout << "\nPROGRAM LINK ERROR\n--" << std::endl;
  std::cout << logText.data() << std::endl;
  
  glDeleteShader( program );
  
  exit( -1 );
}
