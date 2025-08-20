#pragma once

#include "Item.h"
#include "Link.h"
#include <memory>

class Sword: public Item{

    public: 
    Sword(int x, int y, shared_ptr<Handler> handler, ofImage sprite ) : Item(x, y, handler, sprite){
        this->currentSprite = sprite;
    }
    void onCollision(shared_ptr<DynamicEntity> e) {}
    void applyEffect(shared_ptr<Link> link) {}
    ~Sword(){};
    
};