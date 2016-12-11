#include "player.h"
#include "scene.h"
#include "object_frag.h"
#include "object_vert.h"
#include "wall.h"
#include "food.h"
#include <GLFW/glfw3.h>

Player::Player() {

  // Rotate the default model
  //scale *= 2.5f;
  rotation.x = PI/5.0f;
  rotation.y = PI/2.0f;
  rotation.z = 0.0f;

    score = 0;
    radius = .9135f * scale.y;

  // Initialize static resources if needed
  if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
  if (!texture) texture = TexturePtr(new Texture{"pacman.rgb", 512, 512});
  if (!mesh) mesh = MeshPtr(new Mesh{shader, "pacman.obj"});
}

Player::~Player() {
}

bool Player::Update(Scene &scene, float dt) {

    if(scene.keyboard[GLFW_KEY_LEFT] && position.x <= 7.5f) {

        if(CollisionDetection(scene)){
            position.x += 10 * dt;
        }
        rotation.y = PI/0.5f;
      printf("Position X is %f\n", position.x);
      printf("Position Y is %f\n", position.y);
    } else if(scene.keyboard[GLFW_KEY_RIGHT] && position.x > -7.5f) {
        if(CollisionDetection(scene)){
            position.x -= 10 * dt;
        }
        rotation.y = PI;
      printf("Position X is %f\n", position.x);
      printf("Position Y is %f\n", position.y);
    } else if(scene.keyboard[GLFW_KEY_UP] && position.y <= 7.5f) {
        position.y += 10 * dt;
        rotation.y = PI/2.0f;
//      printf("Position X is %f\n", position.x);
//      printf("Position Y is %f\n", position.y);
    }else if(scene.keyboard[GLFW_KEY_DOWN] && position.y > -7.5f) {
        position.y -= 10 * dt;
        rotation.y = -PI/2.0f;
//      printf("Position X is %f\n", position.x);
//      printf("Position Y is %f\n", position.y);
    }

  GenerateModelMatrix();
  return true;
}

bool Player::CollisionDetection(Scene &scene){
    for ( auto obj : scene.objects ) {
        if (obj.get() == this)
            continue;

        auto wall = std::dynamic_pointer_cast<Wall>(obj);
        if(wall){
            if(glm::distance(position, wall->position) < (wall->radius + radius)){
                return false;
            }
        }
    }
    return true;
}

void Player::Render(Scene &scene) {
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
MeshPtr Player::mesh;
ShaderPtr Player::shader;
TexturePtr Player::texture;