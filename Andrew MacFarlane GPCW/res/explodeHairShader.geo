#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;
uniform float time;

vec4 GenExplosion(vec4 position, vec3 normal)
{
	float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0); 
}

void GenerateLine(int index)
{
    gl_Position = GenExplosion(gl_in[0].gl_Position, gs_in[index].normal);
    EmitVertex();
    gl_Position = GenExplosion(gl_in[0].gl_Position, gs_in[index].normal) + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}  