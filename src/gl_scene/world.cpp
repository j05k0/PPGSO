#include "world.h"
#include "scene.h"

#include "space_vert.h"
#include "space_frag.h"

World::World() {
  offset = 0;
  // Z of 1 means back as there is no perspective projection applied during render
  position.z = 1;

  // Initialize static resources if needed
  if (!shader) shader = ShaderPtr(new Shader{space_vert, space_frag});
  if (!texture) texture = TexturePtr(new Texture{"backround.rgb", 512, 512});
  if (!mesh) mesh = MeshPtr(new Mesh{shader, "quad.obj"});
}

World::~World() {
}

bool World::Update(Scene &scene, float dt) {
  // Offset for UV mapping, creates illusion of scrolling
  //offset -= dt/5;

  GenerateModelMatrix();
  return true;
}

void World::Render(Scene &scene) {
  // NOTE: this object does not use camera, just renders the entire quad as is
  shader->Use();

  // Pass UV mapping offset to the shader
  shader->SetFloat(offset, "Offset");

  // Render mesh
  shader->SetMatrix(modelMatrix, "ModelMatrix");
  shader->SetTexture(texture, "Texture");
  mesh->Render();
}

// Static resources
MeshPtr World::mesh;
ShaderPtr World::shader;
TexturePtr World::texture;
