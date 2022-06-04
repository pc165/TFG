#begin vertex
#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoords;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 fragTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main(){
    gl_Position =  uProjection * uView * uModel * vec4(aPosition, 1);
    fragNormal = mat3(transpose(inverse(uModel))) * aNormal;
    fragPos = aPosition;
    fragTexCoord = aTextureCoords;
}
#end vertex

#begin fragment
#version 330 core


struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoord;

out vec4 outColor;

uniform vec3 uColor;
uniform vec3 uViewPos;
uniform bool uIsLightEnabled;

uniform Light uLight;
uniform Material uMaterial;

void main(){

    if (uIsLightEnabled){
        outColor = vec4(uColor, 1.0);
        return;
    }

    // ambient
    vec3 ambient = uLight.ambient * uColor;// texture(uMaterial.diffuse, fragTexCoord).rgb

    // diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(uLight.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = uLight.diffuse * diff * uColor;// texture(uMaterial.diffuse, fragTexCoord).rgb

    // specular
    vec3 viewDir = normalize(uViewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    vec3 specular = uLight.specular * spec * uColor;// texture(uMaterial.specular, fragTexCoord).rgb

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-uLight.direction));
    float epsilon = (uLight.cutOff - uLight.outerCutOff);
    float intensity = clamp((theta - uLight.outerCutOff) / epsilon, 0.0, 1.0);
//    float intensity = smoothstep(uLight.cutOff, uLight.outerCutOff, theta);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(uLight.position - fragPos);
    float attenuation = 1.0 / (uLight.constant + uLight.linear * distance + uLight.quadratic * (distance * distance));
    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;

    outColor = vec4(ambient + diffuse + specular, 1.0);
    //    outColor = vec4(fragTexCoord, 0.0, 0.0);
}
#end fragment