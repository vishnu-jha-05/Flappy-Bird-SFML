# Flappy Bird C++ SFML Project Report

**Developed by:** Vishnu Jha  
**Department:** Computer Science and Engineering, ITER, SOA University

---

## 1. Game Design & Architecture

The project is a 2D side-scrolling Flappy Bird clone designed in **C++** using the **SFML (Simple and Fast Multimedia Library)**. It leverages **Object-Oriented Programming (OOP)** principles to achieve modular, clean, and scalable code.

### Core Architecture Components
1. **`Game` Class (`Game.h` / `Game.cpp`)**: 
   - Manages the core game loop, window creation, states (Menu, Playing, Paused, Game Over), delta-time physics updates, custom rendering layers, and mouse/touch inputs.
2. **`Bird` Class (`Bird.h` / `Bird.cpp`)**: 
   - Controls the bird's physics simulation including gravity acceleration, jump forces, smooth tilt rotation (based on vertical velocity), and sprite rendering.
3. **`PipePair` Class (`Pipe.h` / `Pipe.cpp`)**: 
   - Simulates pairs of obstacles (top and bottom pipes) with a modular vertical gap. It handles horizontal scroll speed, offscreen cleanups, and vertical oscillation for hard difficulty.
4. **`AssetManager` Class (`AssetManager.h` / `AssetManager.cpp`)**: 
   - A thread-safe Singleton resource manager that loads, caches, and processes textures, fonts, and sound buffers efficiently to prevent redundant disk I/O.

---

## 2. Features Implemented

- **Dynamic Difficulty Levels**: 
  - **Easy**: 420px gaps, slower scroll speed (`150.0f`), and long spawn intervals.
  - **Medium**: 340px gaps, normal scroll speed (`200.0f`).
  - **Hard**: 260px gaps, fast scroll speed (`250.0f`), and **moving pipes** that oscillate vertically over time.
- **Persistent High Score System**: Save/load mechanisms linked to `highscore.txt` file handling.
- **Responsive UI & Graphics**:
  - Gold title overlay with shadow outline.
  - Menu selection buttons highlighting on mouse hover.
  - Custom buttons for Restarting and returning to the Menu on Game Over.
- **Audio & Visuals**: Preloaded sound triggers for bird flaps and collisions.
- **Procedural Animated Ground**: A programmatic, scrolling striped ground that moves dynamically.

---

## 3. Challenges Faced & Solutions

### Challenge 1: Procedural Ground Rendering
- **Problem**: Static ground texture assets had transparency issues that rendered poorly on certain backgrounds.
- **Solution**: The ground was implemented as a fully procedural design using C++ `sf::RectangleShape` objects that scroll programmatically based on the active difficulty speed, producing a clean, premium visual aesthetic.

### Challenge 2: Mouse Clicks Misaligned in Fullscreen
- **Problem**: When maximizing the window or running in full-screen mode, the mouse coordinate hitboxes for the difficulty buttons stopped working.
- **Solution**: Replaced default `sf::Mouse::getPosition` tracking with SFML's coordinate mapping engine: `window.mapPixelToCoords()`. This mathematically translates local desktop coordinates into the game's internal 800x600 coordinate system, ensuring buttons remain 100% responsive at any resolution.

### Challenge 3: Inaccurate Collisions
- **Problem**: The bird was colliding with pipes even when there was visible blank space between them due to a large rectangular sprite boundary.
- **Solution**: Shrank the bird's collision box boundary inwards by 12 pixels on each edge. This aligned the hitboxes precisely with the bird's visible body, rewarding close-call dodges.
