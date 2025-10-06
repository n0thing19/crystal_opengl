#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 camMatrix; // projection * view dari Camera::Matrix()

out vec3 vNormal;
out vec3 vWorldPos;

uniform mat3 normalMat;

void main(){
    vec4 world = model * vec4(aPos, 1.0);
    vWorldPos = world.xyz;
    vNormal   = normalize(normalMat * aNormal);
    gl_Position = camMatrix * world;
}
