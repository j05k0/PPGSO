#version 150
// A texture is expected as program attribute
uniform sampler2D Texture;

// The vertex shader fill feed this input
in vec2 FragTexCoord;

// The final color
out vec4 FragmentColor;

// task 4: convolution filter using GLSL
void main() {
    float convolution[25] =
    {
      -1, -1, -1, -1, -1,
      -1,  2,  2,  2, -1,
      -1,  2,  8,  2, -1,
      -1,  2,  2,  2, -1,
      -1, -1, -1, -1, -1,
    };
    float factor = 5.0;
    float bias = 0.0;
   vec4 tempColor;
   float pixel = 1/512.0;
   int i,j;
      // Lookup the color in Texture on coordinates given by fragTexCoord
    for(i = -2; i <= 2; i++){
      for(j = -2; j <= 2; j++){

            tempColor = tempColor + texture(Texture,FragTexCoord + vec2(j*pixel,i*pixel)) * convolution[(i+2)*5+(j+2)] + bias;
      }
    }
    FragmentColor = tempColor/factor;
    //FragmentColor = texture(Texture,FragTexCoord);
}
