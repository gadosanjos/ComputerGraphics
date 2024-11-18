#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "fileRead.h"

void addVertex(vec3*& head, float x, float y, float z) {
    vec3* newVertex = new vec3;
    newVertex->x = x;
    newVertex->y = y;
    newVertex->z = z;
    newVertex->Next = nullptr;  

    if (head == nullptr) {
        head = newVertex;
    } else {
        vec3* current = head;
        while (current->Next != nullptr) {
            current = current->Next;
        }
        current->Next = newVertex;
    }
}

void addTextureCoord(vec2*& head, float u, float v) {
    vec2* newCoord = new vec2;
    newCoord->u = u;
    newCoord->v = v;
    newCoord->Next = nullptr;

    if (head == nullptr) {
        head = newCoord;
    } else {
        vec2* current = head;
        while (current->Next != nullptr) {
            current = current->Next;
        }
        current->Next = newCoord;
    }
}

void addNormal(vec3*& head, float x, float y, float z) {
    vec3* newNormal = new vec3;
    newNormal->x = x;
    newNormal->y = y;
    newNormal->z = z;
    newNormal->Next = nullptr;

    if (head == nullptr) {
        head = newNormal;
    } else {
        vec3* current = head;
        while (current->Next != nullptr) {
            current = current->Next;
        }
        current->Next = newNormal;
    }
}

void addFace(face*& head, int v1, int t1, int n1, int v2, int t2, int n2, int v3, int t3, int n3) {
    face* newFace = new face;
    newFace->vertices[0] = {v1, t1, n1};
    newFace->vertices[1] = {v2, t2, n2};
    newFace->vertices[2] = {v3, t3, n3};
    newFace->Next = nullptr;

    if (head == nullptr) {
        head = newFace;
    } else {
        face* current = head;
        while (current->Next != nullptr) {
            current = current->Next;
        }
        current->Next = newFace;
    }
}

void printVertices(const vec3* head) {
    const vec3* current = head;
    while (current != nullptr) {
        std::cout << "v " << current->x << " " << current->y << " " << current->z << std::endl;
        current = current->Next;
    }
}

void freeVertices(vec3*& head) {
    while (head != nullptr) {
        vec3* temp = head;
        head = head->Next;
        delete temp;
    }
}

void freeTextureCoords(vec2*& head) {
    while (head != nullptr) {
        vec2* temp = head;
        head = head->Next;
        delete temp;
    }
}

void freeNormals(vec3*& head) {
    while (head != nullptr) {
        vec3* temp = head;
        head = head->Next;
        delete temp;
    }
}

void freeFaces(face*& head) {
    while (head != nullptr) {
        face* temp = head;
        head = head->Next;
        delete temp;
    }
}

bool copyFile(const char* inputFile, vec3*& vertices) {
    std::streampos begin, end, size;
    const int bufferSize = 1024;
    char buffer[bufferSize];

    std::ifstream infile(inputFile, std::ios::binary);

    std::string prefix;
    float x, y, z;

    if (infile.is_open()) {
        // Determine file size
        infile.seekg(0, std::ios::beg);
        begin = infile.tellg();
        infile.seekg(0, std::ios::end);
        end = infile.tellg();
        size = end - begin;

        std::cout << "File size: " << size << " bytes." << std::endl;

        // Reset pointer to the beginning
        infile.seekg(0, std::ios::beg);

        // Copying data
        while (infile >> prefix) {
            if (prefix == "v") {
                // Directly read x, y, z values from the stream
                infile >> x >> y >> z;
                addVertex(vertices, x, y, z);
            } else {
                // Skip the rest of the line for other prefixes
                infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        // Close files
        infile.close();
        std::cout << "Vertices extracted successfully." << std::endl;
        return true;
    } else {
        std::cerr << "Error opening file(s)." << std::endl;
        return false;
    }
}

bool parseObjFile(const char* inputFile, vec3*& vertices, vec2*& textureCoords, vec3*& normals, face*& faces) {
    std::streampos begin, end, size;
    const int bufferSize = 1024;
    char buffer[bufferSize];

    std::ifstream infile(inputFile);

    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << inputFile << std::endl;
        return false;
    }

    // Determine file size
    infile.seekg(0, std::ios::beg);
    begin = infile.tellg();
    infile.seekg(0, std::ios::end);
    end = infile.tellg();
    size = end - begin;

    std::cout << "File size: " << size << " bytes." << std::endl;

    // Reset pointer to the beginning
    infile.seekg(0, std::ios::beg);

    std::string prefix;
    while (infile >> prefix) {
        if (prefix == "v") {
            float x, y, z;
            infile >> x >> y >> z;
            addVertex(vertices, x, y, z);
        } else if (prefix == "vt") {
            float u, v;
            infile >> u >> v;
            addTextureCoord(textureCoords, u, v);
        } else if (prefix == "vn") {
            float x, y, z;
            infile >> x >> y >> z;
            addNormal(normals, x, y, z);
        } else if (prefix == "f") {
            int v1, t1, n1, v2, t2, n2, v3, t3, n3;
            char separator; // For slashes between indices
            infile >> v1 >> separator >> t1 >> separator >> n1
                   >> v2 >> separator >> t2 >> separator >> n2
                   >> v3 >> separator >> t3 >> separator >> n3;
            addFace(faces, v1, t1, n1, v2, t2, n2, v3, t3, n3);
        } else {
            // Skip the rest of the line for unrecognized prefixes
            infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    infile.close();
    return true;
}