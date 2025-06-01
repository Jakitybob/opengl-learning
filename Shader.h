#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h> // include glad to get the required opengl headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	// The Shader Program ID
	unsigned int ID;

	// Constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use / activate the shader
	void Use();
	
	// Utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
};


#endif
