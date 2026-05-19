#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Bird : public sf::Drawable, public sf::Transformable {
public:
    Bird();
    void update(float dt);
    void flap();
    void reset(float x, float y);
    sf::FloatRect getBounds() const;
    void setRotation(float angle);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::optional<sf::Sprite> sprite;
    float velocity;
    const float gravity = 800.0f;
    const float jumpForce = -350.0f;
};
