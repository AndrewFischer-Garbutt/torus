#include "shaders.h"

void parseShaders(const std::string& file, std::vector<std::string>& shaders)
{
	std::fstream fs; fs.open("basic.shader", std::ios::in); 
	std::string line;
	std::stringstream ss;
	bool start = false;

	while(getline(fs, line))
	{
		if(line.find("#shader") == line.npos)
		{
			ss << line << std::endl;
		}
		else
		{
			if(start)
			{
				shaders.push_back(ss.str());
				ss.str(std::string());
			}
			else
				start = true;
		}
	}
	shaders.push_back(ss.str());
}

unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(sizeof(char)*length);
		glGetShaderInfoLog(id, length, &length, message);
		if (type == GL_VERTEX_SHADER)
		{
			std::cout << "Failed to compile vertex shader." << std::endl;
		}
		if(type == GL_FRAGMENT_SHADER)
		{
			std::cout << "Failed to compile fragment shader." << std::endl;
		}
		std::cout << message << std::endl;
		glDeleteShader(id);
	}

	return id;
}

unsigned int CreateShaders(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int programID = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(programID, vs);
	glAttachShader(programID, fs);

	glLinkProgram(programID);

	glValidateProgram(programID);

	return programID;
}
