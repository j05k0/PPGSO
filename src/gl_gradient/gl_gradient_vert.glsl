#version 150
// Get vertex position from the vertex buffer
in vec3 Position;
in vec2 TexCoords;
//in vec3 Color;

// This will be passed on to the fragment shader
//out vec3 VertexColor;
out vec2 FragTexCoords;

void main() {
 // VertexColor = Color;
    FragTexCoords = TexCoords;
  // The vertex shader needs to output a vec4 gl_Position so we fill in z and w to comply
  gl_Position = vec4(Position, 1.0);
}
