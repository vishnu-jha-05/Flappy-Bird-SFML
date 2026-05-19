#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

class AssetManager {
public:
    static AssetManager& getInstance();

    void loadTexture(const std::string& name, const std::string& filename);
    sf::Texture& getTexture(const std::string& name);
    bool hasTexture(const std::string& name) const;

    void loadFont(const std::string& name, const std::string& filename);
    sf::Font& getFont(const std::string& name);
    bool hasFont(const std::string& name) const;

    void loadSound(const std::string& name, const std::string& filename);
    sf::SoundBuffer& getSoundBuffer(const std::string& name);
    bool hasSound(const std::string& name) const;

private:
    AssetManager() = default;
    void processImage(sf::Image& image);
    ~AssetManager() = default;
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
};
