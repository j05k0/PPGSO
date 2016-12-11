// Example gl_gradient
// - Demonstrates the use of a dynamically generated gradient in GLSL
// - Displays the gradient on a quad using OpenGL

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "gl_gradient_vert.h"
#include "gl_gradient_frag.h"

const unsigned int SIZE = 512;
#define PI 3.14159265

struct Point {
    float x;
    float y;
};

Point lerp(Point A, Point B, float t){
  Point temp;
  temp.x = A.x + ((B.x - A.x) * t);
  temp.y = A.y + ((B.y - A.y) * t);
  return temp;
}

void Bezier(std::vector<GLfloat> &out, std::vector<Point> in, float N){
  Point A,B,C,X,Y,P;
  float dt = 1/N;
  int j = 0;
  for(int i = 0; i < (in.size() - 1) / 3; i++) {
    for (float t = 0; t <= 1; t += dt) {
      A = lerp(in[j], in[j + 1], t);
      B = lerp(in[j + 1], in[j + 2], t);
      C = lerp(in[j + 2], in[j + 3], t);
      X = lerp(A, B, t);
      Y = lerp(B, C, t);
      P = lerp(X, Y, t);
      out.push_back(P.x);
      out.push_back(P.y);
    }
    j += 3;
  }
}

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW!" << std::endl;
    return EXIT_FAILURE;
  }

    std::vector<Point> in;
    std::vector<GLfloat> out;
    int alpha = -45, i;
    for (i = 0; i < 4; i++) {
        Point tmp;
        tmp.x = (100 * sin(alpha*PI/180) + SIZE/2) / SIZE;
        tmp.y = (-100* cos(alpha*PI/180) + SIZE/2) / SIZE;
        alpha =  (alpha + 90) % 360;
        in.push_back(tmp);
    }

    Point start, end;
    start = in[0];
    end = in[3];
    for(i = 0; i < 3; i++){
        Point tmp;
        tmp.x = end.x + ((start.x - end.x) * ((i + 1) / 3));
        tmp.y = end.y + ((start.y - end.y) * ((i + 1) / 3));
        in.push_back(tmp);
    }

    Bezier(out, in, 1000000);

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
  auto program = ShaderPtr(new Shader{gl_gradient_vert, gl_gradient_frag});
  program->Use();

  // Generate a vertex array object
  // This keeps track of what attributes are associated with buffers
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

    /*
  // Setup geometry
  std::vector<GLfloat> vertex_buffer{
          // x, y
          1.0f, 1.0f,
          -1.0f, 1.0f,
          1.0f, -1.0f,
          -1.0f, -1.0f
  };*/

  // Generate a vertex buffer object, this will feed data to the vertex shader
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  //glBufferData(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(GLfloat), vertex_buffer.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, out.size() * sizeof(GLfloat), out.data(), GL_STATIC_DRAW);

  // Setup vertex array lookup, this tells the shader how to pick data for the "Position" input
  auto position_attrib = program->GetAttribLocation("Position");
  glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(position_attrib);

    /*
  // Colors for vertices
  std::vector<GLfloat> color_buffer{
          // r, g, b
          1.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f,
          0.0, 0.0f, 1.0f,
          1.0f, 1.0f, 1.0f
  };*/

  GLuint cbo;
  glGenBuffers(1, &cbo);
  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  //glBufferData(GL_ARRAY_BUFFER, color_buffer.size() * sizeof(GLfloat), color_buffer.data(), GL_STATIC_DRAW);
/*
  // Same thing for colors
  auto color_attrib = program->GetAttribLocation("Color");
  glVertexAttribPointer(color_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(color_attrib);*/

  // Main execution loop
  while (!glfwWindowShouldClose(window) ) {
    // Set gray background
    glClearColor(.8941,.3451,.9255,0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw triangles using the program
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) (out.size() / 2));

    // Display result
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  glfwTerminate();

  return EXIT_SUCCESS;
}
