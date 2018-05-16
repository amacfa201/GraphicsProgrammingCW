#version 330 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;


// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
  
} vs_out;

uniform mat4 transform;

void main()
{
    vs_out.FragPos = VertexPosition;
    vs_out.Normal = VertexNormal;
    //gl_Position = projection * view * vec4(VertexPosition, 1.0);

	gl_Position = transform * vec4(VertexPosition,1.0);
}