#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
uniform float Z;

uniform mat4 MVP;
void main()
{
	gl_Position = MVP * vec4(position.xyz, 1.0);
}

#shader fragment
#version 330 core

out vec4 color;

// Values that stay constant for the whole mesh.
//uniform sampler2D textureSampler;

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
};
