#include "Game.h"
#include "AssetManager.h"
#include <fstream>
#include <iostream>
#include <random>

Game::Game() : 
    window(sf::VideoMode(sf::Vector2u(800, 600)), "Flappy Bird - Vishnu Jha Edition"),
    state(GameState::MENU),
    difficulty(Difficulty::MEDIUM),
    score(0),
    highScore(0),
    pipeSpawnTimer(0),
    pipeSpawnInterval(2.0f),
    speed(200.0f) 
{
    window.setFramerateLimit(60);

    // Load assets
    AssetManager& assets = AssetManager::getInstance();
    assets.loadTexture("bird", "assets/textures/bird.png");
    assets.loadTexture("pipe", "assets/textures/pipe.png");
    assets.loadTexture("background", "assets/textures/background.png");
    assets.loadTexture("ground", "assets/textures/ground.png");
    
    // Fallback for font - check common Windows path
    assets.loadFont("main", "C:/Windows/Fonts/arial.ttf");
    assets.loadSound("flap", "assets/sounds/flap.wav");
    assets.loadSound("collision", "assets/sounds/collision.wav");

    if (assets.hasSound("flap")) flapSound.emplace(assets.getSoundBuffer("flap"));
    if (assets.hasSound("collision")) collisionSound.emplace(assets.getSoundBuffer("collision"));

    // Initialize sprites
    if (assets.hasTexture("background")) {
        background.emplace(assets.getTexture("background"));
        // Scale background to window
        sf::Vector2u winSize = window.getSize();
        sf::Vector2u texSize = background->getTexture().getSize();
        background->setScale(sf::Vector2f((float)winSize.x / texSize.x, (float)winSize.y / texSize.y));
    }

    // Ground sprite removed in favor of procedural ground

    // Text setup
    if (assets.hasFont("main")) {
        scoreText.emplace(assets.getFont("main"), "Score: 0");
        scoreText->setCharacterSize(35);
        scoreText->setFillColor(sf::Color::White);
        scoreText->setOutlineColor(sf::Color::Black);
        scoreText->setOutlineThickness(3.0f);
        scoreText->setPosition(sf::Vector2f(20.0f, 20.0f));

        menuText.emplace(assets.getFont("main"), "");
        menuText->setCharacterSize(45);
        menuText->setFillColor(sf::Color::White);
        menuText->setOutlineColor(sf::Color::Black);
        menuText->setOutlineThickness(2.0f);

        creditText.emplace(assets.getFont("main"), "Developed by Vishnu Jha");
        creditText->setCharacterSize(24);
        creditText->setFillColor(sf::Color(0, 255, 255, 180)); // Cyan translucent
        creditText->setOutlineColor(sf::Color(0, 0, 0, 150));
        creditText->setOutlineThickness(1.5f);
        creditText->setPosition(sf::Vector2f(260.0f, 550.0f));

        gameOverText.emplace(assets.getFont("main"), "GAME OVER\n\nPress R to Restart\nPress M for Menu");
        gameOverText->setCharacterSize(50);
        gameOverText->setFillColor(sf::Color(255, 50, 50));
        gameOverText->setOutlineColor(sf::Color::Black);
        gameOverText->setOutlineThickness(3.0f);
        gameOverText->setPosition(sf::Vector2f(200.0f, 150.0f));
    }

    if (assets.hasTexture("bird")) bird.emplace();

    loadHighScore();
    resetGame();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        // Keyboard support
        if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
            if (state == GameState::MENU) {
                if (kp->code == sf::Keyboard::Key::Space) state = GameState::PLAYING;
                if (kp->code == sf::Keyboard::Key::Num1) { difficulty = Difficulty::EASY; resetGame(); }
                if (kp->code == sf::Keyboard::Key::Num2) { difficulty = Difficulty::MEDIUM; resetGame(); }
                if (kp->code == sf::Keyboard::Key::Num3) { difficulty = Difficulty::HARD; resetGame(); }
            } 
            else if (state == GameState::PLAYING) {
                if (kp->code == sf::Keyboard::Key::Space) {
                    if (bird) bird->flap();
                    if (flapSound) flapSound->play();
                }
                if (kp->code == sf::Keyboard::Key::P) state = GameState::PAUSED;
            }
            else if (state == GameState::PAUSED) {
                if (kp->code == sf::Keyboard::Key::P) state = GameState::PLAYING;
            }
            else if (state == GameState::GAME_OVER) {
                if (kp->code == sf::Keyboard::Key::R) {
                    resetGame();
                    state = GameState::PLAYING;
                }
                if (kp->code == sf::Keyboard::Key::M) {
                    state = GameState::MENU;
                    resetGame();
                }
            }
        }

        // Mouse & Touch support
        if (event->is<sf::Event::MouseButtonPressed>() || event->is<sf::Event::TouchBegan>()) {
            sf::Vector2i pixelPos;
            if (const auto* tb = event->getIf<sf::Event::TouchBegan>()) {
                pixelPos = sf::Vector2i(tb->position.x, tb->position.y);
            } else if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                pixelPos = sf::Vector2i(mb->position.x, mb->position.y);
            } else {
                pixelPos = sf::Mouse::getPosition(window);
            }
            sf::Vector2f pos = window.mapPixelToCoords(pixelPos);

            if (state == GameState::MENU) {
                // Easy: 100 to 280, y: 300 to 360
                if (pos.x >= 100 && pos.x <= 280 && pos.y >= 300 && pos.y <= 360) {
                    difficulty = Difficulty::EASY; resetGame();
                }
                // Medium: 310 to 490, y: 300 to 360
                else if (pos.x >= 310 && pos.x <= 490 && pos.y >= 300 && pos.y <= 360) {
                    difficulty = Difficulty::MEDIUM; resetGame();
                }
                // Hard: 520 to 700, y: 300 to 360
                else if (pos.x >= 520 && pos.x <= 700 && pos.y >= 300 && pos.y <= 360) {
                    difficulty = Difficulty::HARD; resetGame();
                }
                else {
                    state = GameState::PLAYING;
                }
            } else if (state == GameState::PLAYING) {
                if (bird) bird->flap();
                if (flapSound) flapSound->play();
            } else if (state == GameState::GAME_OVER) {
                // Restart: 200 to 600, y: 300 to 360
                if (pos.x >= 200 && pos.x <= 600 && pos.y >= 300 && pos.y <= 360) {
                    resetGame();
                    state = GameState::PLAYING;
                }
                // Menu: 200 to 600, y: 380 to 440
                else if (pos.x >= 200 && pos.x <= 600 && pos.y >= 380 && pos.y <= 440) {
                    state = GameState::MENU;
                    resetGame();
                }
            }
        }
    }
}

void Game::update(float dt) {
    if (state != GameState::PLAYING || !bird) return;

    bird->update(dt);

    // Spawn pipes
    pipeSpawnTimer += dt;
    if (pipeSpawnTimer >= pipeSpawnInterval) {
        spawnPipe();
        pipeSpawnTimer = 0;
    }

    // Update ground
    groundOffset -= speed * dt;
    if (groundOffset <= -40.0f) groundOffset += 40.0f;

    // Update pipes
    for (auto it = pipes.begin(); it != pipes.end(); ) {
        it->update(dt, speed);
        
        // Oscillation for hard mode
        if (difficulty == Difficulty::HARD) {
            it->oscillate(dt, 80.0f, 1.5f);
        }

        // Scoring
        if (it->isPassed(bird->getPosition().x)) {
            score++;
            it->setPassed(true);
        }

        if (it->isOffScreen()) {
            it = pipes.erase(it);
        } else {
            ++it;
        }
    }

    // Collision detection
    handleCollision();

    if (scoreText) scoreText->setString("Score: " + std::to_string(score) + "  High Score: " + std::to_string(highScore));
}

void Game::handleCollision() {
    if (!bird) return;
    sf::FloatRect birdBounds = bird->getBounds();
    birdBounds.position.x += 12.0f;
    birdBounds.position.y += 12.0f;
    birdBounds.size.x -= 24.0f;
    birdBounds.size.y -= 24.0f;
    
    // Pipes
    for (const auto& pipe : pipes) {
        if (birdBounds.findIntersection(pipe.getTopBounds()) || 
            birdBounds.findIntersection(pipe.getBottomBounds())) {
            state = GameState::GAME_OVER;
            if (collisionSound) collisionSound->play();
            saveHighScore();
        }
    }

    // Ground and Top
    if (bird->getPosition().y < -15 || bird->getPosition().y > 550) {
        state = GameState::GAME_OVER;
        if (collisionSound) collisionSound->play();
        saveHighScore();
    }
}

void Game::render() {
    window.clear();
    if (background) window.draw(*background);
    
    for (const auto& pipe : pipes) window.draw(pipe);
    
    // Draw moving ground
    sf::RectangleShape groundRect(sf::Vector2f(800.0f, 50.0f));
    groundRect.setPosition(sf::Vector2f(0.0f, 550.0f));
    groundRect.setFillColor(sf::Color(80, 200, 80));
    window.draw(groundRect);

    for (float x = groundOffset; x < 800.0f; x += 40.0f) {
        sf::RectangleShape stripe(sf::Vector2f(20.0f, 50.0f));
        stripe.setPosition(sf::Vector2f(x, 550.0f));
        stripe.setFillColor(sf::Color(60, 180, 60));
        window.draw(stripe);
    }

    sf::RectangleShape groundBorder(sf::Vector2f(800.0f, 5.0f));
    groundBorder.setPosition(sf::Vector2f(0.0f, 545.0f));
    groundBorder.setFillColor(sf::Color(50, 150, 50));
    window.draw(groundBorder);

    if (bird) window.draw(*bird);
    
    if (scoreText) {
        scoreText->setString("Score: " + std::to_string(score) + "   High: " + std::to_string(highScore));
        window.draw(*scoreText);
    }

    if (state == GameState::MENU) {
        // Draw a translucent overlay
        sf::RectangleShape overlay(sf::Vector2f(800.0f, 600.0f));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        if (menuText) {
            sf::Text titleText = *menuText;
            titleText.setString("FLAPPY BIRD");
            titleText.setCharacterSize(65);
            titleText.setFillColor(sf::Color(255, 215, 0)); // Gold
            titleText.setOutlineColor(sf::Color::Black);
            titleText.setOutlineThickness(4.0f);
            
            sf::FloatRect titleBounds = titleText.getLocalBounds();
            titleText.setPosition(sf::Vector2f((800.0f - titleBounds.size.x) / 2.0f, 100.0f));
            window.draw(titleText);

            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
            auto drawButton = [&](sf::Vector2f pos, sf::Vector2f size, sf::Color color, std::string text, bool active) {
                sf::RectangleShape btn(size);
                btn.setPosition(pos);
                
                bool hovered = (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x && 
                                mousePos.y >= pos.y && mousePos.y <= pos.y + size.y);
                                
                if (hovered && !active) color = sf::Color(color.r, color.g, color.b, 255);
                else if (!active) color = sf::Color(color.r/2, color.g/2, color.b/2, 200);
                
                btn.setFillColor(color);
                btn.setOutlineColor(active ? sf::Color::Yellow : (hovered ? sf::Color::White : sf::Color(200,200,200)));
                btn.setOutlineThickness(active ? 4.0f : (hovered ? 2.0f : 1.0f));
                window.draw(btn);

                sf::Text btnText = *menuText;
                btnText.setString(text);
                btnText.setCharacterSize(30);
                btnText.setFillColor(sf::Color::White);
                
                sf::FloatRect textBounds = btnText.getLocalBounds();
                btnText.setPosition(sf::Vector2f(
                    pos.x + (size.x - textBounds.size.x) / 2.0f - textBounds.position.x,
                    pos.y + (size.y - textBounds.size.y) / 2.0f - textBounds.position.y
                ));
                window.draw(btnText);
            };

            drawButton(sf::Vector2f(100, 300), sf::Vector2f(180, 60), sf::Color(50, 200, 50), "EASY", difficulty == Difficulty::EASY);
            drawButton(sf::Vector2f(310, 300), sf::Vector2f(180, 60), sf::Color(200, 150, 20), "MEDIUM", difficulty == Difficulty::MEDIUM);
            drawButton(sf::Vector2f(520, 300), sf::Vector2f(180, 60), sf::Color(200, 50, 50), "HARD", difficulty == Difficulty::HARD);

            sf::Text startText = *menuText;
            startText.setString("Press SPACE to Start");
            startText.setCharacterSize(35);
            sf::FloatRect startBounds = startText.getLocalBounds();
            startText.setPosition(sf::Vector2f((800.0f - startBounds.size.x) / 2.0f, 420.0f));
            window.draw(startText);
        }
        if (creditText) window.draw(*creditText);
    } else if (state == GameState::GAME_OVER) {
        sf::RectangleShape overlay(sf::Vector2f(800.0f, 600.0f));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        if (gameOverText) {
            sf::Text goText = *gameOverText;
            goText.setString("GAME OVER");
            goText.setCharacterSize(60);
            sf::FloatRect bounds = goText.getLocalBounds();
            goText.setPosition(sf::Vector2f((800.0f - bounds.size.x) / 2.0f, 150.0f));
            window.draw(goText);
        }

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        auto drawButton = [&](sf::Vector2f pos, sf::Vector2f size, sf::Color color, std::string text) {
            sf::RectangleShape btn(size);
            btn.setPosition(pos);
            bool hovered = (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x && 
                            mousePos.y >= pos.y && mousePos.y <= pos.y + size.y);
            btn.setFillColor(hovered ? sf::Color(color.r, color.g, color.b, 255) : sf::Color(color.r/2, color.g/2, color.b/2, 200));
            btn.setOutlineColor(hovered ? sf::Color::White : sf::Color(200,200,200));
            btn.setOutlineThickness(hovered ? 3.0f : 1.0f);
            window.draw(btn);
            
            sf::Text btnText = *menuText;
            btnText.setString(text);
            btnText.setCharacterSize(30);
            sf::FloatRect textBounds = btnText.getLocalBounds();
            btnText.setPosition(sf::Vector2f(
                pos.x + (size.x - textBounds.size.x) / 2.0f - textBounds.position.x,
                pos.y + (size.y - textBounds.size.y) / 2.0f - textBounds.position.y
            ));
            window.draw(btnText);
        };

        drawButton(sf::Vector2f(200, 300), sf::Vector2f(400, 60), sf::Color(50, 150, 200), "RESTART");
        drawButton(sf::Vector2f(200, 380), sf::Vector2f(400, 60), sf::Color(200, 100, 50), "MENU");
    } else if (state == GameState::PAUSED) {
        if (menuText) {
            sf::Text pauseText = *menuText;
            pauseText.setString("PAUSED\n\nPress P to Resume");
            pauseText.setPosition(sf::Vector2f(200.0f, 200.0f));
            window.draw(pauseText);
        }
    }

    window.display();
}

void Game::resetGame() {
    if (bird) bird->reset(100, 300);
    pipes.clear();
    score = 0;
    pipeSpawnTimer = 0;

    // Difficulty settings
    switch(difficulty) {
        case Difficulty::EASY: 
            pipeSpawnInterval = 2.5f; 
            speed = 150.0f;
            break;
        case Difficulty::MEDIUM: 
            pipeSpawnInterval = 2.0f; 
            speed = 200.0f;
            break;
        case Difficulty::HARD: 
            pipeSpawnInterval = 1.5f; 
            speed = 250.0f;
            break;
    }
}

void Game::spawnPipe() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(150.0f, 400.0f);
    
    float gapSize = 300.0f;
    if (difficulty == Difficulty::EASY) gapSize = 420.0f;
    else if (difficulty == Difficulty::MEDIUM) gapSize = 340.0f;
    else gapSize = 260.0f;

    pipes.emplace_back(850.0f, dis(gen), gapSize);
}

void Game::saveHighScore() {
    if (score > highScore) {
        highScore = score;
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << highScore;
        }
    }
}

void Game::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
    }
}
