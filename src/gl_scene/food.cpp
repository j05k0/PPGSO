//
// Created by Miroslav on 10.12.2016.
//

#include "food.h"
#include "scene.h"

#include "object_frag.h"
#include "object_vert.h"
#include "player.h"
#include "explosion.h"
#include "generator.h"

#include <GLFW/glfw3.h>

Food::Food() {

    scale *= 0.3f;
    isDead = false;

    // Initialize static resources if needed
    if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
    if (!texture) texture = TexturePtr(new Texture{"white.rgb", 512, 512});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "food.obj"});

}

Food::~Food() {
}

bool Food::Update(Scene &scene, float dt) {

    if(isDead){
        return false;
    }

    for ( auto obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        auto player = std::dynamic_pointer_cast<Player>(obj);
        auto food = std::dynamic_pointer_cast<Food>(obj);
        auto generator = std::dynamic_pointer_cast<Generator>(obj);
        if(player){
            if(glm::distance(position, player->position) < player->scale.z){
                player->score++;
                scene.numberOfFood--;
                printf("Score is %d\n", player->score);
                return false;
            }
        }
        if(food){
            if(glm::distance(position, food->position) < (food->scale.x)*2){
                // destroy food a nie this
                food->isDead = true;
                scene.numberOfFood--;
            }
        }
    }

    GenerateModelMatrix();
    return true;
}

void Food::Render(Scene &scene) {
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
MeshPtr Food::mesh;
ShaderPtr Food::shader;
TexturePtr Food::texture;