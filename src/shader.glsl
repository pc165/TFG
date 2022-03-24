#begin vertex
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
void main(){
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
}
#end vertex

#begin fragment
#version 330 core
out vec4 color;
uniform vec4 uColor;
void main(){
    color = uColor;
}
#end fragment
