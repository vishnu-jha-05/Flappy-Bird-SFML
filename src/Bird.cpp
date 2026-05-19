#include "Bird.h"
#include "AssetManager.h"
#include <cmath>

Bird::Bird() : velocity(0) {
    sprite.emplace(AssetManager::getInstance().getTexture("bird"));
    // Center origin
    sf::FloatRect bounds = sprite->getLocalBounds();
    sprite->setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
    
    // Scale to target height (e.g., 50 pixels)
    float targetHeight = 50.0f;
    float scale = targetHeight / bounds.size.y;
    sprite->setScale(sf::Vector2f(-scale, scale)); // Flip to face right
}

void Bird::update(float dt) {
    velocity += gravity * dt;
    move(sf::Vector2f(0.0f, velocity * dt));

    // Smooth rotation
    float rotation = velocity * 0.1f;
    if (rotation > 30.0f) rotation = 30.0f;
    if (rotation < -30.0f) rotation = -30.0f;
    sprite->setRotation(sf::degrees(rotation));
}

void Bird::flap() {
    velocity = jumpForce;
}

void Bird::reset(float x, float y) {
    setPosition(sf::Vector2f(x, y));
    velocity = 0;
    sprite->setRotation(sf::degrees(0));
}

sf::FloatRect Bird::getBounds() const {
    return getTransform().transformRect(sprite->getGlobalBounds());
}

void Bird::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    if (sprite) target.draw(*sprite, states);
}
