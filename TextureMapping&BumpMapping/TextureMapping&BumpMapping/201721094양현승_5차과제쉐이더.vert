#version 410 core
//shader.vert:::vertex shader
layout(location=0) in vec3 in_Position; //in-Position: vertexarray에 붙여준 데이터, 0번에 붙여준 데이터, 3차원 좌표
layout(location=1/*vertexarray index*/) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoord;

uniform mat4 viewMat;
uniform mat4 projectionMat;
uniform mat4 modelMat;

out vec3 worldPosition;
out vec3 normal;
out vec2 texCoord;

void main(void)
{
    worldPosition = (modelMat * vec4(in_Position, 1)).xyz;
    normal = (modelMat * vec4(in_Normal,0)).xyz; //vec3인 in_Normal을 vec4 modelMat와 연산을 위해 homogeneous로 표현::augmented vector
    texCoord = in_TexCoord;
    gl_Position = projectionMat * viewMat * modelMat * vec4(in_Position, 1.0); //행렬연산
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

