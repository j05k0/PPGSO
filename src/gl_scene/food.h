//
// Created by Miroslav on 10.12.2016.
//

#ifndef PPGSO_FOOD_H
#define PPGSO_FOOD_H

#include <texture.h>
#include <shader.h>
#include <mesh.h>

#include "object.h"

class Food : public Object {
public:
    Food();
    ~Food();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

    bool isDead;

private:

    // Static resources (Shared between instances)
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};
typedef std::shared_ptr< Food > FoodPtr;

#endif //PPGSO_FOOD_H
