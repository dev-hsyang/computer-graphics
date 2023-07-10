//
//  main.cpp
//  ComputerGraphics_OpenGL_Xcode
//
//  Created by 양현승 on 2021/09/13.
//
// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#define GLEW_STATIC
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "toys.h"
#define PI 3.141592
//float blue=0; 1차과제
using namespace std;
using namespace glm;
void render( GLFWwindow* window ); //그림을 그리는 함수
void init(); //그릴 data를 초기화하고, render에 보내주는 함수
Program program; //vertex데이터 받아와서 그림그리는 역할. 처리하는 역할
GLuint vertBuf=0;
GLuint vertArray=0;
GLuint circleBuf=0;
float radius=0.8;
int count_t=0;

int main(void){
    if ( !glfwInit() )        exit(EXIT_FAILURE);
  
#ifdef __APPLE__

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#endif

    GLFWwindow* window = glfwCreateWindow( 640,480 , "Computer Graphics", NULL, NULL ); //(가로, 세로, 타이틀, , )
    glfwMakeContextCurrent( window );
    //glfwSwapInterval(0);
    init();
    while ( !glfwWindowShouldClose( window ) ) {
        render( window );
        glfwPollEvents(); //window에서 발생하는 event들
    }
    glfwDestroyWindow( window );
    glfwTerminate();
}

void init(){ //그릴 data를 초기화하고, render에 보내주는 함수
    program.loadShaders("/Users/hyunseung/포트폴리오/컴퓨터그래픽스/과제/1차과제/shader.vert", "/Users/hyunseung/포트폴리오/컴퓨터그래픽스/과제/1차과제/shader.frag");//program초기화.
    vector<glm::vec3> vert; //vec3를 원소로 갖는 vector배열 vert
    vector<glm::u32vec3> triangles;
    for(float theta=0; theta<2*PI; theta+=0.1){
        vert.push_back(vec3(radius*cos(theta), radius*sin(theta), 0));
        count_t++;
    }
    for(int i=0; i<count_t;i++){
        triangles.push_back(u32vec3(0,i,i+1));
    }
    triangles.push_back(u32vec3(0,1,count_t));
    /*
    for(float i=0; i<=360; i++){
        float theta=(i/180)*PI;
        float x = (float)radius*cos(theta);
        float y = (float)radius*sin(theta);
        vert.push_back(vec3(x, y, 0));
        triangles.push_back(u32vec3(0,i,i+1));
    }
     */
    glGenBuffers(1, &vertBuf); //버퍼생성 (버퍼 크기, 데이터받아올 인덱스)
    glBindBuffer(GL_ARRAY_BUFFER, vertBuf); //바인드할곳, 바인드할 버퍼
    glBufferData(GL_ARRAY_BUFFER, vert.size()*sizeof(glm::vec3), vert.data(), GL_STATIC_DRAW);//data를 보낸다(보낼곳,보낼사이즈(바이트 단위), 보낼데이터, 데이터가 안바뀔것을 알리는 코드)
    
    glGenVertexArrays(1, &vertArray); //vertexarray생성 (개수, 데이터받아올 곳)
    glBindVertexArray(vertArray);
    glEnableVertexAttribArray(0); //vertexarray의 몇번째 자리에 data가 들어갈 것인가
    glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0); //vertexarray에 data 연결 (연결할 array의 자리, data하나의 단위의 size, data형태, 0, 0,0)
    glGenBuffers(1, &circleBuf); //버퍼 개수, 이름
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleBuf); //바인드할곳, 바인드할 버퍼
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size()*sizeof(glm::u32vec3), triangles.data(), GL_STATIC_DRAW);
}

void render( GLFWwindow* window ){
         
    int width, height;
    
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height); //Viewport: 그림을 그릴 영역을 정함, (0,0,w,h): 0.0부터 w.h까지 그린다
    glClearColor(0, 0, 0.7, 0);//(R,G,B,불투명도 a)
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program.programID);
    glBindVertexArray(vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleBuf);
    
    glDrawElements(GL_TRIANGLES, count_t*3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
}



//1차과제
/*void render( GLFWwindow* window ){
        
    int width, height;
    
    if(blue>=1.00){
        blue=0;
    }
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height); //Viewport: 그림을 그릴 영역을 정함, (0,0,w,h): 0.0부터 w.h까지 그린다
    glClearColor(0, 0, blue+=0.05, 0);//(R,G,B,불투명도 a)
    printf("blue: %f", blue);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    
}
*/
// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.




