#pragma once

struct vec3 {
    float x;
    float y;
    float z;
    vec3* Next = nullptr;
};

struct vec3Tex {
    float u;
    float v;
    float w;
    vec3Tex* Next = nullptr;
};

// (vertex, texture, normal).
struct face {
    struct vertex {
        int vIndex;
        int tIndex; // Texture coordinate index
        int nIndex; // Normal index
    };
    vertex vertices[3]; // Assuming triangular faces
    face* Next = nullptr;
};

// (vertex, normal only).
struct face2 { 
    struct vertex {
        int vIndex;
        int nIndex; // Normal index
    };
    vertex vertices[3]; // Assuming triangular faces
    face2* Next = nullptr;
};

// Function to add a vertex to the linked list
void addVertex(vec3*& head, float x, float y, float z);

// Function to add a texture coordinate to the linked list
void addTextureCoord(vec3*& head, float u, float v, float w);

// Function to add a normal to the linked list
void addNormal(vec3*& head, float x, float y, float z);

// Function to add a face to the linked list
void addFace(face*& head, int v1, int t1, int n1, int v2, int t2, int n2, int v3, int t3, int n3);

// Function to add // to face
void addFace2(face2*& head, int v1, int n1, int v2, int n2, int v3, int n3);

// Function to print all vertices in the linked list
void printVertices(const vec3* head);

// Function to print all normals in the linked list
void printNormals(const vec3* head);

// Function to parse the .obj file containing v/t/n and store data in linked lists
bool parseObjFile(const char* inputFile, vec3*& vertices, vec3Tex*& textureCoords, vec3*& normals, face*& faces);

// Check //
bool isDoubleSlashFormat(const char* inputFile);

// Function to parse the .obj file containing v//n and store data in linked lists
bool parseObjFile2(const char* inputFile, vec3*& vertices, vec3*& normals, face2*& faces);

// Clean up dynamically allocated linked list
void freeVertices(vec3*& head);
void freeTextureCoords(vec3Tex*& head);
void freeNormals(vec3*& head);
void freeFaces(face*& head);
void freeFaces2(face2*& head);