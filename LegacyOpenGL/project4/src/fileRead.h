#pragma once

struct vec3 {
    float x;
    float y;
    float z;
    vec3* Next = nullptr;
};

struct vec2 {
    float u;
    float v;
    vec2* Next = nullptr;
};

struct face {
    struct vertex {
        int vIndex;
        int tIndex; // Texture coordinate index
        int nIndex; // Normal index
    };
    vertex vertices[3]; // Assuming triangular faces
    face* Next = nullptr;
};

// Function to add a vertex to the linked list
void addVertex(vec3*& head, float x, float y, float z);

// Function to add a texture coordinate to the linked list
void addTextureCoord(vec2*& head, float u, float v);

// Function to add a normal to the linked list
void addNormal(vec3*& head, float x, float y, float z);

// Function to add a face to the linked list
void addFace(face*& head, int v1, int t1, int n1, int v2, int t2, int n2, int v3, int t3, int n3);

// Function to print all vertices in the linked list
void printVertices(const vec3* head);

// Function to parse the .obj file and add vertices to the linked list
bool copyFile(const char* inputFile, vec3*& vertices);

// Function to parse the .obj file and store data in linked lists
bool parseObjFile(const char* inputFile, vec3*& vertices, vec2*& textureCoords, vec3*& normals, face*& faces);

// Clean up dynamically allocated linked list
void freeVertices(vec3*& head);
void freeTextureCoords(vec2*& head);
void freeNormals(vec3*& head);
void freeFaces(face*& head);