#include "Layer.h"

Layer::Layer(std::string filePath, float speed)
{
	layer.loadFromFile(filePath);
	layer.setRepeated(true);
	sprite.setTexture(layer);
	sprite.setPosition(0, 0);
	layerSpeed = speed;
}

void Layer::resizeToWindow(sf::Vector2u windowSize) {
    sf::Vector2u layerSize = layer.getSize();

    float scaleX = static_cast<float>(windowSize.x) / layerSize.x;
    float scaleY = static_cast<float>(windowSize.y) / layerSize.y;

    sprite.setScale(scaleX, scaleY);
}

void Layer::draw(sf::RenderWindow* window, float deltaTime)
{
	offest += layerSpeed * deltaTime;
	sprite.setTextureRect(sf::IntRect(offest, 0, layer.getSize().x *2, layer.getSize().y));
	window->draw(sprite);
}
