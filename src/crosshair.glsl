#begin vertex
#version 330 core
layout(location = 0) in vec2 pos;

uniform mat4 model;
uniform mat4 projection;

void main(){
    gl_Position = projection * model * vec4(pos, 0, 1);
}
#end vertex

#begin fragment
#version 330 core

out vec3 color;
uniform vec3 colorIn;

void main(){
    color = colorIn;
}
#end fragment