//
//  toys.h
//  ComputerGraphics_2
//
//  Created by Hyun Joon Shin on 13/03/2018.
//  Copyright © 2018 Hyun Joon Shin. All rights reserved.
//

#ifndef toys_h
#define toys_h

#ifdef __APPLE__
#include "OpenGL/gl3.h"
#else
#include "gl/glew.h"
#endif
#include <string>
#include <fstream>
#include <iostream>

#ifdef WIN32
typedef wchar_t CHAR_T;
#include <windows.h>
inline std::wstring utf82Unicode(const std::string& s) {
    int len = MultiByteToWideChar(CP_UTF8,0,s.c_str(),-1,NULL,0);
    wchar_t* wstr = new wchar_t[len+1];
    len = MultiByteToWideChar(CP_UTF8,0,s.c_str(),-1,wstr,len);
    wstr[len]=L'\0';
    std::wstring ws(L""); ws+=wstr;
    delete wstr;
    return ws;
}
#else
typedef char CHAR_T;
#define utf82Unicode(X) (X)
#endif

inline std::string getFilenameFromAbsPath( const std::string& filename )
{
    size_t slashPos = filename.find_last_of('/');
    if( slashPos == std::string::npos ) return filename;
    if( slashPos == filename.length()-1 ) return "";
    return filename.substr(slashPos+1);
}


inline std::string loadText(const std::string& filename) {
    std::ifstream t(utf82Unicode(filename));
    if( !t.is_open() ) {
        std::cerr<<"[ERROR] Text file: "<<getFilenameFromAbsPath(filename)<<" is not found\n";
        return "";
    }
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
}


struct Program {
    GLuint programID = 0;
    GLuint vertShaderID = 0;
    GLuint fragShaderID = 0;
    
    
    static inline void printInfoProgramLog(GLuint obj )
    {
        int infologLength = 0, charsWritten  = 0;
        glGetProgramiv( obj, GL_INFO_LOG_LENGTH, &infologLength );
        if( infologLength <=0 ) return;
        std::cerr<<"Program Info:"<<std::endl;
        char *infoLog = new char[infologLength];
        glGetProgramInfoLog( obj, infologLength, &charsWritten, infoLog );
        std::cerr<<infoLog<<std::endl;
        delete [] infoLog;
    }
    static inline void printInfoShaderLog(GLuint obj )
    {
        int infologLength = 0, charsWritten  = 0;
        glGetShaderiv ( obj, GL_INFO_LOG_LENGTH, &infologLength );
        if( infologLength <=0 ) return;
        std::cerr<<"Shader Info:"<<std::endl;
        char *infoLog = new char[infologLength];
        glGetShaderInfoLog ( obj, infologLength, &charsWritten, infoLog );
        std::cerr<<infoLog<<std::endl;
        delete [] infoLog;
    }

    
    
    inline void loadShaders( const char* vsFilename, const char* fsFilename ) {
        cleanUp();
        
        vertShaderID = glCreateShader(GL_VERTEX_SHADER);
        fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        
        programID = glCreateProgram();
        
        std::string vertCode = loadText( vsFilename );
        std::string fragCode = loadText( fsFilename );
        
        if( vertCode.length() <1 ) {
            std::cerr<<"Vertex shader code is not loaded properly"<<std::endl;
            return;
        }
        
        const GLchar* vshaderCode = vertCode.c_str();
        glShaderSource    ( vertShaderID, 1, &vshaderCode, nullptr );
        glCompileShader    ( vertShaderID );
        printInfoShaderLog    ( vertShaderID );
        glAttachShader  ( programID, vertShaderID );
        
        const GLchar* fshaderCode = fragCode.c_str();
        glShaderSource    ( fragShaderID, 1, &fshaderCode, nullptr );
        glCompileShader    ( fragShaderID );
        printInfoShaderLog    ( fragShaderID ) ;
        glAttachShader  ( programID, fragShaderID );
        
        glLinkProgram( programID );
        glUseProgram ( programID );
        printInfoProgramLog( programID );
    }
    void cleanUp() {
        if( programID )
            glDeleteProgram( programID );
        if( vertShaderID )
            glDeleteShader( vertShaderID );
        if( fragShaderID )
            glDeleteShader( fragShaderID );
        programID = vertShaderID = fragShaderID = 0;
    }
    ~Program() {
        cleanUp();
    }
};


#endif /* toys_h */
