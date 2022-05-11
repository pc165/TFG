#begin vertex
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out vec3 fragmentColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main(){
    gl_Position =  projection * view * model * vec4(pos, 1);
    fragmentColor = normal;
}
#end vertex

#begin fragment
#version 330 core

in vec3 fragmentColor;
out vec3 color;


uniform vec3 colorIn;
void main(){
    color = colorIn;
}
#end fragment