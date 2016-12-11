#include "generator.h"
#include "food.h"
#include "player.h"

bool Generator::Update(Scene &scene, float dt) {
  // Accumulate time
  time += dt;

  // Add object to scene when time reaches certain level
  if (time > .01 && scene.numberOfFood < 10) {
    auto obj = FoodPtr(new Food());
    obj->position.x = Rand(-7.5f, 7.5f);
    obj->position.y = Rand(-5.0f, 7.5f);
    scene.objects.push_back(obj);
    time = 0;
    scene.numberOfFood++;
  }

  return true;
}

void Generator::Render(Scene &scene) {
  // Generator will not be rendered
}

Generator::~Generator() {
}

Generator::Generator() {
  time = 0;
}
