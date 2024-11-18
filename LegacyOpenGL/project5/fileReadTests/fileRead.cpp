#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include "fileRead.h"

void printVertices(const std::vector<vec3>& vertices) {
    for (const auto& vertex : vertices) {
        std::cout << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }
}

void printNormals(const std::vector<vec3>& normals) {
    for (const auto& normal : normals) {
        std::cout << "vn " << normal.x << " " << normal.y << " " << normal.z << std::endl;
    }
}

void printFaces(const std::vector<face>& faces) {
    for (const auto& f : faces) {
        std::cout << "f " 
                  << f.vertices[0].vIndex << "/" << f.vertices[0].tIndex << "/" << f.vertices[0].nIndex << " "
                  << f.vertices[1].vIndex << "/" << f.vertices[1].tIndex << "/" << f.vertices[1].nIndex << " "
                  << f.vertices[2].vIndex << "/" << f.vertices[2].tIndex << "/" << f.vertices[2].nIndex << std::endl;
    }
}

void printFaces2(const std::vector<face2>& faces) {
    for (const auto& f : faces) {
        std::cout << "f " 
                  << f.vertices[0].vIndex << "//" << f.vertices[0].nIndex << " "
                  << f.vertices[1].vIndex << "//" << f.vertices[1].nIndex << " "
                  << f.vertices[2].vIndex << "//" << f.vertices[2].nIndex << std::endl;
    }
}

bool parseObjFile(const char* inputFile, std::vector<vec3>& vertices, std::vector<vec3Tex>& textureCoords, std::vector<vec3>& normals, std::vector<face>& faces) {
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
            vec3 vertex;
            infile >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (prefix == "vt") {
            vec3Tex texCoord;
            infile >> texCoord.u >> texCoord.v;
            // Check if there's a third value for 3D texture coordinates
            if (infile.peek() != '\n') {
                infile >> texCoord.w; // Optional w
            } 
            textureCoords.push_back(texCoord);
        } else if (prefix == "vn") {
            vec3 normal;
            infile >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (prefix == "f") {
            face f;
            char separator; // For `/` between indices
            for (int i = 0; i < 3; ++i) {
                infile >> f.vertices[i].vIndex >> separator >> f.vertices[i].tIndex >> separator >> f.vertices[i].nIndex;
            }
            faces.push_back(f);
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

bool parseObjFile2(const char* inputFile, std::vector<vec3>& vertices, std::vector<vec3Tex>& textureCoords, std::vector<vec3>& normals, std::vector<face2>& faces) {
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
            vec3 vertex;
            infile >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (prefix == "vt") {
            vec3Tex texCoord;
            infile >> texCoord.u >> texCoord.v;
            if (infile.peek() != '\n') {
                infile >> texCoord.w; // Optional w
            }
            textureCoords.push_back(texCoord);
        } else if (prefix == "vn") {
            vec3 normal;
            infile >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (prefix == "f") {
            face2 f;
            char separator1, separator2;
            for (int i = 0; i < 3; ++i) {
                infile >> f.vertices[i].vIndex >> separator1 >> separator2 >> f.vertices[i].nIndex;
            }
            faces.push_back(f);
        } else {
            // Skip the rest of the line for unrecognized prefixes
            infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    infile.close();
    return true;
}