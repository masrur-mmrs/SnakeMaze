# SnakeMaze 🐍

A **Player vs CPU** snake maze game built with **Qt 6 + QML**.  
Race through a procedurally-generated maze to the center ★ while collecting power-ups. The snake with the highest score when reaching the goal wins.

---

## 📁 Directory Structure

```
SnakeMaze/
├── CMakeLists.txt              # Build configuration
├── README.md
│
├── src/
│   ├── main.cpp                # App entry point, QML type registration
│   │
│   ├── core/                   # Game logic (C++)
│   │   ├── GameEngine.h/.cpp   # Central orchestrator, tick loop
│   │   ├── GameState.h/.cpp    # Read-only QML-facing state mirror
│   │   ├── Snake.h/.cpp        # Snake body, movement, growth
│   │   ├── MazeGenerator.h/.cpp# Recursive-backtracking maze gen
│   │   └── PowerUp.h/.cpp      # Collectible types & values
│   │
│   ├── ai/                     # AI difficulty strategies
│   │   ├── AIController.h/.cpp # Dispatcher: Easy / Medium / Hard
│   │   └── AStarPathfinder.h/.cpp  # A* for Hard difficulty
│   │
│   └── ui/                     # QML screens & components
│       ├── Main.qml            # Root window + screen router
│       ├── MenuScreen.qml      # Animated main menu
│       ├── DifficultyScreen.qml# Difficulty card picker
│       ├── GameScreen.qml      # Gameplay + keyboard input
│       ├── HUDOverlay.qml      # Score bar + pause/quit
│       ├── GameOverScreen.qml  # Results + replay
│       │
│       └── components/         # Reusable QML widgets
│           ├── MazeRenderer.qml    # Canvas maze + snake + power-ups
│           ├── SnakeRenderer.qml   # Standalone snake preview
│           ├── PowerUpItem.qml     # Icon + label widget
│           ├── AnimatedButton.qml  # Glow button
│           ├── ScorePanel.qml      # Animated score counter
│           └── ParticleEffect.qml  # Burst particles on collect
│
├── assets/
│   ├── fonts/                  # (add custom .ttf fonts here)
│   ├── images/                 # (logo, icons)
│   └── sounds/                 # (add .wav/.mp3 sound effects)
│
├── levels/                     # (future: hand-crafted level JSON)
└── tests/                      # (future: unit tests)
```

---

## 🎮 Gameplay

| Action | Keys |
|--------|------|
| Move up | ↑ or W |
| Move right | → or D |
| Move down | ↓ or S |
| Move left | ← or A |
| Pause | P or ESC |

### Power-ups

| Icon | Name    | Points | Grow |
|------|---------|--------|------|
| ★    | Star    | +30    | +1   |
| ✦    | Berry   | +15    | +2   |
| ◆    | Crystal | +8     | +4   |

---

## 🤖 AI Difficulties

| Level  | Strategy | Notes |
|--------|----------|-------|
| Easy   | Random Walk | Changes direction randomly; avoids walls |
| Medium | Greedy | Always moves toward the goal (Manhattan distance) |
| Hard   | A\* Algorithm | Optimal pathfinding with star-power-up detours |

---

## 🔧 Build Requirements

- Qt 6.5+ (Core, Quick, QuickControls2, Multimedia)
- CMake 3.16+
- C++17 compiler (GCC 10+, Clang 12+, MSVC 2022)

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --parallel

# Run
./build/SnakeMaze
```

---

## 🧩 Adding Features

### New difficulty strategy
1. Add a method to `AIController` (`.h` + `.cpp`)
2. Wire it in `AIController::update()` switch statement
3. Add a card in `DifficultyScreen.qml`

### New power-up type
1. Add enum value to `PowerUp::Type`
2. Set `points()` and `growAmount()` values
3. Add icon/color in `MazeRenderer.qml` and `PowerUpItem.qml`

### New screen
1. Create `src/ui/NewScreen.qml`
2. Add to `CMakeLists.txt` QML_FILES list
3. Add a new index in `Main.qml` `StackLayout`

---

## 🎨 Color Palette

| Role | Hex | Usage |
|------|-----|-------|
| Background | `#0D0F1A` | Window background |
| Card | `#151828` | UI panels |
| Player | `#44E5A0` | Player snake, score |
| CPU | `#FF5E78` | CPU snake, score |
| Goal | `#FFD166` | Goal star, star power-up |
| Berry | `#C77DFF` | Berry power-up |
| Crystal | `#48CAE4` | Crystal power-up |
| Wall | `#0F1225` | Maze walls |
| Path | `#111428` | Open maze cells |