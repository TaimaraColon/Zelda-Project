#include "Map.h"

using namespace std;

void Map::update() {
    shared_ptr<Section> currentSection = this->getCurrentSection();

    // If we are transitioning, nothing else should happen
    if (transitioning){
        currentSection->setTranstioning(true);
        transition(move);
        return;
    }

    // Tell the current section that it is not transitioning
    // This way, the section can update/draw its enemies and items
    currentSection->setTranstioning(false);


    // Do not update the section if link is picking up an item
    // We don't want Link getting bopped by enemies while he's picking up an item
    if (!link->getIsPickingUp()){
        link->update();
        currentSection->update();
    }
    
    else{ // We ARE picking up an item
        // Tick the pick up timer
        if (pickupTimer < PICKUP_TIME)
            pickupTimer++;
        

        // If the timer is up, unpickup
        else{
            pickupTimer = 0;
            link->unPickup();
        }
    }

    checkTransition();
    checkCollision();

}
void Map::draw() {
    ofSetBackgroundColor(0, 0, 0);

    // Draw the background
    this->background.drawSubsection(
        Handler::GAME_SCREEN.x,
        Handler::GAME_SCREEN.y,
        Handler::GAME_SCREEN.width,
        Handler::GAME_SCREEN.height,
        mapX, mapY, Handler::GAME_SCREEN.width, Handler::GAME_SCREEN.height);

    this->getCurrentSection()->draw();
    link->draw();
}

// Moves the map background by deltaX and deltaY
void Map::moveMap(int deltaX, int deltaY) {
    this->mapX += deltaX;
    this->mapY += deltaY;

    if (this->mapX < 0)
        this->mapX = 0;

    else if (this->mapX > this->background.getWidth() - Handler::SECTION_WIDTH)
        this->mapX = this->background.getWidth() - Handler::SECTION_WIDTH;

    if (this->mapY < 0)
        this->mapY = 0;

    else if (this->mapY > this->background.getHeight() - Handler::SECTION_HEIGHT)
        this->mapY = this->background.getHeight() - Handler::SECTION_HEIGHT;
}

// Moves the current section by 1 in the given direction
void Map::moveSection(Direction d) {
    switch (d) {
    case UP:
        if (this->currentSectionY > 0)
            this->currentSectionY -= 1;
        break;

    case DOWN:
        if (this->currentSectionY < this->sections.size() - 1)
            this->currentSectionY += 1;
        break;

    case LEFT:
        if (this->currentSectionX > 0)
            this->currentSectionX -= 1;
        break;

    case RIGHT:
        if (this->currentSectionX < this->sections[0].size() - 1)
            this->currentSectionX += 1;
        break;
    }

    // Revive all enemies in the new section
    this->getCurrentSection()->respawnEnemies();
}

// Transitions the map in the given direction
void Map::transition(Direction direction)
{
    if (direction == UP)
    {
        moveMap(0, -TRANSITION_SPEED);
        getPlayer()->setY(getPlayer()->getY() + TRANSITION_SPEED);

        if (pixelsMoved >= Handler::SECTION_HEIGHT)
        {
            moveSection(Direction::UP);
            mapY = currentSectionY * Handler::SECTION_HEIGHT;
            getPlayer()->setY(Handler::GAME_SCREEN.getBottom() - getPlayer()->getHeight() - TRANSITION_SPEED);
            pixelsMoved = 0;
            transitioning = false;
        }
    }
    else if (direction == DOWN)
    {
        moveMap(0, TRANSITION_SPEED);
        getPlayer()->setY(getPlayer()->getY() - TRANSITION_SPEED);

        if (pixelsMoved >= Handler::SECTION_HEIGHT)
        {
            moveSection(Direction::DOWN);
            mapY = currentSectionY * Handler::SECTION_HEIGHT;
            getPlayer()->setY(Handler::GAME_SCREEN.getTop() + TRANSITION_SPEED);
            pixelsMoved = 0;
            transitioning = false;
        }
    }
    else if (direction == LEFT)
    {
        moveMap(-TRANSITION_SPEED, 0);
        getPlayer()->setX(getPlayer()->getX() + TRANSITION_SPEED);

        if (pixelsMoved >= Handler::SECTION_WIDTH)
        {
            moveSection(Direction::LEFT);
            mapX = currentSectionX * Handler::SECTION_WIDTH;
            pixelsMoved = 0;
            getPlayer()->setX(Handler::GAME_SCREEN.getRight() - getPlayer()->getWidth() - TRANSITION_SPEED);
            transitioning = false;
        }
    }
    else
    {
        moveMap(TRANSITION_SPEED, 0);
        getPlayer()->setX(getPlayer()->getX() - TRANSITION_SPEED);

        if (pixelsMoved >= Handler::SECTION_WIDTH)
        {
            moveSection(Direction::RIGHT);
            mapX = currentSectionX * Handler::SECTION_WIDTH;
            pixelsMoved = 0;
            getPlayer()->setX(Handler::GAME_SCREEN.getLeft() + TRANSITION_SPEED);
            transitioning = false;
        }
    }

    pixelsMoved += TRANSITION_SPEED;
}

// Checks if the player is colliding with a border, triggering a transition if so
void Map::checkTransition(){
    if (getPlayer()->getBounds().getRight() >= Handler::GAME_SCREEN.getRight())
    {
        transitioning = true;
        move = RIGHT;
    }
    else if (getPlayer()->getBounds().getLeft() <= Handler::GAME_SCREEN.getLeft())
    {
        transitioning = true;
        move = LEFT;
    }
    else if (getPlayer()->getBounds().getTop() <= Handler::GAME_SCREEN.getTop())
    {
        transitioning = true;
        move = UP;
    }
    else if (getPlayer()->getBounds().getBottom() >= Handler::GAME_SCREEN.getBottom())
    {
        transitioning = true;
        move = DOWN;
    }
}

void Map::checkCollision(){
    shared_ptr<Section> currentSection = this->getCurrentSection();

    // Block w/ Entity Collision
    for (auto row : currentSection->getBlocks()){
        for (auto block : row){
            if (block == NULL) continue;

            // Check if Link is colliding with a block
            if (block->collidesWith(link)){
                shared_ptr<DoorBlock> door = dynamic_pointer_cast<DoorBlock>(block);

            if (door) {
                this->setFinished(true);
                this->setWhereTo(door->getWhereTo());
            }

            else
                block->onCollision(link);
            }
            // Check if an enemy is colliding with this same block
            for (auto enemy : currentSection->getEnemies()){
                if (block->collidesWith(enemy))
                    block->onCollision(enemy);
            }
        }
    }
}

void Map::keyPressed(int key) {
    link->keyPressed(key);
}

void Map::keyReleased(int key) {
    link->keyReleased(key);
}