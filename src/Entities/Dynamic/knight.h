#pragma once

#include "DynamicEntity.h"
#include "Link.h"

class Knight : public DynamicEntity {
    private:
        int counter;
        const int WAITING_TIME = 60;
        const int MOVEMENT_TIME = 30;

    public: 
        int linkX = 0;
        int linkY = 0;
        Knight(int x, int y, shared_ptr<Handler> handler, shared_ptr<DynamicEntityAnimations> animations) : DynamicEntity(x, y, handler, animations) {
            this->MAX_HEALTH = 1;
            this->health = MAX_HEALTH;
            this->counter = 0;
        };

        void update();
        void damage(int damage);
        void onDeath();
        void onCollision(shared_ptr<DynamicEntity> entity){};
};