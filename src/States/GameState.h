#pragma once

#include "Animation.h"
#include "Link.h"
#include "Sword.h"
#include "Tektike.h"
#include "knight.h"
#include "MapBuilder.h"
#include "State.h"
#include "Entity.h"
#include "Map.h"
#include "Item.h"
#include <memory>


class GameState : public State {

private:
    shared_ptr<Map> currentMap;
    shared_ptr<Map> overWorldMap;
    shared_ptr<Map> oldManCaveMap;
    unique_ptr<MapBuilder> mapBuilder;
    shared_ptr<Sword> createSword;
    shared_ptr<Map> dungeonMap;
   
public:
    GameState(shared_ptr<Handler> handler) : State(handler) { this->reset(); }
    void update();
    void draw();
    void reset();
    void keyPressed(int key);
    void keyReleased(int key);
    ofImage menuHUD;
    bool paused;
    int hudY;
    int hudH;
    int hudHeight;
    int changeState;
    bool pickedUp=false;
    

    shared_ptr<Tektike> createTektike();
    shared_ptr<Knight> createKnight();
    shared_ptr<Knight> knight;
    shared_ptr<Knight> tektike;
    
    void pickUpSword();
    void tektikeattack();
    void linkattack();
    
};