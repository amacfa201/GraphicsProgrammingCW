#version 330 core
layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;


// declare an interface block; see 'Advanced GLSL' for what these are.


uniform mat4 transform;

out vec3 v_norm;
out vec3 v_pos;

void main()
{
   v_norm = VertexPosition;
   v_pos  = VertexNormal;
    //gl_Position = projection * view * vec4(VertexPosition, 1.0);

	gl_Position = transform * vec4(VertexPosition,1.0);
}