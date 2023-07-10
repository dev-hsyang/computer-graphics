//shadow.vert
#version 410 core
layout(location=0) in vec3 in_Position;

uniform mat4 shadowMVP;

void main(){
    gl_Position = shadowMVP * vec4( in_Position, 1 );
}
