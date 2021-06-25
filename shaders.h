#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include <GL/glew.h>

void parseShaders(const std::string& file, std::vector<std::string>& shaders);
unsigned int CompileShader(unsigned int type, const std::string& source);
unsigned int CreateShaders(const std::string& vertexShader, const std::string& fragmentShader);