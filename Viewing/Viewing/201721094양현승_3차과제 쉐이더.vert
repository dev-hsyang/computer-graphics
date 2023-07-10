#version 410 core

layout(location=0) in vec3 in_Position; //in-Position: vertexarray에 붙여준 데이터, 0번에 붙여준 데이터, 3차원 좌표
uniform mat4 viewMat;
uniform mat4 projectionMat;
uniform mat4 modelMat;
void main(void)
{
    gl_Position = projectionMat * viewMat*modelMat * vec4(in_Position, 1.0);
}







//원그리기 과제까지 shader.vert
/*
#version 410 core

layout(location=0) in vec3 in_Position;

void main(void)
{
    gl_Position= vec4(in_Position, 1.0);
}
*/

