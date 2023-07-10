//
//  main.cpp
//  ComputerGraphics_OpenGL_Xcode
//
//  Created by 양현승 on 2021/09/13.
//
// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
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
#include "stb_image.h"
 
Program program;//vertex데이터 받아와서 그림그리는 역할. 처리하는 역할
Program shadowProgram;

void render( GLFWwindow* window ); //그림을 그리는 함수
void init();//그릴 data를 초기화하고, render에 보내주는 함수
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

using namespace std;
using namespace glm;

GLuint vertBuffer=0;
GLuint vertArray=0;
GLuint normBuffer=0;
GLuint tCordBuffer=0;
GLuint triBuf=0;
GLuint textureID=0;
GLuint bumpID=0;
GLuint shadowTex=0;
GLuint shadowDepth=0;
GLuint shadowFBO=0;

const float PI = 3.14159265358979;
const int shadowMapSize = 4096;
float theta = 0;
float phi = 0;
float fovy = 60;
float cameraDistance = 10;
float shin = 30;
double oldX, oldY;

vec3 cameraPosition = vec3(0, 0, 50);
vec3 lightPosition = vec3(5,5,5);
vec3 lightInt = vec3(1,1,1);
vec3 color = vec3(.2, .5, .3);
vec3 ambInt = vec3(.2, .2, .2);


int main(void){
    if ( !glfwInit() )        exit(EXIT_FAILURE);
  
#ifdef __APPLE__

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#endif

    GLFWwindow* window = glfwCreateWindow( 1000, 1000 , "Computer Graphics - Shadow Mapping", NULL, NULL ); //(가로, 세로, 타이틀, , )
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
    int w, h, n;
    
    glGenTextures(1, &shadowTex);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB32F, shadowMapSize, shadowMapSize, 0,GL_RGB, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glGenTextures(1, &shadowDepth);
    glBindTexture(GL_TEXTURE_2D, shadowDepth);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32F, shadowMapSize, shadowMapSize, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadowTex, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowDepth, 0);
    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("FBO Error\n");
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    //절대경로로 주어야 shader가 불러와집니다
    program.loadShaders("/Users/hyunseung/XcodeProjects/ComputerGraphics/shader.vert", "/Users/hyunseung/XcodeProjects/ComputerGraphics/shader.frag");
    shadowProgram.loadShaders("/Users/hyunseung/XcodeProjects/ComputerGraphics/shadow.vert", "/Users/hyunseung/XcodeProjects/ComputerGraphics/shadow.frag");

    loadJ3A("/Users/hyunseung/XcodeProjects/ComputerGraphics/ComputerGraphics_OpenGL_Xcode/ComputerGraphics_OpenGL_Xcode/dwarf.j3a");
    stbi_set_flip_vertically_on_load(true); //texture map을 뒤집기 위해 TRUE하여 킨다
    void* buffer = stbi_load(("/Users/hyunseung/XcodeProjects/ComputerGraphics/ComputerGraphics_OpenGL_Xcode/ComputerGraphics_OpenGL_Xcode/" + diffuseMap[0]).c_str(), &w, &h, &n, 4); //texture map image 읽고
   
    //buffer에 load, texture 생성, 속성 지정 후 upload
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D); //aliasing 문제 해결위해 mipmap사용
    stbi_image_free(buffer); //buffer free
    
    
    buffer = stbi_load(("/Users/hyunseung/XcodeProjects/ComputerGraphics/ComputerGraphics_OpenGL_Xcode/ComputerGraphics_OpenGL_Xcode/" + bumpMap[0]).c_str(), &w, &h, &n, 4); //bump map image 읽고
    //buffer에 load, texture 생성, 속성 지정 후 upload
    glGenTextures(1, &bumpID);
    glBindTexture(GL_TEXTURE_2D, bumpID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(buffer);
    
    
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
    //glEnable(GL_DEPTH_TEST);
    int width, height;
    GLuint loc;
    glfwGetFramebufferSize(window, &width, &height);
    mat4 modelMat = mat4(1);
    mat4 shadowMVP = ortho(-2.f, 2.f, -2.f, 2.f, 0.01f, 10.f) * lookAt( lightPosition, vec3(0), vec3(0,1,0) ) * modelMat;
    //glEnable(GL_FRAMEBUFFER_SRGB);
    glUseProgram(shadowProgram.programID);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glViewport(0, 0, shadowMapSize, shadowMapSize);
    //glClearColor(1, 1, 1, 1);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    loc = glGetUniformLocation(shadowProgram.programID, "shadowMVP");
    glUniformMatrix4fv(loc, 1, false, value_ptr(shadowMVP));
    
    glBindVertexArray(vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    vec3 cameraPos = vec3( rotate( theta, vec3( 0, 1, 0 ) ) * rotate ( phi, vec3(1, 0, 0) ) * vec4( cameraPosition, 1 ) ); //rotate( 각도, 회전축 )
    mat4 viewMat = lookAt( cameraPos, vec3(0, 0, 0), vec3(0, 1, 0) );
    mat4 projectionMat = perspective( fovy/PI/180, width/(float)height, 0.1f, 100.f );
    
    glViewport(0, 0, width, height); //Viewport: 그림을 그릴 영역을 정함, (0,0,w,h): 0.0부터 w.h까지 그린다
    glClearColor(0, 0, 0, 0);//(R,G,B,불투명도 a)
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_FRAMEBUFFER_SRGB);
    
    glUseProgram(program.programID);
    
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
    glUniform3fv(loc, 1, value_ptr(diffuseColor[0]));
    loc = glGetUniformLocation(program.programID, "cameraPosition");
    glUniform3fv(loc, 1, value_ptr(cameraPos));
    loc = glGetUniformLocation(program.programID, "shininess");
    glUniform1f(loc, shininess[0]);
    loc = glGetUniformLocation(program.programID, "specularColor");
    glUniform3fv(loc, 1, value_ptr(specularColor[0]));
    
    //texture map과 bump map 각각 다른 slot에 bind, shader에 uniform으로 보내준다
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    loc = glGetUniformLocation(program.programID, "colorTexture");
    glUniform1i(loc, 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bumpID);
    loc = glGetUniformLocation(program.programID, "bumpTexture");
    glUniform1i(loc, 1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    loc = glGetUniformLocation(program.programID, "shadowMap");
    glUniform1i(loc, 2);
    loc = glGetUniformLocation(program.programID, "shadowMVP");
    glUniformMatrix4fv(loc, 1, false, value_ptr(shadowMVP));
    
    glBindVertexArray(vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
    
    
    glfwSwapBuffers(window);
}


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

   
