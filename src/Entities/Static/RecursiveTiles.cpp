#include "RecursiveTiles.h"

void RecursiveTiles::onCollision(shared_ptr<DynamicEntity> e){
    //base case ---> checks if link is not colliding with a tile anymore; if that's the case then stop moving him
    if(this->collidesWith(e)==false) return;

    //if the tile is colliding with link then check for the direction of each arrow and move his position appropiately
    if(this->collidesWith(e)){
        if(this->direction == "up"){
            e->setY(e->getY()-1);
            onCollision(e);         //calls the function ---> it continues to set link's new position as long as it keeps colliding with an arrow tile
        }
       
       if(this->direction == "down"){
            e->setY(e->getY()+1);
            onCollision(e);
        }

        if(this->direction == "right"){
            e->setX(e->getX()+1);
            onCollision(e);
        }

        if(this->direction == "left"){
            e->setX(e->getX()-1);
            onCollision(e);
        }

    }
}

