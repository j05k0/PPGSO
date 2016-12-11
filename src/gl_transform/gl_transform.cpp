
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "mesh.h"
#include "gl_transform_vert.h"
#include "gl_transform_frag.h"

class My2DShape{
public:
    std::vector<glm::vec2> points;
    float positionX = 0.0;
    float positionY = 0.0;
    float rotation = 0;
    float scaling = 1;
    int direction = 1;
    int scalingDir = 1;

    void draw(){
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei) (points.size()));
    };

    void move(float X, float Y){
        if(positionX > 1  || positionX < 0 )
            direction = -direction;
        positionX += X * direction;
    };

    void rotate(float R) {
        rotation += R;
    };

    void scale(float S){
        if((scaling < 1) && (scaling >= 0)){
            scaling += scalingDir * S;
        }
        else{
            scalingDir = -scalingDir;
            scaling += scalingDir * S;
        }

    }

    glm::mat4 createMatrix() {

        glm::mat4 model = glm::mat4();
        model = glm::rotate(model, rotation, glm::vec3(0.0,0.0,1.0));
        model = glm::translate(model, glm::vec3(positionX,positionY, 0.0));
        model = glm::scale(model, glm::vec3(scaling,scaling, 0.0));
        return model;
    };

};

const unsigned int SIZE = 512;

void SetTransformation(ShaderPtr program, float time, My2DShape my2DShape) {
    // Create transformation matrix
    // NOTE: glm matrices are declared column major !

    // identity
    auto transform = glm::mat3({
                                       1.0, 0.0, 0.0,
                                       0.0, 1.0, 0.0,
                                       0.0, 0.0, 1.0
                               });

    // scale
    // transform *= glm::mat3({
    //    std::sin(time), 0.0, 0.0,
    //    0.0, std::sin(time), 0.0,
    //    0.0, 0.0, 1.0
    //  });

    // squash
    // transform *= glm::mat3({
    //   std::sin(time), 0.0, 0.0,
    //   0.0, std::cos(time), 0.0,
    //   0.0, 0.0, 1.0
    // });

    // rotate
    /*transform *= glm::mat3({
       std::cos(time), std::sin(time), 0.0,
       -std::sin(time), std::cos(time), 0.0,
       0.0, 0.0, 1.0
    });*/

    // translate
    // transform *= glm::mat3({
    //    1.0, 0.0, 0.0,
    //   0.0, 1.0, 0.0,
    //   std::sin(time)/2.0, std::cos(time)/2.0, 1.0
    // });

    // Rotate around top right corner
    // transform *= glm::mat3({
    //   // Move to origin
    //   1.0, 0.0, 0.0,
    //   0.0, 1.0, 0.0,
    //   1.0, 1.0, 1.0
    // }) * glm::mat3({
    //   // Rotate
    //   std::cos(time), std::sin(time), 0.0,
    //   -std::sin(time), std::cos(time), 0.0,
    //   0.0, 0.0, 1.0
    // }) * glm::mat3({
    //   // Move back
    //   1.0, 0.0, 0.0,
    //   0.0, 1.0, 0.0,
    //   -1.0, -1.0, 1.0
    // });

    // Send matrix value to program
    auto transform_uniform = program->GetUniformLocation("Transform");
    glUniformMatrix4fv(transform_uniform, 1, GL_FALSE, glm::value_ptr(my2DShape.createMatrix()));
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return EXIT_FAILURE;
    }

    My2DShape shape;
    shape.points = { {-1.0,1.0}, {-1.0,0.0}, {0.0,1.0}};


    // Setup OpenGL context
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Try to create a window
    auto window = glfwCreateWindow( SIZE, SIZE, "PPGSO gl_transform", nullptr, nullptr);
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
    auto program = ShaderPtr(new Shader{gl_transform_vert, gl_transform_frag});
    program->Use();

    //auto texture = Texture{"lena.rgb", 512, 512};
    //auto quad = Mesh{program, "quad.obj"};

    float time = 0;
    // Main execution loop

    //DOPLNAM:
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, shape.points.size() * sizeof(glm::vec2), shape.points.data(), GL_STATIC_DRAW);

    auto position_attrib = program->GetAttribLocation("Position");
    glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_attrib);

    //

    while (!glfwWindowShouldClose(window)) {
        // Set gray background
        glClearColor(.5f,.5f,.5f,0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shape.rotate(0.001);
        shape.scale(0.001);
        shape.move(0.001, 0.001);

        SetTransformation(program, time+=0.01f, shape);

        shape.draw();

        //program->SetMatrix(glm::mat4(1.0f), "ModelView");
        //quad.Render();

        // Display result
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();

    return EXIT_SUCCESS;
}