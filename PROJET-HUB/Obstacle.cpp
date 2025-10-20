#include "Obstacle.h"


// Constructor to create an obstacle at a specific position
Obstacle::Obstacle(sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
    : animation(texture, imageCount, switchTime)
{
    this->texture = texture;
    obstacleSprite.setTexture(*texture);
    obstacleSprite.setScale(1.0f, 1.0f);
    
    float groundLevel = Window_H - 68;
    float size_texture = texture->getSize().y / imageCount.y;
    obstacleSprite.setPosition(Window_W, groundLevel - size_texture);
}

void Obstacle::update(float deltaTime, float speed) {
    obstacleSprite.move(-speed * deltaTime, 0);

    animation.Update(0, deltaTime);  // ligne 0, ou change selon les lignes
    obstacleSprite.setTextureRect(animation.uvRect);
}


void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(obstacleSprite);
}

sf::FloatRect Obstacle::getGlobalBounds(){
    return obstacleSprite.getGlobalBounds();
}

void Obstacle::setPosition(float x, float y) {
    obstacleSprite.setPosition(x, y); // Assuming the obstacle is represented by an `sf::RectangleShape` or similar
}

