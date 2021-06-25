#shader vertex
#version 330 core

layout(location = 0) in vec3 ve;
layout(location = 1) in vec3 no;
varying vec3 N;
varying vec4 v;
uniform mat4 MVP;

void main(void)
{
   v = MVP * vec4(ve, 1.0f);
   N = normalize((MVP * vec4(no,1.0f)).xyz);
   gl_Position = MVP * vec4(ve, 1.0f);
}      

#shader fragment
#version 330 core

varying vec3 N;
varying vec4 v;
layout(location = 0) out vec4 diffuseColor;

void main(void)
{
   vec3 torusColor = vec3(0.9, 0.6, 0.9);
   vec3 lightPos = vec3(2.0f,0.0f,2.0f);
   vec3 L = normalize(lightPos - v.xyz);
   vec4 Idiff = vec4(torusColor * max(dot(N,L), 0.0f), 0.0f);
   Idiff = clamp(Idiff, 0.0, 1.0);
   vec4 Iambient = vec4(torusColor * vec3(0.23, 0.5, 0.4),1.0);

   diffuseColor = Idiff + Iambient;
}
