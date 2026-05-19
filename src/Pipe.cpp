#include "Pipe.h"
#include "AssetManager.h"
#include <cmath>

PipePair::PipePair(float x, float gapY, float gapSize) 
    : xPos(x), yGap(gapY), gapHeight(gapSize), passed(false), oscillationTimer(0) {
    
    sf::Texture& tex = AssetManager::getInstance().getTexture("pipe");
    
    topPipe.emplace(tex);
    bottomPipe.emplace(tex);

    sf::FloatRect bounds = topPipe->getLocalBounds();
    float targetWidth = 80.0f;
    float scale = targetWidth / bounds.size.x;

    // Top pipe: flip vertically
    topPipe->setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y));
    topPipe->setScale(sf::Vector2f(scale, -scale)); // Negative scale flips it
    
    // Bottom pipe
    bottomPipe->setOrigin(sf::Vector2f(bounds.size.x / 2.0f, 0.0f));
    bottomPipe->setScale(sf::Vector2f(scale, scale));

    // Stems setup using texture repeating
    topStem.emplace(tex);
    bottomStem.emplace(tex);
    
    // Use the middle part of the pipe texture for the stems
    sf::IntRect stemRect({0, (int)bounds.size.y / 4}, {(int)bounds.size.x, (int)bounds.size.y / 2});
    topStem->setTextureRect(stemRect);
    bottomStem->setTextureRect(stemRect);

    topStem->setOrigin(sf::Vector2f(bounds.size.x / 2.0f, 0.0f));
    bottomStem->setOrigin(sf::Vector2f(bounds.size.x / 2.0f, 0.0f));
    
    // Scale stems to match target width and cover the screen
    topStem->setScale(sf::Vector2f(scale, 600.0f / stemRect.size.y));
    bottomStem->setScale(sf::Vector2f(scale, 600.0f / stemRect.size.y));

    // Flip top stem
    topStem->setRotation(sf::degrees(180));

    updateSprites();
}

void PipePair::update(float dt, float speed) {
    xPos -= speed * dt;
    updateSprites();
}

void PipePair::oscillate(float dt, float amplitude, float frequency) {
    oscillationTimer += dt;
    yGap += std::sin(oscillationTimer * frequency) * amplitude * dt;
    updateSprites();
}

bool PipePair::isOffScreen() const {
    return xPos < -100.0f;
}

sf::FloatRect PipePair::getTopBounds() const {
    sf::FloatRect b = topPipe->getGlobalBounds();
    b.position.y -= 600.0f; // Extend upwards
    b.size.y += 600.0f;
    return b;
}

sf::FloatRect PipePair::getBottomBounds() const {
    sf::FloatRect b = bottomPipe->getGlobalBounds();
    b.size.y += 600.0f; // Extend downwards
    return b;
}

bool PipePair::isPassed(float birdX) const {
    return !passed && birdX > xPos;
}

void PipePair::setPassed(bool p) {
    passed = p;
}

void PipePair::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (topStem) target.draw(*topStem, states);
    if (bottomStem) target.draw(*bottomStem, states);
    if (topPipe) target.draw(*topPipe, states);
    if (bottomPipe) target.draw(*bottomPipe, states);
}

void PipePair::updateSprites() {
    float topY = yGap - gapHeight / 2.0f;
    float bottomY = yGap + gapHeight / 2.0f;
    
    topPipe->setPosition(sf::Vector2f(xPos, topY));
    bottomPipe->setPosition(sf::Vector2f(xPos, bottomY));
    
    topStem->setPosition(sf::Vector2f(xPos, topY));
    bottomStem->setPosition(sf::Vector2f(xPos, bottomY));
}

// Add this to header if not present
// void updateSprites();
