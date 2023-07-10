//
//  main.cpp
//  ComputerGraphics_OpenGL_Xcode
//
//  Created by 양현승 on 2021/09/13.
//
// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <Opengl/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "toys.h"
#define PI 3.141592
using namespace std;
using namespace glm;

void render( GLFWwindow* window ); //그림을 그리는 함수
void init(); //그릴 data를 초기화하고, render에 보내주는 함수
Program program; //vertex데이터 받아와서 그림그리는 역할. 처리하는 역할
GLuint vertBuf=0;
GLuint vertArray=0;
GLuint circleBuf=0;
GLuint triBuf=0;
float theta=0;
float radian=0;

int main(void){
    if ( !glfwInit() )        exit(EXIT_FAILURE);
  
#ifdef __APPLE__

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#endif

    GLFWwindow* window = glfwCreateWindow( 800, 800 , "201721094양현승", NULL, NULL ); //(가로, 세로, 타이틀, , )
    glfwMakeContextCurrent( window );
    init();
    while ( !glfwWindowShouldClose( window ) ) {
        theta +=0.08;
        if(theta>360) theta = 0;
        render( window );
        glfwPollEvents(); //window에서 발생하는 event들
    }
    glfwDestroyWindow( window );
    glfwTerminate();
}

void init(){
    program.loadShaders("/Users/hyunseung/포트폴리오/컴퓨터그래픽스/과제/2차과제/201721094양현승_2차과제_쉐이더.vert", "/Users/hyunseung/포트폴리오/컴퓨터그래픽스/과제/1차과제/shader.frag");
    vector<vec3> vert;
    vert.push_back(vec3(0,0.7,0));
    vert.push_back(vec3(-0.7,-0.7,0));
    vert.push_back(vec3(0.7,-0.7,0));
    
    vector<u32vec3> triangles;
    triangles.push_back(u32vec3(0,1,2));
    
    glGenBuffers(1, &vertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
    glBufferData(GL_ARRAY_BUFFER, vert.size()*sizeof(vec3), vert.data(), GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vertArray);
    glBindVertexArray(vertArray);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    
    glGenBuffers(1, &triBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size()*sizeof(u32vec3), triangles.data(), GL_STATIC_DRAW);
}

void render( GLFWwindow* window ){
         
    int width, height;
    GLuint location;
    
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height); //Viewport: 그림을 그릴 영역을 정함, (0,0,w,h): 0.0부터 w.h까지 그린다
    glClearColor(0, 0, 0, 0);//(R,G,B,불투명도 a)
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program.programID);
    
    mat3 transf = mat3(cos(theta),sin(theta),0, -sin(theta),cos(theta),0, 0,0,1);
    location = glGetUniformLocation(program.programID, "transf");
    glUniformMatrix3fv(location, 1, false, value_ptr(transf));
    glBindVertexArray(vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
}




