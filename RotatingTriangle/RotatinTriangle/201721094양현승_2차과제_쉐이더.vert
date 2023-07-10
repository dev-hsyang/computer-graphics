#version 410 core

layout(location=0) in vec3 in_Position; //in-Position: vertexarray에 붙여준 데이터, 0번에 붙여준 데이터, 3차원 좌표
uniform mat3 transf = mat3(1,0,0, 0,1,0, 0,0,1);
void main(void)
{
    vec3 p = transf * vec3(in_Position.xy, 1);
	gl_Position= vec4(p.xy, 0, 1);
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

