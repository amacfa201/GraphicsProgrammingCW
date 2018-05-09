#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
varying vec2 texCoord0;
varying vec3 normal0;

uniform mat4 transform;

out vec3 v_norm;
out vec4 v_pos; 

void main()
{
    gl_Position = transform * vec4(VertexPosition, 1.0);
	v_norm = VertexNormal;
	v_pos = vec4(VertexPosition, 1.0);
	texCoord0 = texCoord;
	normal0 = (transform * vec4(normal, 0.0)).xyz;
	
}
