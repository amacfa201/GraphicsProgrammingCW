#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;

}vs_out;


uniform mat4 transform;



void main()
{

	vs_out.FragPos = VertexPosition;
	vs_out.Normal = VertexNormal;
	gl_Position = transform * vec4(VertexPosition, 1.0);

	//v_norm = VertexNormal;
	//v_pos = vec3(VertexPosition);
	//gl_Position = transform * vec4(VertexPosition, 1.0);
}