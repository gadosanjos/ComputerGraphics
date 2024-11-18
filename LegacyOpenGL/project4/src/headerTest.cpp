#include <iostream>
#include <string>
#include "fileRead.h"

int main()
{
    vec3* vertices = nullptr; // Linked list to store vertices
    vec2* textureCoords = nullptr; // Linked list to store texture
    vec3* normals = nullptr; // Linked list to store normals
    face* faces = nullptr; // Linked list to store faces

    const char* inputFile = "cube.obj";

    /* if (copyFile(inputFile, vertices)) {
        std::cout << "File copy successful." << std::endl;
        printVertices(vertices);
    } else {
        std::cout << "Failed to extract vertices." << std::endl;
    } */

    if (parseObjFile(inputFile, vertices, textureCoords, normals, faces)) {
        std::cout << "Parsed vertices:" << std::endl;
        printVertices(vertices);

        std::cout << "Parsed texture coordinates:" << std::endl;
        vec2* currentTex = textureCoords;
        while (currentTex) {
            std::cout << "vt " << currentTex->u << " " << currentTex->v << std::endl;
            currentTex = currentTex->Next;
        }

        std::cout << "Parsed normals:" << std::endl;
        printVertices(normals);

        std::cout << "Parsed faces:" << std::endl;
        face* currentFace = faces;
        while (currentFace) {
            for (int i = 0; i < 3; ++i) {
                std::cout << "f " << currentFace->vertices[i].vIndex << "/"
                          << currentFace->vertices[i].tIndex << "/"
                          << currentFace->vertices[i].nIndex << " ";
            }
            std::cout << std::endl;
            currentFace = currentFace->Next;
        }
    } else {
        std::cerr << "Failed to parse the .obj file." << std::endl;
    }

    // Free allocated memory
    freeVertices(vertices);
    freeTextureCoords(textureCoords);
    freeNormals(normals);
    freeFaces(faces);

    return 0;
}
