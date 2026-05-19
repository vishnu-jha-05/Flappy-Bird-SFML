#include "AssetManager.h"
#include <iostream>
#include <algorithm>

AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

void AssetManager::loadTexture(const std::string& name, const std::string& filename) {
    sf::Image image;
    if (image.loadFromFile(filename)) {
        if (name != "background") processImage(image);
        sf::Texture texture;
        if (texture.loadFromImage(image)) {
            if (name == "pipe") texture.setRepeated(true);
            textures[name] = texture;
        }
    } else {
        std::cerr << "Error loading texture: " << filename << std::endl;
    }
}

sf::Texture& AssetManager::getTexture(const std::string& name) {
    return textures.at(name);
}

bool AssetManager::hasTexture(const std::string& name) const {
    return textures.find(name) != textures.end();
}

void AssetManager::loadFont(const std::string& name, const std::string& filename) {
    sf::Font font;
    if (font.openFromFile(filename)) {
        fonts[name] = font;
    } else {
        std::cerr << "Error loading font: " << filename << std::endl;
    }
}

sf::Font& AssetManager::getFont(const std::string& name) {
    return fonts.at(name);
}

bool AssetManager::hasFont(const std::string& name) const {
    return fonts.find(name) != fonts.end();
}

void AssetManager::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(filename)) {
        soundBuffers[name] = buffer;
    } else {
        std::cerr << "Error loading sound: " << filename << std::endl;
    }
}

sf::SoundBuffer& AssetManager::getSoundBuffer(const std::string& name) {
    return soundBuffers.at(name);
}

bool AssetManager::hasSound(const std::string& name) const {
    return soundBuffers.find(name) != soundBuffers.end();
}

void AssetManager::processImage(sf::Image& image) {
    sf::Vector2u size = image.getSize();
    if (size.x < 2 || size.y < 2) return;

    // Force-Alpha: Identify background colors from corners
    std::vector<sf::Color> bgColors;
    // Sample a small 10x10 block at each corner to catch both colors of a checkerboard
    for (unsigned int x = 0; x < std::min(10u, size.x); ++x) {
        for (unsigned int y = 0; y < std::min(10u, size.y); ++y) {
            bgColors.push_back(image.getPixel(sf::Vector2u(x, y)));
            bgColors.push_back(image.getPixel(sf::Vector2u(size.x - 1 - x, y)));
            bgColors.push_back(image.getPixel(sf::Vector2u(x, size.y - 1 - y)));
            bgColors.push_back(image.getPixel(sf::Vector2u(size.x - 1 - x, size.y - 1 - y)));
        }
    }

    auto isBg = [&](sf::Color c) {
        if (c.a < 10) return true; // Already transparent
        
        // Neutral grays/whites (checkerboards)
        bool isNeutral = std::abs((int)c.r - (int)c.g) < 25 && std::abs((int)c.g - (int)c.b) < 25;
        if (isNeutral && c.r > 130) return true; // Only light neutral colors
        
        for (const auto& bg : bgColors) {
            if (std::abs((int)c.r - (int)bg.r) < 50 &&
                std::abs((int)c.g - (int)bg.g) < 50 &&
                std::abs((int)c.b - (int)bg.b) < 50) return true;
        }
        return false;
    };

    for (unsigned int y = 0; y < size.y; ++y) {
        for (unsigned int x = 0; x < size.x; ++x) {
            if (isBg(image.getPixel(sf::Vector2u(x, y)))) {
                image.setPixel(sf::Vector2u(x, y), sf::Color(0, 0, 0, 0));
            }
        }
    }

    // Crop (tighten bounds) to remove any isolated edge pixels
    unsigned int minX = size.x, minY = size.y, maxX = 0, maxY = 0;
    bool found = false;
    for (unsigned int y = 0; y < size.y; ++y) {
        for (unsigned int x = 0; x < size.x; ++x) {
            if (image.getPixel(sf::Vector2u(x, y)).a > 50) {
                if (x < minX) minX = x; if (x > maxX) maxX = x;
                if (y < minY) minY = y; if (y > maxY) maxY = y;
                found = true;
            }
        }
    }

    if (found) {
        sf::Image cropped;
        cropped.resize(sf::Vector2u(maxX - minX + 1, maxY - minY + 1));
        for (unsigned int y = 0; y < cropped.getSize().y; ++y)
            for (unsigned int x = 0; x < cropped.getSize().x; ++x)
                cropped.setPixel(sf::Vector2u(x, y), image.getPixel(sf::Vector2u(minX + x, minY + y)));
        image = cropped;
    }
}
