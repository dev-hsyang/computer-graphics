//
//  j3a.cpp
//  ComputerGraphics_3
//
//  Created by Hyun Joon Shin on 12/04/2018.
//  Copyright © 2018 Hyun Joon Shin. All rights reserved.
//
#include "j3a.hpp"
#include <fstream>
#include <string>

using namespace std;
using namespace glm;

int nObjects=0;
std::vector<unsigned>        nVertices;
std::vector<glm::vec3*>        vertices;
std::vector<glm::vec3*>        normals;
std::vector<glm::vec2*>        texCoords;
std::vector<unsigned>        nTriangles;
std::vector<glm::u32vec3*>    triangles;
std::vector<glm::vec4>        diffuseColor;
std::vector<glm::vec3>        specularColor;
std::vector<float>            shininess;
std::vector<std::string>    diffuseMap;
std::vector<std::string>    bumpMap;
std::vector<std::string>    specularMap;
std::vector<std::string>    ambientOcclusionMap;

std::string readString( istream& ifs ) {
    string ret = "";
    std::getline(ifs, ret, '\"');
    std::getline(ifs, ret, '\"');
    return ret;
}

bool loadJ3A( const char* fn ) {
    
    for( auto ptr: vertices ) delete ptr; vertices.clear();
    for( auto ptr: normals ) delete ptr; normals.clear();
    for( auto ptr: texCoords ) delete ptr; texCoords.clear();
    for( auto ptr: triangles ) delete ptr; triangles.clear();
    
    ifstream ifs( fn );
    if( !ifs.is_open() ) {
        printf("J3A file cannot be open.. maybe file not found?\n");
        return false;
    }
    
    ifs>>nObjects;
    printf("Number of objects: %d\n", nObjects );
    
    
    nVertices.resize( nObjects );
    vertices.resize( nObjects );
    normals.resize( nObjects );
    texCoords.resize( nObjects );
    nTriangles.resize( nObjects );
    triangles.resize( nObjects );
    diffuseColor.resize( nObjects );
    specularColor.resize( nObjects );
    shininess.resize( nObjects );
    diffuseMap.resize( nObjects );
    bumpMap.resize( nObjects );
    specularMap.resize( nObjects );
    ambientOcclusionMap.resize( nObjects );
    
    for( int i=0; i<nObjects; i++ ) {
        int temp;
        ifs>>diffuseColor[i].r>>diffuseColor[i].g>>diffuseColor[i].b>>diffuseColor[i].a;
        printf("Diff: %f %f %f %f\n", diffuseColor[i].r, diffuseColor[i].g, diffuseColor[i].b, diffuseColor[i].a );
        ifs>>specularColor[i].r>>specularColor[i].g>>specularColor[i].b>>shininess[i];
        printf("Spec: %f %f %f %f\n", specularColor[i].r, specularColor[i].g, specularColor[i].b, shininess[i] );
        diffuseMap[i] = readString( ifs );
        printf("DiffMap: %s\n", diffuseMap[i].c_str() );

        bumpMap[i] = readString( ifs );
        printf("BumpMap: %s\n", bumpMap[i].c_str() );
        ifs>>temp;
        specularMap[i] = readString( ifs );
        printf("SpecMap: %s\n", specularMap[i].c_str() );
        ambientOcclusionMap[i] = readString( ifs );
        printf("AmbOcc: %s\n", ambientOcclusionMap[i].c_str() );

        ifs>>nVertices[i];
        
        printf("Number of vertices: %d (of object: %d)\n", nVertices[i], i );
        vertices[i]        = new vec3[nVertices[i]];
        normals[i]        = new vec3[nVertices[i]];
        texCoords[i]    = new vec2[nVertices[i]];
        
        for( unsigned j=0; j<nVertices[i]; j++ )
            ifs>>vertices[i][j].x>>vertices[i][j].y>>vertices[i][j].z;
        for( unsigned j=0; j<nVertices[i]; j++ )
            ifs>>normals[i][j].x>>normals[i][j].y>>normals[i][j].z;
        for( unsigned j=0; j<nVertices[i]; j++ )
            ifs>>texCoords[i][j].x>>texCoords[i][j].y;
        
        ifs>>nTriangles[i];
        printf("Number of triangles: %d (of object: %d)\n", (int)nTriangles[i], i );
        triangles[i] = new u32vec3[nTriangles[i]];
        for( unsigned j=0; j<nTriangles[i]; j++ ) {
            ifs>>triangles[i][j].x>>triangles[i][j].y>>triangles[i][j].z;
            if( ifs.eof() ) printf("EOF\n");
        }
        int xxx;
        ifs>>xxx;
        if( ifs.eof() ) printf("EOF\n");
    }
    ifs.close();
    return true;
}
