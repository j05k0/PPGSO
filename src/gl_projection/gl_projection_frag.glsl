#version 150
// A texture is expected as program attribute
uniform sampler2D Texture;

// The vertex shader fill feed this input
in vec2 FragTexCoord;

// The final color
out vec4 FragmentColor;

void main() {
  // Lookup the color in Texture on coordinates given by fragTexCoord
  FragmentColor = texture(Texture, FragTexCoord);
  //FragmentColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
