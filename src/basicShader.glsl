#begin vertex
#version 460 core
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec3 VertexNormal;

out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main(){
    gl_Position =  uProjection * uView * uModel * vec4(VertexPos, 1);
    fragNormal = VertexNormal;
    fragPos = VertexPos;
}
#end vertex

#begin fragment
#version 460 core

in vec3 fragNormal;
in vec3 fragPos;

out vec4 outColor;

uniform vec3 uColor;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform bool uIsPicking;

void main(){

    if(uIsPicking){
        outColor = vec4(uColor,1.0);
        return;
    }

    // ambient
    vec3 ambient = 0.05 * uColor;

    // diffuse
    vec3 lightDir = normalize(uLightPos - fragPos);
    vec3 normal = normalize(fragNormal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * uColor;

    // specular
    vec3 viewDir = normalize(uViewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);

    vec3 specular = vec3(0.3) * spec;
    outColor = vec4(ambient + diffuse + specular, 1.0);
//    outColor = vec4(fragNormal,1.0);
}
#end fragment