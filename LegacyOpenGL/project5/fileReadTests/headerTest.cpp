#include <iostream>
#include <string>
#include <vector>
#include "fileRead.h"

int main()
{
    std::vector<vec3> vertices; // Vector to store vertices
    std::vector<vec3Tex> textureCoords; // Vector to store texture coordinates
    std::vector<vec3> normals; // Vector to store normals
    std::vector<face> faces; // Vector to store faces (v/t/n format)
    std::vector<face2> faces2; // Vector to store faces (v//n format)

    const char* inputFile = "cow.obj";

    bool isDoubleSlash = isDoubleSlashFormat(inputFile);

    if (isDoubleSlash) {
        std::cout << "Using `v//n` parser." << std::endl;
        if (parseObjFile2(inputFile, vertices, textureCoords, normals, faces2)){
            std::cout << "Parsed vertices:" << std::endl;
            printVertices(vertices);

            std::cout << "Parsed texture coordinates:" << std::endl;
            for (const auto& tex : textureCoords) {
                std::cout << "vt " << tex.u << " " << tex.v << " " << tex.w << std::endl;
            }

            std::cout << "Parsed normals:" << std::endl;
            printNormals(normals);    

            std::cout << "Parsed faces:" << std::endl;
            printFaces2(faces2);

        } else {
            std::cerr << "Failed to parse the .obj file." << std::endl;
        }
        std::cout << "Using `v//n` parser." << std::endl;
    } else {
        std::cout << "Using `v/t/n` parser." << std::endl;
        if (parseObjFile(inputFile, vertices, textureCoords, normals, faces)) {
            std::cout << "Parsed vertices:" << std::endl;
            printVertices(vertices);

            std::cout << "Parsed texture coordinates:" << std::endl;
            for (const auto& tex : textureCoords) {
                std::cout << "vt " << tex.u << " " << tex.v << " " << tex.w << std::endl;
            }

            std::cout << "Parsed normals:" << std::endl;
            printNormals(normals);

            std::cout << "Parsed faces:" << std::endl;
            printFaces(faces);
        } else {
            std::cerr << "Failed to parse the .obj file." << std::endl;
        }
        std::cout << "Using `v/t/n` parser." << std::endl;
    }

    return 0;
}
