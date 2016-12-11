// Example gl_projection
// - Demonstrates the use of perspective projection
// - Animates object rotation
// - Useful for demonstrating culling and depth test concepts

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "mesh.h"

#include "gl_projection_vert.h"
#include "gl_projection_frag.h"

const unsigned int SIZE = 512;

#define PI 3.14159265358979323846f

GLuint vbo;
GLuint cdbo;

struct Point {
    float x;
    float y;
    float z;
};

glm::vec3 lerp(glm::vec3 A, glm::vec3 B, float t){
    glm::vec3 temp;
    temp.x = A.x + ((B.x - A.x) * t);
    temp.y = A.y + ((B.y - A.y) * t);
    temp.z = A.z + ((B.z - A.z) * t);
    return temp;
}

void Bezier(std::vector<GLfloat> &out, std::vector<glm::vec3> in, std::vector<GLfloat> &texCoords, float N){
    glm::vec3 A,B,C,X,Y,P;
    float dt = 1.0f / N;

    for (float t = 0.0f; t <= 1.0f; t += dt){
        std::vector<glm::vec3> temp;
        for(int i = 0; i < 16; i += 4) {
            A = lerp(in[i], in[i + 1], t);
            B = lerp(in[i + 1], in[i + 2], t);
            C = lerp(in[i + 2], in[i + 3], t);
            X = lerp(A, B, t);
            Y = lerp(B, C, t);
            P = lerp(X, Y, t);
            temp.push_back(P);
        }

        for(float i = 0.0f; i <= 1.0f; i += dt) {
            A = lerp(temp[0], temp[1], i);
            B = lerp(temp[1], temp[2], i);
            C = lerp(temp[2], temp[3], i);
            X = lerp(A, B, i);
            Y = lerp(B, C, i);
            P = lerp(X, Y, i);
            out.push_back(P.x);
            out.push_back(P.y);
            out.push_back(P.z);
            texCoords.push_back(t);
            texCoords.push_back(i);
        }
    }

}

class My3DShape{
public:
    std::vector<glm::vec3>  points = {
            {-0.95f, 0.4f, 0.0f}, {-0.3f, 0.95f, 0.0f},
            {0.3f, 0.95f, 0.0f}, {+0.95f, 0.4f, 1.0f},

            {-0.95f, 0.3f, 0.0f}, {-0.95f, 0.95f, 1.0f},
            {0.95f, 0.95f, 1.0f}, {+0.95f, 0.3f, 0.0f},

            {-0.95f, -0.3f, 0.0f}, {-0.95f, -0.95f, 1.0f},
            {0.95f, -0.95f, 1.0f}, {+0.95f, -0.3f, 0.0f},

            {-0.95f, -0.4f, 1.0f}, {-0.3f, -0.95f, 0.0f},
            {0.3f, -0.95f, 0.0f}, {+0.95f, -0.4f, 0.0f},
    };
    float rotation = 0;
    std::vector<GLuint> elements;
    std::vector<GLfloat> out;
    std::vector<GLfloat> texCoords;

    void draw(){
        glDrawElements(GL_TRIANGLES, (GLsizei) elements.size(), GL_UNSIGNED_INT, 0);
    };

    void rotate(float R) {
        rotation += R;
    };

    glm::mat4 createMatrix() {
        glm::mat4 model = glm::mat4();
        model = glm::rotate(model, rotation, glm::vec3(0.0,1.0,0.0));
        //model = glm::translate(model, glm::vec3(positionX,positionY, 0.0));
        //model = glm::scale(model, glm::vec3(scaling,scaling, 0.0));
        return model;
    };

    void update(float time){
        points = {
                {-0.95f, 0.4f, 0.0f}, {-0.3f, 0.95f, 0.0f},
                {0.3f, 0.95f, 0.0f}, {+0.95f, 0.4f, std::sin(time)},

                {-0.95f, 0.3f, 0.0f}, {-0.95f, 0.95f, 1.0f},
                {0.95f, 0.95f, 1.0f}, {+0.95f, 0.3f, 0.0f},

                {-0.95f, -0.3f, 0.0f}, {-0.95f, -0.95f, 1.0f},
                {0.95f, -0.95f, 1.0f}, {+0.95f, -0.3f, 0.0f},

                {-0.95f, -0.4f, std::sin(time)}, {-0.3f, -0.95f, 0.0f},
                {0.3f, -0.95f, 0.0f}, {+0.95f, -0.4f, 0.0f},
        };
        out.clear();
        texCoords.clear();
        Bezier(out, points, texCoords, 9);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, out.size() * sizeof(GLfloat), out.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, cdbo);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

    };
};

void SetTransformation(ShaderPtr program, My3DShape my3DShape) {
    auto transform = glm::mat3({
                                       1.0, 0.0, 0.0,
                                       0.0, 1.0, 0.0,
                                       0.0, 0.0, 1.0
                               });

    auto transform_uniform = program->GetUniformLocation("Transform");
    glUniformMatrix4fv(transform_uniform, 1, GL_FALSE, glm::value_ptr(my3DShape.createMatrix()));
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return EXIT_FAILURE;
    }

    My3DShape shape;
    float time = 0;

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            shape.elements.push_back(i + j * 10);
            shape.elements.push_back(i + 1 + j * 10);
            shape.elements.push_back(i + (j + 1) * 10);

            shape.elements.push_back(i + 1 + j * 10);
            shape.elements.push_back(i + (j + 1) * 10);
            shape.elements.push_back(i + 1 + (j + 1) * 10);
        }
    }

    Bezier(shape.out, shape.points, shape.texCoords, 9);

    // Setup OpenGL context
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Try to create a window
    auto window = glfwCreateWindow( SIZE, SIZE, "PPGSO gl_gradient", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to open GLFW window, your graphics card is probably only capable of OpenGL 2.1" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Finalize window setup
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    if (!glewIsSupported("GL_VERSION_3_3")) {
        std::cerr << "Failed to initialize GLEW with OpenGL 3.3!" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Load shaders
    auto program = ShaderPtr(new Shader{gl_projection_vert, gl_projection_frag});
    program->Use();

    // Generate a vertex array object
    // This keeps track of what attributes are associated with buffers
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate a vertex buffer object, this will feed data to the vertex shader

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, shape.out.size() * sizeof(GLfloat), shape.out.data(), GL_STATIC_DRAW);

    auto position_attrib = program->GetAttribLocation("Position");
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_attrib);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.elements.size() * sizeof(GLuint), shape.elements.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &cdbo);
    glBindBuffer(GL_ARRAY_BUFFER, cdbo);
    glBufferData(GL_ARRAY_BUFFER, shape.texCoords.size() * sizeof(GLfloat), shape.texCoords.data(), GL_STATIC_DRAW);

    // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input

    auto texcoord_attrib = program->GetAttribLocation("TexCoord");
    glVertexAttribPointer(texcoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texcoord_attrib);

    auto texture = TexturePtr(new Texture{"lena.rgb", 512, 512});
    program->SetTexture(texture, "Texture");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Main execution loop
    while (!glfwWindowShouldClose(window) ) {
        // Set gray background
        glClearColor(.5,.5,.5,0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw triangles using the program
        // glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) (out.size() / 2));

        //task 6
        //glDrawElements(GL_TRIANGLES, (GLsizei) elementsOutput.size(), GL_UNSIGNED_INT, 0);

       // glDrawElements(GL_LINE_STRIP, (GLsizei) elementsOutput.size(), GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_LINE_STRIP, (GLsizei) 9, GL_UNSIGNED_INT, 0);

        //task 7
        float R = 0.000f;
        if((glfwGetKey(window, GLFW_KEY_RIGHT)) == GLFW_PRESS){
            R = 0.001f;
        }
        else if((glfwGetKey(window, GLFW_KEY_LEFT)) == GLFW_PRESS){
            R = -0.001f;
        }
        else {
            R = 0.000f;
        }
        shape.update(time += 0.001f);
        shape.rotate(R);
        SetTransformation(program, shape);
        shape.draw();

        // Display result
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);
    glfwTerminate();

    return EXIT_SUCCESS;
}