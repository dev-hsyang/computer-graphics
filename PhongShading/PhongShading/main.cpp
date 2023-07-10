//
//  main.cpp
//  ComputerGraphics_OpenGL_Xcode
//
//  Created by 양현승 on 2021/09/13.
//
// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
#define GL_SILENCE_DEPRECATION
#include <Opengl/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <iostream>
#include "j3a.hpp"
#include "toys.h"
 
using namespace std;
using namespace glm;
const float PI = 3.14159265358979;
void render( GLFWwindow* window ); //그림을 그리는 함수
void init(); //그릴 data를 초기화하고, render에 보내주는 함수
Program program; //vertex데이터 받아와서 그림그리는 역할. 처리하는 역할
GLuint vertBuffer=0;
GLuint vertArray=0;
GLuint normBuffer=0;
GLuint tCordBuffer=0;
GLuint triBuf=0;
float theta = 0;
float phi = 0;
float fovy = 60;
float cameraDistance = 1;
double oldX, oldY;
vec3 cameraPosition = vec3(0, 0, 50);
vec3 lightPosition = vec3(5,5,50);
vec3 lightInt = vec3(1,1,1);
vec3 color = vec3(.2, .5, .3);
vec3 ambInt = vec3(.2, .2, .2);

void cursorCallback(GLFWwindow* window, double xpos, double ypos){
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)){
        theta -= ( xpos - oldX ) / width * PI;
        phi -= (ypos - oldY ) / height * PI;
    }
    oldX = xpos;
    oldY = ypos;
    if(theta<-(PI/2)) theta = -PI/2;
    //printf("%f\n", theta);
    if(theta>PI/2) theta = PI/2;
}
  
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    //cameraDistance = cameraDistance * pow( 1.01, -yoffset ); //dolly effect
    fovy = fovy * pow ( 1.01, -yoffset );
    if (fovy>170) fovy = 170;
    if (fovy<10) fovy = 10;
}

int main(void){
    if ( !glfwInit() )        exit(EXIT_FAILURE);
  
#ifdef __APPLE__

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#endif

    GLFWwindow* window = glfwCreateWindow( 800, 800 , "Computer Graphics - Phong Shading", NULL, NULL ); //(가로, 세로, 타이틀, , )
    glfwSetCursorPosCallback( window, cursorCallback );
    glfwSetScrollCallback( window, scrollCallback );
    glfwMakeContextCurrent( window );
    init();
    while ( !glfwWindowShouldClose( window ) ) {
        render( window );
        glfwPollEvents(); //window에서 발생하는 event들
    }
    glfwDestroyWindow( window );
    glfwTerminate();
}

void init(){
    program.loadShaders("/Users/hyunseung/포트폴리오/컴퓨터그래픽스/과제/4차과제/201721094양현승_4차과제 쉐이더.vert", "/Users/hyunseung/포트폴리오/컴퓨터그래픽스/과제/4차과제/201721094양현승_4차과제 쉐이더.frag");
    loadJ3A("/Users/hyunseung/XcodeProjects/ComputerGraphics/ComputerGraphics_OpenGL_Xcode/ComputerGraphics_OpenGL_Xcode/dragon.j3a");
    
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec3), vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &normBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec3), normals[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &tCordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tCordBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec2), texCoords[0], GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vertArray);
    glBindVertexArray(vertArray);
    glEnableVertexAttribArray(0); //0번 vertexarr
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
    
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, tCordBuffer);
    glVertexAttribPointer(2, 2/*vec정보*/, GL_FLOAT, 0, 0, 0);
    
    glGenBuffers(1, &triBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(u32vec3), triangles[0], GL_STATIC_DRAW);
}

void render( GLFWwindow* window ){
         
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    vec3 cameraPos = vec3( rotate( theta, vec3( 0, 1, 0 ) ) * rotate ( phi, vec3(1, 0, 0) ) * vec4( cameraPosition, 1 ) ); //rotate( 각도, 회전축 )
    
    mat4 viewMat = lookAt( cameraPos, vec3(0, 0, 0), vec3(0, 1, 0) );
    mat4 projectionMat = perspective( fovy/PI/180, width/(float)height, 0.1f, 100.f );
    mat4 modelMat = mat4(1); //rotate(theta,vec3(0,1,0));
    
    glViewport(0, 0, width, height); //Viewport: 그림을 그릴 영역을 정함, (0,0,w,h): 0.0부터 w.h까지 그린다
    glClearColor(0, 0, 0, 0);//(R,G,B,불투명도 a)
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    glUseProgram(program.programID);
    GLuint loc;
    loc = glGetUniformLocation(program.programID, "viewMat");
    glUniformMatrix4fv(loc, 1, false, value_ptr(viewMat));
    loc = glGetUniformLocation(program.programID, "projectionMat");
    glUniformMatrix4fv(loc, 1, false, value_ptr(projectionMat));
    loc = glGetUniformLocation(program.programID, "modelMat");
    glUniformMatrix4fv(loc, 1, false, value_ptr(modelMat));
    loc = glGetUniformLocation(program.programID, "lightPosition");
    glUniform3fv(loc, 1, value_ptr(lightPosition));
    loc = glGetUniformLocation(program.programID, "lightInt");
    glUniform3fv(loc, 1, value_ptr(lightInt));
    loc = glGetUniformLocation(program.programID, "ambInt");
    glUniform3fv(loc, 1, value_ptr(ambInt));
    loc = glGetUniformLocation(program.programID, "color");
    glUniform3fv(loc, 1, value_ptr(color));
    loc = glGetUniformLocation(program.programID, "cameraPosition");
    glUniform3fv(loc, 1, value_ptr(cameraPos));
    
    glBindVertexArray(vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
}
   
