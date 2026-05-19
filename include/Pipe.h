#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class PipePair : public sf::Drawable {
public:
    PipePair(float x, float gapY, float gapSize);
    void update(float dt, float speed);
    bool isOffScreen() const;
    sf::FloatRect getTopBounds() const;
    sf::FloatRect getBottomBounds() const;
    bool isPassed(float birdX) const;
    void setPassed(bool passed);

    // For hard mode
    void oscillate(float dt, float amplitude, float frequency);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateSprites();

    std::optional<sf::Sprite> topPipe;
    std::optional<sf::Sprite> bottomPipe;
    std::optional<sf::Sprite> topStem;
    std::optional<sf::Sprite> bottomStem;
    float xPos;
    float yGap;
    float gapHeight;
    bool passed;
    float oscillationTimer;
};
