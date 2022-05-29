/*
VERTEX SHADER
*/
#begin vertex
#version 330 core

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 VertexNormal;


out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 uView;
uniform mat4 uModel;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(uView * uModel)));
    vs_out.normal = vec3(vec4(normalMatrix * VertexNormal, 0.0));
    gl_Position = uView * uModel * vec4(VertexPos, 1.0);
}

    #end vertex
    /*
    GEOMETRIC SHADER
    */
    #begin geometric
    #version 330 core


layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform mat4 uProjection;

void GenerateLine(int index)
{
    gl_Position = uProjection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = uProjection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * 0.2);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0);// first vertex normal
    GenerateLine(1);// second vertex normal
    GenerateLine(2);// third vertex normal
}

    #end geometric
    /*
    FRAGMENT SHADER
    */
    #begin fragment
    #version 330 core

out vec4 outColor;

void main()
{
    outColor = vec4(1.0, 1.0, 0.0, 1.0);
}


    #end fragment