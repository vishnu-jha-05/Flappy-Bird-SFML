# Flappy Bird - C++ SFML Game

A beautifully polished, feature-rich 2D side-scrolling **Flappy Bird** clone built in **C++** using the **SFML 3.0.2** graphics library.

Developed by **Vishnu Jha**.

---

## 🎮 How to Play

### 1. Directly Run the Game
Simply double-click the **`FlappyBird.exe`** executable in the root folder! All required SFML `.dll` library dependencies are included in the folder so it runs instantly on Windows.

### 2. Controls
- **Mouse / Touch Screen**: 
  - Hover and click on **EASY**, **MEDIUM**, or **HARD** buttons on the Main Menu to select difficulty.
  - Left-click or tap anywhere on the screen during gameplay to make the bird flap.
  - On the Game Over screen, hover and click **RESTART** to play again or **MENU** to return to the difficulty selection.
- **Keyboard**:
  - **Spacebar**: Flap the bird's wings (or start the game).
  - **M**: Return to the Main Menu (on Game Over screen).
  - **R**: Restart the game (on Game Over screen).
  - **Esc**: Pause/Unpause the game.

---

## ✨ Features Implemented

- **Interactive Dynamic UI**: Custom rectangular selection buttons with gold text outline, smooth border thickness scaling, and highlight-on-hover visual responses.
- **Responsive Screen Mapping**: Fullscreen support using `window.mapPixelToCoords()`, ensuring buttons are clickable at any screen resolution or window scale.
- **Custom Difficulty Modes**:
  - **Easy**: Forgiving 420px gaps, slower scroll speed (`150.0f`).
  - **Medium**: Balanced 340px gaps, standard scroll speed (`200.0f`).
  - **Hard**: Challenging 260px gaps, fast scroll speed (`250.0f`), and **vertically moving obstacles** that dynamically oscillate up and down.
- **Fair Collision Detection**: Custom tightened hitboxes that match the bird's visible body model precisely to reward tight maneuvers.
- **Persistent High Scores**: Auto-saving and auto-loading through file stream connectivity with `highscore.txt`.
- **Procedural Grass Ground**: Programmatic scrolling bottom ground designed dynamically in C++ to guarantee 0% graphic glitches or AI checkerboard artifacts.

---

## 🛠️ Project Deliverables & Reports

For the detailed submission reports, including the full Object-Oriented software design, class architectures, and challenge breakdowns, please check:
📄 **[Project Submission Report (REPORT.md)](REPORT.md)**
