#version 150 core
//shader.frag:::fragment shader
out vec4 out_Color;

in vec3 worldPosition;
in vec3 normal;
in vec2 texCoord;

uniform vec3 lightPosition;
uniform vec3 lightInt;
uniform vec3 ambInt;
uniform vec3 color;
uniform vec3 cameraPosition;

void main(void){
    vec3 L = normalize(lightPosition-worldPosition);
    vec3 N = normalize(normal);
    vec3 R = 2  * dot(N, L) * N - L;
    vec3 V = normalize (cameraPosition - worldPosition);
    
    vec3 diffuse = lightInt * max(0,dot(N, L)) * color;
    vec3 ambient = ambInt * color;
    vec3 specular = lightInt * pow(max(0, dot(R, V)), 20);
    out_Color = vec4(diffuse + ambient + specular, 1.0);
}
