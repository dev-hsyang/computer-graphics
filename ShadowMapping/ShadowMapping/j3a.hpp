//
//  j3a.hpp
//  ComputerGraphics_3
//
//  Created by Hyun Joon Shin on 12/04/2018.
//  Copyright © 2018 Hyun Joon Shin. All rights reserved.
//

#ifndef j3a_hpp
#define j3a_hpp

#ifdef __APPLE__
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <vector>
#include <string>

//! j3a (Joony 3D Ascii) file loader.
//!
//! To load data, simply call
//!
//!     loadJ3A( filename );
//!
//! Of course, to use this functionality, you add j3a.cpp file into your project.
//!
//! For XCode user:
//!   Do not forget to copy the required *.j3a file into your product directory
//!   like what you do for shader.vert, and shader.frag
//!
//! Then, vertices, faces, normal vectors, and other properties will be filled
//! automatically.
//!
//! When you fill vertex buffer, do as follows:
//!
//!     glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);
//!
//! When you fill element buffer, do as follows:
//!
//!     glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);
//!
//! When you draw elements, do like
//!
//!        glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
//!
//! That's it.
//!
//! Quick tip:
//!   When the program crashes, it is probabily because the file is not loaded well.
//!   To check the file is loaded well, see console or check the return value of loadJ3A function.


extern bool loadJ3A( const char* fn );
extern int nObjects;
extern std::vector<unsigned>        nVertices;
extern std::vector<glm::vec3*>        vertices;
extern std::vector<glm::vec3*>        normals;
extern std::vector<glm::vec2*>        texCoords;
extern std::vector<unsigned>        nTriangles;
extern std::vector<glm::u32vec3*>    triangles;
extern std::vector<glm::vec4>        diffuseColor;
extern std::vector<glm::vec3>        specularColor;
extern std::vector<float>            shininess;
extern std::vector<std::string>        diffuseMap;
extern std::vector<std::string>        bumpMap;
extern std::vector<std::string>        specularMap;
extern std::vector<std::string>        ambientOcclusionMap;

#endif /* j3a_hpp */
