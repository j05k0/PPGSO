//
// Created by Miroslav on 09.12.2016.
//

#include "wall.h"
#include "scene.h"

#include "object_frag.h"
#include "object_vert.h"

#include <GLFW/glfw3.h>

Wall::Wall() {



//    rotation.x = PI/1.0f;
//    rotation.y = PI/2.0f;
//    rotation.z = PI/2.0f;

    radius = 1.0f * scale.x;

    // Initialize static resources if needed
//    if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
//    if (!texture) texture = TexturePtr(new Texture{"wall.rgb", 512, 512});
//    if (!mesh) mesh = MeshPtr(new Mesh{shader, "wall.obj"});

    if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
    if (!texture) texture = TexturePtr(new Texture{"white.rgb", 512, 512});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "cube.obj"});
}

Wall::~Wall() {
}

bool Wall::Update(Scene &scene, float dt) {

    GenerateModelMatrix();
    return true;
}

void Wall::Render(Scene &scene) {
    shader->Use();

    // use camera
    shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
    shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

    // render mesh
    shader->SetMatrix(modelMatrix, "ModelMatrix");
    shader->SetTexture(texture, "Texture");
    mesh->Render();
}

// shared resources
MeshPtr Wall::mesh;
ShaderPtr Wall::shader;
TexturePtr Wall::texture;