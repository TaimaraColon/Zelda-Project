#include "Knight.h"

void Knight::update(){
    
    counter++;

    if (movingDir[UP] || movingDir[DOWN] || movingDir[LEFT] || movingDir[RIGHT]){
        if (counter == MOVEMENT_TIME){
            movingDir[UP] = false;
            movingDir[DOWN] = false;
            movingDir[LEFT] = false;
            movingDir[RIGHT] = false;
            counter = 0;
        }
    }

    else{
        if (counter == WAITING_TIME){
            
            if(this->getX() < linkX){
                movingDir[RIGHT]=true;
            }
            if(this->getX() > linkX){
                movingDir[LEFT]=true;
            }
            if(this->getY() > linkY){
                movingDir[UP]=true;
            }
            if(this->getY() < linkY){
                movingDir[DOWN]=true;
            }
            counter = 0;
        }
    }

    DynamicEntity::update();
}

void Knight::damage(int damage){
    if (dead) return;
   
    if (invincibilityFrames == 0)
        handler->getSoundEffectManager()->playSoundEffect("enemy_hurt");
        
    DynamicEntity::damage(damage);
}

void Knight::onDeath(){
    handler->getSoundEffectManager()->playSoundEffect("enemy_death");
}