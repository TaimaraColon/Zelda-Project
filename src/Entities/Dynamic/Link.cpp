#include "Link.h"
#include "ImageManager.h"

void Link::update() {
    DynamicEntity::update();

    if (dyingTimer > 0)
        dyingTimer--;

    if (dead)
        return;

    if (this->health <= 2)
        this->handler->getSoundEffectManager()->playSoundEffect("link_low_health"); 

    
    if (attacking)
    {
        if (currentAnimation->hasEnded())
        {
            // Reset the sword hitbox
            swordHitbox = ofRectangle(0, 0, 0, 0);
            attacking = false;
            currentAnimation->reset();
            if (currentAnimation == getAnimations()->attacking.up)
                this->currentAnimation = getAnimations()->movement.up;
            else if (currentAnimation == getAnimations()->attacking.down)
                this->currentAnimation = getAnimations()->movement.down;
            else if (currentAnimation == getAnimations()->attacking.left)
                this->currentAnimation = getAnimations()->movement.left;
            else if (currentAnimation == getAnimations()->attacking.right)
                this->currentAnimation = getAnimations()->movement.right;
        }
    }
   
}

void Link::draw() {
    
    if (attacking) {
        // The goal is to draw the sword on top of link, offset by the swordOffsets array
        int offset = swordOffsets[currentAnimation->getIndex()];

        // Because the sword is not centered in the 16x16 square, we have to
        // offset the sword by a few pixels to make it look right

        // The sword is offset by 2 pixels to the left when facing up, 1 pixel to the right when facing down
        // 2 pixels down when facing left, and 1 pixel down when facing right
        int xOffset = 1;
        int yOffset = 1;

        // Determine the direction the sword should be drawn in
        Direction swordDirection = UP;

        if (currentAnimation == getAnimations()->attacking.up) {
            swordDirection = UP;
            yOffset = -offset;
            xOffset = -2; // Funny special case
        } else if (currentAnimation == getAnimations()->attacking.down) {
            swordDirection = DOWN;
            yOffset = offset;
        } else if (currentAnimation == getAnimations()->attacking.left) {
            swordDirection = LEFT;
            xOffset = -offset;
            yOffset = 2; // Funny special case
        } else if (currentAnimation == getAnimations()->attacking.right) {
            swordDirection = RIGHT;
            xOffset = offset;
        }

        // Get the sword image
        ofImage sword = handler->getImageManager()->basicSword;

        // Rotate the sword if necessary.
        // By default, the sword is facing up
        if (swordDirection == DOWN)
            sword.rotate90(2);
        else if (swordDirection == LEFT)
            sword.rotate90(-1);
        else if (swordDirection == RIGHT)
            sword.rotate90(-3);

        // Calculate the sword hitbox
        swordHitbox = ofRectangle(x + xOffset * Handler::PIXEL_SCALE, y + yOffset * Handler::PIXEL_SCALE, width, height);

        // Draw the sword
        sword.draw(swordHitbox);
    }

    DynamicEntity::draw();

    if (Handler::DEBUG){
        ofSetColor(255,0,0);
        ofSetLineWidth(Handler::PIXEL_SCALE);
        ofNoFill();
        ofDrawRectangle(swordHitbox);
        ofFill();
        ofSetColor(255, 255, 255);
    }
}

void Link::move(Direction direction) {
    // Link can only move if he is not attacking
    if (!attacking)
        DynamicEntity::move(direction);
}

void Link::damage(int damage) {
    if (dead)
        return;

    // Play the damage sound if we're about to take damage
    if (invincibilityFrames == 0)
        handler->getSoundEffectManager()->playSoundEffect("link_hurt");

    // Should lower health and give invincibility frames
    DynamicEntity::damage(damage);
}

void Link::onDeath() {
    // Play the death sound
    handler->getSoundEffectManager()->playSoundEffect("link_death");
    handler->getMusicManager()->stopMusic();
    currentAnimation = getAnimations()->death;
    dyingTimer = 150;
}
void Link::onCollision(shared_ptr<DynamicEntity> e) {
    if (dead)
        return;
}

void Link::drawHearts(int hudHeight) {

    // Handles everything regarding hearts
    ofImage fullHeart = handler->getImageManager()->fullHeart;
    ofImage halfHeart = handler->getImageManager()->halfHeart;
    ofImage emptyHeart = handler->getImageManager()->emptyHeart;
    int acc = 0;

    // Draws the Full Heart
    for (int i = 0; i < health / 2; i++)
    {
        if (i <= 7 || health <= 7)
        {
            fullHeart.draw((4 * ofGetWidth()) / 6 + (ofGetWidth()/32 * i), hudHeight - ofGetHeight()/8, Handler::TILE_WIDTH, Handler::TILE_HEIGHT); // Row 1
        }

        if (i >= 8 && i <= 16)
        {
            fullHeart.draw((4 * ofGetWidth()) / 6 + (ofGetWidth()/32 * (i - 8)), hudHeight - ofGetHeight()/12, Handler::TILE_WIDTH, Handler::TILE_HEIGHT); // Row 2
        }
        acc = i + 1;
    }

    // Draws the Half Heart
    if (health % 2 == 1)
    {
        if (health <= 16)
        {
            halfHeart.draw((4 * ofGetWidth()) / 6 + (ofGetWidth()/32 * acc), hudHeight - ofGetHeight()/8, Handler::TILE_WIDTH, Handler::TILE_HEIGHT);
        }

        else if (health <= 32)
        {
            halfHeart.draw((4 * ofGetWidth()) / 6 + (ofGetWidth()/32 * (acc - 8)), hudHeight - ofGetHeight()/12, Handler::TILE_WIDTH, Handler::TILE_HEIGHT);
        }
    }

    // Draws the Remainder Hearts
    //  (health + 1 )/2 gives us the starting position, we loop until the maxHealth/2 which is the max position
    for (int i = (health + 1) / 2; i < MAX_HEALTH / 2; i++)
    {
        if (i <= 7)
        {
            emptyHeart.draw((4 * ofGetWidth()) / 6 + (ofGetWidth()/32 * i),hudHeight - ofGetHeight()/8, Handler::TILE_WIDTH, Handler::TILE_HEIGHT);
        }

        if (i >= 8 && i <= 16)
        {
            emptyHeart.draw((4 * ofGetWidth()) / 6 + (ofGetWidth()/32 * (i - 8)), hudHeight - ofGetHeight()/12, Handler::TILE_WIDTH, Handler::TILE_HEIGHT);
        }
    }
}

void Link::keyPressed(int key) {
    if (!pause){
        if (this->getIsPickingUp() || dead ) return;
        
        if (key == 'w')
            this->movingDir[UP] = true;
        
        else if (key == 's')
            this->movingDir[DOWN] = true;

        else if (key == 'a')
            this->movingDir[LEFT] = true;

        else if (key == 'd')
            this->movingDir[RIGHT] = true;

        else if (key == ' ' || key == 'b') {

                //if link doesn't have the sword, return 
                if (attacking || dead || !swordCollected)
                return;
            
                this->attacking = true;
            if (currentAnimation == getAnimations()->movement.up)
                this->currentAnimation = getAnimations()->attacking.up;
            else if (currentAnimation == getAnimations()->movement.down)
                this->currentAnimation = getAnimations()->attacking.down;
            else if (currentAnimation == getAnimations()->movement.left)
                this->currentAnimation = getAnimations()->attacking.left;
            else if (currentAnimation == getAnimations()->movement.right)
                this->currentAnimation = getAnimations()->attacking.right;

            // Play the sword sound
            handler->getSoundEffectManager()->playSoundEffect("link_sword_attack");
            }
            
        // }
    }
}

void Link::keyReleased(int key) {
    if (key == 'w')
        this->movingDir[UP] = false;

    else if (key == 's')
        this->movingDir[DOWN] = false;

    else if (key == 'a')
        this->movingDir[LEFT] = false;

    else if (key == 'd')
        this->movingDir[RIGHT] = false;
}