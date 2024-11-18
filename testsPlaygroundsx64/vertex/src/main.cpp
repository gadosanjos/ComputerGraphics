#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>  // Include GLM for vector and matrix math
// youtube tutorial https://www.youtube.com/watch?v=OErPXo2UqvY&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=6

GLuint VBO;

// Define a simple 3D vector structure (replaces Vector3f)
/* struct Vector3f {
    float x, y, z;

    // Default constructor
    Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}

    // Constructor with initialization
    Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
}; */

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //glDrawArrays(GL_POINTS, 0, 1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

static void CreateVertexBuffer()
{
    /* Vector3f Vertices[1];
    Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);  // Create a single point at the origin */
    /* glm::vec3 Vertices[3];
    Vertices[0] = glm::vec3(0.0f, 1.0f, 0.0f); // Create a single point at the origin*/
  
    glEnable(GL_CULL_FACE); // Dont draw what we ccant see, in this case the back
    glm::vec3 Vertices[3];
    Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f); // Bottom Left
    Vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f); // Top
    Vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f); // Bottom Right

    // WITH CULL FACE ENABLED IF WE REVERSE THE ORDER TO LOOK FROM THE BACK IT WONT DRAW IT TO SAVE RESOURCES, COMMENT OUT AFTER TESTING
    // BY DEFAULT COUNTER CLOCKWISE MEANS FRONT FACING, CLOCKWISE MEANS BACK FACING
    // Can also set which rotation we want to be front or back with glFrontFace(GL_CW); GL_CW = COUNTERCLOKWISE
    // we can also define if we want front or back to be cut with glCullFace(GL_FRONT); this will stop rendering front faces
    glFrontFace(GL_CW); // Comment out to go back to defaul counter clockwise as front
    Vertices[1] = glm::vec3(0.0f, 1.0f, 0.0f); // Top
    Vertices[2] = glm::vec3(1.0f, -1.0f, 0.0f); // Bottom Right

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    int width = 1920/2;
    int height = 1080/2;
    glutInitWindowSize(width, height);

    int x = 200;
    int y = 100;
    glutInitWindowPosition(x, y);
    int win = glutCreateWindow("Tutorial 02");
    printf("window id: %d\n", win);

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    CreateVertexBuffer();

    glutDisplayFunc(RenderSceneCB);

    glutMainLoop();

    return 0;
}
