//
// Created by luis.reis on 08-04-2018.
//

#ifndef ORESCLONE_RESOURCE_H
#define ORESCLONE_RESOURCE_H


#include "Entity.h"
#include "../Level/TileType.h"

class Resource : public Entity {

public:
    TileTypeRef tile;

    explicit Resource(Level &_level, TileTypeRef tile);
    ~Resource() override = default;

    void tick(float delta) override;
    void render() override;

};

#endif //ORESCLONE_RESOURCE_H
