#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <optional>
#include "Bird.h"
#include "Pipe.h"

enum class GameState { MENU, PLAYING, PAUSED, GAME_OVER };
enum class Difficulty { EASY, MEDIUM, HARD };

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    
    void resetGame();
    void spawnPipe();
    void saveHighScore();
    void loadHighScore();
    void handleCollision();

    sf::RenderWindow window;
    GameState state;
    Difficulty difficulty;

    std::optional<Bird> bird;
    std::vector<PipePair> pipes;
    float pipeSpawnTimer;
    float pipeSpawnInterval;

    int score;
    int highScore;
    
    std::optional<sf::Text> scoreText;
    std::optional<sf::Text> menuText;
    std::optional<sf::Text> creditText;
    std::optional<sf::Text> gameOverText;

    std::optional<sf::Sprite> background;
    float groundOffset = 0.0f;

    std::optional<sf::Sound> flapSound;
    std::optional<sf::Sound> collisionSound;
    sf::Music bgMusic;

    float speed;
};
