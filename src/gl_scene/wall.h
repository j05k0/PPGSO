//
// Created by Miroslav on 09.12.2016.
//

#ifndef PPGSO_WALL_H
#define PPGSO_WALL_H

#include <texture.h>
#include <shader.h>
#include <mesh.h>

#include "object.h"

class Wall : public Object {
public:
    Wall();
    ~Wall();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

private:

    // Static resources (Shared between instances)
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};
typedef std::shared_ptr< Wall > WallPtr;


#endif //PPGSO_WALL_H
