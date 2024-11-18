#pragma once

#include <vector>

// A 3D vector for vertices and normals
struct vec3 {
    float x;
    float y;
    float z;
};

// A 3D texture coordinate vector (with optional 'w')
struct vec3Tex {
    float u;
    float v;
    float w = 0.0f; // Default to 0.0
};

// A face structure for (vertex, texture, normal).
struct face {
    struct vertex {
        int vIndex;
        int tIndex; // Texture coordinate index
        int nIndex; // Normal index
    };
    vertex vertices[3]; // Assuming triangular faces
};

// A face structure for (vertex, normal only).
struct face2 { 
    struct vertex {
        int vIndex; // Vertex index
        int nIndex; // Normal index
    };
    vertex vertices[3]; // Assuming triangular faces
};

void printVertices(const std::vector<vec3>& vertices);
void printNormals(const std::vector<vec3>& normals);
void printFaces(const std::vector<face>& faces);
void printFaces2(const std::vector<face2>& faces);

// Function to parse the .obj file containing v/t/n and store data in linked lists
bool parseObjFile(const char* inputFile, std::vector<vec3>& vertices, std::vector<vec3Tex>& textureCoords, std::vector<vec3>& normals, std::vector<face>& faces);

// Check //
bool isDoubleSlashFormat(const char* inputFile);

// Function to parse the .obj file containing v//n and store data in linked lists
bool parseObjFile2(const char* inputFile, std::vector<vec3>& vertices, std::vector<vec3Tex>& textureCoords, std::vector<vec3>& normals, std::vector<face2>& faces);
