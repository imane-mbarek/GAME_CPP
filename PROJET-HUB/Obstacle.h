#pragma once
#include <SFML/Graphics.hpp>
#include "definition.h"
#include "Animation.h" 

class Obstacle {
    sf::Sprite obstacleSprite;
    Animation animation;  // <== NOUVEAU
    sf::Texture* texture; // pour garder une référence si besoin

public:
    Obstacle(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);

    void update(float deltaTime, float speed);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds();
    void setPosition(float x, float y);
};