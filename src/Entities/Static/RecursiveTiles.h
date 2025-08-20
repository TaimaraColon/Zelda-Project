#pragma once

#include "StaticEntity.h"
#include "Link.h"
#include "MapBuilder.h"
#include <memory>

class RecursiveTiles : public StaticEntity {
    private:
        string direction;

public:
    
    RecursiveTiles(int x, int y, shared_ptr<Handler> handler, string direction) : StaticEntity(x, y, handler) {
        //direction is added to know where the arrow is pointing at
        this->direction = direction;
    }
    ~RecursiveTiles(){};
    void onCollision(shared_ptr<DynamicEntity> e);
};