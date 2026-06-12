/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Engine
*/

#include "Engine.hpp"

/**
* Initializes the Engine with default parameters.
**/
Zappy::Engine::Engine() : Engine(1280, 720, "Zappy - GUI")
{
    _resourceModels[Zappy::RESSOURCE::FOOD] = std::make_unique<Zappy::Model3D>("assets/object/Food.glb");
    _resourceModels[Zappy::RESSOURCE::LINEMATE] = std::make_unique<Zappy::Model3D>("assets/object/Linemate.glb");
    _resourceModels[Zappy::RESSOURCE::DERAUMERE] = std::make_unique<Zappy::Model3D>("assets/object/Deraumere.glb");
    _resourceModels[Zappy::RESSOURCE::SIBUR] = std::make_unique<Zappy::Model3D>("assets/object/Sibur.glb");
    _resourceModels[Zappy::RESSOURCE::MENDIANE] = std::make_unique<Zappy::Model3D>("assets/object/Mendiane.glb");
    _resourceModels[Zappy::RESSOURCE::PHIRAS] = std::make_unique<Zappy::Model3D>("assets/object/Phiras.glb");
    _resourceModels[Zappy::RESSOURCE::THYSTAME] = std::make_unique<Zappy::Model3D>("assets/object/Thystame.glb");
}

/**
* Initializes the Engine with custom parameters.
* @param width The width of the window.
* @param height The height of the window.
* @param title The title of the window.
**/
Zappy::Engine::Engine(int width, int height, const std::string &title)
    : _audioDevice(),
      _window(width, height, title, 60),
      _camera({15.0f, 15.0f, 15.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f),
      _hexTile("assets/object/GrassTile.glb"),
      _mainFont("assets/fonts/Roboto-Black.ttf"),
      _title(_mainFont, "EPITECH - ZAPPY", {500.0f, 20.0f}, 40.0f, DARKBLUE),
      _sprite("assets/sprites/epitech.png"),
      _music("assets/audio/music.mp3"),
      _sound("assets/audio/sound.mp3"),
      _gap(0.3f)
{
    _audioDevice.setMasterVolume(0.5f);
    _music.play();
}

/**
* Checks if the engine window is open.
* @return True if the window is open, false otherwise.
**/
bool Zappy::Engine::isOpen() const
{
    return _window.isOpen();
}

/**
* Handles user inputs.
* @return 1 if the user wants to quit, 0 otherwise.
**/
int Zappy::Engine::handleInputs()
{
    if (Zappy::Input::isKeyPressed(KEY_ESCAPE)) {
        return 1;
    }

    if (Zappy::Input::isKeyPressed(KEY_SPACE)) {
        _sound.play();
    }
    return 0;
}

/**
* Renders the 3D game scene.
* @param game The game instance to render.
**/
void Zappy::Engine::render3D(const Zappy::Game &game)
{
    const Zappy::Map &map = game.getMap();
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();

    if (mapWidth == 0 || mapHeight == 0)
        return;

    float totalWidth = (mapWidth - 1) * (_hexTile.getWidth() + _gap);
    float totalDepth = (mapHeight - 1) * (_hexTile.getDepth() + _gap);

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            float xPos = x * (_hexTile.getWidth() + _gap) - (totalWidth / 2.0f);
            float zPos = y * (_hexTile.getDepth() + _gap) - (totalDepth / 2.0f);
            
            _hexTile.draw({xPos, 0.0f, zPos}, 1.0f, WHITE);
        }
    }
}

/**
* Renders the 2D game scene.
* @param game The game instance to render.
**/
void Zappy::Engine::render2D(const Zappy::Game &)
{
    _title.draw();
    _sprite.draw({10.0f, 50.0f, 0.0f}, 0.1f, WHITE);
    _window.drawFPS(10, 10);
}

/**
* Renders the game scene.
* @param game The game instance to render.
**/
void Zappy::Engine::render(const Zappy::Game &game)
{
    _music.update();
    _camera.update();

    _window.beginDrawing();
        _window.clear(SKYBLUE);
        _camera.beginMode();
        render3D(game);
        _camera.endMode();
        render2D(game);
    _window.endDrawing();
}