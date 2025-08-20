#include "Section.h"

Section::Section() {

    for (int y = 0; y < Handler::VERTICAL_TILE_COUNT; y++) {
        vector<shared_ptr<StaticEntity>> currBlocks;
        vector<shared_ptr<Item>> currItems;
        // fill funny with x nullptrs
        for (int x = 0; x < Handler::HORIZONTAL_TILE_COUNT; x++) {
            currBlocks.push_back(nullptr);
            currItems.push_back(nullptr);
        }

        blocks.push_back(currBlocks);
        items.push_back(currItems);
    }
};
void Section::update() {
    // Tick all enemies
    for (shared_ptr<DynamicEntity> enemy : enemies){
        if (!enemy->isDead()){
             enemy->update();
        }
    }
        
    // Items don't need to be ticked unless
    // they have an animation

    // Blocks don't need to be ticked unless
    // they have an animation
}

void Section::draw(){

    // If DEBUG mode is on, draw the hitboxes of all blocks
    if (Handler::DEBUG){
        for(int i = 0; i < blocks.size(); i++){
            for(int j = 0; j < blocks[i].size(); j++){
                shared_ptr<StaticEntity> block = blocks[i][j];
                if(blocks[i][j] != NULL){
                    ofNoFill();
                    ofSetLineWidth(Handler::PIXEL_SCALE);     
                    if (dynamic_pointer_cast<DoorBlock>(block) != nullptr)
                        ofSetColor(ofColor::purple);
                    
                    else
                        ofSetColor(ofColor::white);

                    ofDrawRectangle(blocks[i][j]->getBounds());
                    //Clear the color
                    ofSetColor(255,255,255);
                }
            }
        }
    }
    if(!transtioning){
        for (int i = 0; i < items.size(); i++) {
            for (int j = 0; j < items[i].size(); j++) {
                //checks if i haven't collected the sword yet ---> if that's the case then keep drawing it
                if (items[i][j] != NULL && !items[i][j]->isCollected)
                    items[i][j]->draw();
            }
        }

        for (int i = 0; i < enemies.size(); i++){
            if (enemies[i] != NULL && !enemies[i]->isDead())
                enemies[i]->draw();
        }
    }
}

void Section::respawnEnemies() {
    for (shared_ptr<DynamicEntity> enemy : enemies){
        enemy->revive();
    }
}