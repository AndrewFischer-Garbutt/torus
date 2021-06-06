#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define RESOLUTION 10

void load_texture(const std::string& path)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        assert(false);
    }
    stbi_image_free(data);
}

void set_up_window(GLFWwindow*& window, int width, int hight, const std::string& title)
{
    /* Initialize the library */
    if (!glfwInit())
        assert(false);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, hight, "Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        assert(false);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
}

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

static unsigned int CompileShader(unsigned int type, const std::string& source)
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

static unsigned int CreateShaders(const std::string& vertexShader, const std::string& fragmentShader)
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

int main(void)
{
    GLFWwindow* window;
    set_up_window(window, 1600, 800, "Window");


    if(glewInit() != GLEW_OK)
    {
	    std::cout << "Error!" << std::endl;
    }

    float positions[3*RESOLUTION*RESOLUTION];

    float a = 0.25;
    float c = 1.0;

    for(int i = 0; i < RESOLUTION; i++)
    {
	    for(int j = 0; j < RESOLUTION; j++)
	    {
		float x = (c + a*cos(float(j)*2.0*M_PI / RESOLUTION))*cos(float(i)*2*M_PI/RESOLUTION);
		float y = (c + a*cos(float(j)*2.0*M_PI / RESOLUTION))*sin(float(i)*2*M_PI/RESOLUTION);
		float z = a*sin(float(i)*2*M_PI/RESOLUTION);
		std::cout << x << " " << y << " " << z << std::endl;
		positions[3*RESOLUTION*i + 3*j] = x;
		positions[3*RESOLUTION*i + 3*j + 1] = y;
		positions[3*RESOLUTION*i + 3*j + 2] = z;
	    }
    }

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    std::vector<unsigned int> indices;

    // fill "indices" as needed
    for(int i = 0; i < RESOLUTION; i++)
    {
	    for(int j = 0; j < RESOLUTION; j++)
	    {
		    indices.push_back(i + RESOLUTION * j);
		    indices.push_back(((i + 1) % RESOLUTION) + RESOLUTION * j);
		    indices.push_back(i + RESOLUTION * ((j + 1) % RESOLUTION));

		    indices.push_back(i + RESOLUTION * ((j + 1) % RESOLUTION));
		    indices.push_back(((i + 1) % RESOLUTION) + RESOLUTION * ((j + 1) % RESOLUTION));
		    indices.push_back(((i + 1) % RESOLUTION) + RESOLUTION * j);
	    }
    }


    // Generate a buffer for the indices
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    /* 
     * vertexShader and fragmentShader 
     * are variables holding the
     * shader code.
     */

    std::vector<std::string> shaders;
    parseShaders("basic.shader", shaders);

    int shader = CreateShaders(shaders[0], shaders[1]);
    glUseProgram(shader);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, .1f, 100.0f);

    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    glm::mat4 View = glm::lookAt(
		    glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
		    glm::vec3(0,0,0), // and looks at the origin
		    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(shader, "MVP");
    

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
	    /* Render here */
	    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	    // Send our transformation to the currently bound shader, in the "MVP" uniform
	    // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]); 

	    glDrawElements(
			    GL_TRIANGLES,      // mode
			    indices.size(),    // count
			    GL_UNSIGNED_INT,   // type
			    (void*)0           // element array buffer offset
			  );

	    /* Swap front and back buffers */
	    glfwSwapBuffers(window);

	    /* Poll for and process events */
	    glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
