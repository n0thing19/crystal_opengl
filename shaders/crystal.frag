#version 330 core
in vec3 vNormal;
in vec3 vWorldPos;

out vec4 FragColor;

uniform vec3 camPos;
uniform vec3 lightDir;    // normalized
uniform vec3 lightColor;
uniform vec3 baseColor;
uniform float ambientK;   // 0..1

void main(){
    vec3 N = normalize(vNormal);
    vec3 L = normalize(-lightDir);
    vec3 V = normalize(camPos - vWorldPos);
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 32.0);

    vec3 color = baseColor * (ambientK + diff) + lightColor * (0.25 * spec);
    FragColor = vec4(color, 1.0);
}
