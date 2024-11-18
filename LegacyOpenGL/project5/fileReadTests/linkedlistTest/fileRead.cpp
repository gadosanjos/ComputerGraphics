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

void addTextureCoord(vec3Tex*& head, float u, float v, float w = 0.0f) {
    vec3Tex* newCoord = new vec3Tex;
    newCoord->u = u;
    newCoord->v = v;
    newCoord->w = w;
    newCoord->Next = nullptr;

    if (head == nullptr) {
        head = newCoord;
    } else {
        vec3Tex* current = head;
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

void addFace2(face2*& head, int v1, int n1, int v2, int n2, int v3, int n3) {
    face2* newFace = new face2;
    newFace->vertices[0] = {v1, n1};
    newFace->vertices[1] = {v2, n2};
    newFace->vertices[2] = {v3, n3};
    newFace->Next = nullptr;

    if (head == nullptr) {
        head = newFace;
    } else {
        face2* current = head;
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

void printNormals(const vec3* head) {
    const vec3* current = head;
    while (current != nullptr) {
        std::cout << "vn " << current->x << " " << current->y << " " << current->z << std::endl;
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

void freeTextureCoords(vec3Tex*& head) {
    while (head != nullptr) {
        vec3Tex* temp = head;
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

void freeFaces2(face2*& head) {
    while (head != nullptr) {
        face2* temp = head;
        head = head->Next;
        delete temp;
    }
}

bool parseObjFile(const char* inputFile, vec3*& vertices, vec3Tex*& textureCoords, vec3*& normals, face*& faces) {
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
            float u, v, w = 0.0f; // Default w to 0.0 in case we need to ignore it
            infile >> u >> v;
            // Check if there's a third value for 3D texture coordinates
            if (infile.peek() != '\n' && infile >> w) {
                // Read w if present
            } 
            addTextureCoord(textureCoords, u, v, w);
        } else if (prefix == "vn") {
            float x, y, z;
            infile >> x >> y >> z;
            addNormal(normals, x, y, z);
        } else if (prefix == "f") {
            int v1, t1, n1, v2, t2, n2, v3, t3, n3;
            char separator; // For `/` between indices
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

bool isDoubleSlashFormat(const char* inputFile) {
    std::ifstream infile(inputFile);
    std::string line;
    while (std::getline(infile, line)) {
        if (line.rfind("f ", 0) == 0) { // Check for lines starting with 'f'
            if (line.find("//") != std::string::npos) {
                return true; // `v//n` format detected
            } else {
                return false; // `v/t/n` format detected
            }
        }
    }
    return false; // Default to `v/t/n` if no faces are found
}

bool parseObjFile2(const char* inputFile, vec3*& vertices, vec3*& normals, face2*& faces) {
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
        } else if (prefix == "vn") {
            float x, y, z;
            infile >> x >> y >> z;
            addNormal(normals, x, y, z);
        } else if (prefix == "f") {
            int v1, v2, v3, t1, t2, t3, n1, n2, n3;
            char separator1, separator2;

            infile >> v1 >> separator1 >> separator2 >> n1
                    >> v2 >> separator1 >> separator2 >> n2
                    >> v3 >> separator1 >> separator2 >> n3;
            addFace2(faces, v1, n1, v2, n2, v3, n3);
        } else {
            // Skip the rest of the line for unrecognized prefixes
            infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    infile.close();
    return true;
}