cmake_minimum_required(VERSION 3.16)

project(SnakeMaze VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 1. Find packages (Included Qml for stability)
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    QuickControls2
    Multimedia
    Qml
)

qt_standard_project_setup(REQUIRES 6.5)

# 2. Set Policy Early
qt_policy(SET QTP0004 NEW)

# Collect all source files
set(SOURCES
    src/main.cpp
    src/core/GameEngine.cpp
    src/core/MazeGenerator.cpp
    src/core/Snake.cpp
    src/core/PowerUp.cpp
    src/core/GameState.cpp
    src/ai/AIController.cpp
    src/ai/AStarPathfinder.cpp
)

set(HEADERS
    src/core/GameEngine.h
    src/core/MazeGenerator.h
    src/core/Snake.h
    src/core/PowerUp.h
    src/core/GameState.h
    src/ai/AIController.h
    src/ai/AStarPathfinder.h
)

# 3. Create Executable
qt_add_executable(appSnakeMaze
    ${SOURCES}
    ${HEADERS}
)

# 4. Add QML Module (Target name must match appSnakeMaze)
qt_add_qml_module(appSnakeMaze
    URI SnakeMaze
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml/SnakeMaze"
    QML_FILES
        src/ui/Main.qml
        src/ui/GameScreen.qml
        src/ui/MenuScreen.qml
        src/ui/DifficultyScreen.qml
        src/ui/GameOverScreen.qml
        src/ui/HUDOverlay.qml
        src/ui/components/SnakeRenderer.qml
        src/ui/components/MazeRenderer.qml
        src/ui/components/AnimatedButton.qml
        src/ui/components/PowerUpItem.qml
        src/ui/components/ParticleEffect.qml
        src/ui/components/ScorePanel.qml
        src/ui/components/DifficultyCard.qml
    RESOURCES
        src/assets/images/logo.png
)

# 5. Link Libraries to the NEW target name
target_link_libraries(appSnakeMaze PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Multimedia
)

# 6. Include Directories
target_include_directories(appSnakeMaze PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ai
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)

# 7. Install Command
install(TARGETS appSnakeMaze
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)