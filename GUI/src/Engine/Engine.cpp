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
    _playerModel = std::make_unique<Zappy::Model3D>("assets/object/Generic_Male.glb");
    _eggModel = std::make_unique<Zappy::Model3D>("assets/object/Egg.glb");
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

    _resourceModels[Zappy::RESSOURCE::FOOD] = std::make_unique<Zappy::Model3D>("assets/object/Food.glb");
    _resourceModels[Zappy::RESSOURCE::LINEMATE] = std::make_unique<Zappy::Model3D>("assets/object/Linemate.glb");
    _resourceModels[Zappy::RESSOURCE::DERAUMERE] = std::make_unique<Zappy::Model3D>("assets/object/Deraumere.glb");
    _resourceModels[Zappy::RESSOURCE::SIBUR] = std::make_unique<Zappy::Model3D>("assets/object/Sibur.glb");
    _resourceModels[Zappy::RESSOURCE::MENDIANE] = std::make_unique<Zappy::Model3D>("assets/object/Mendiane.glb");
    _resourceModels[Zappy::RESSOURCE::PHIRAS] = std::make_unique<Zappy::Model3D>("assets/object/Phiras.glb");
    _resourceModels[Zappy::RESSOURCE::THYSTAME] = std::make_unique<Zappy::Model3D>("assets/object/Thystame.glb");
    _playerModel = std::make_unique<Zappy::Model3D>("assets/object/Generic_Male.glb");
    _eggModel = std::make_unique<Zappy::Model3D>("assets/object/Egg.glb");
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
* Switches the camera mode.
**/
void Zappy::Engine::nextCameraMode()
{
    if (_camera.getMode() == CAMERA_FREE)
        _camera.setMode(CAMERA_ORBITAL);
    else
        _camera.setMode(CAMERA_FREE);
}

/**
* Handles user inputs.
* @return 1 if the user wants to quit, 0 otherwise.
**/
int Zappy::Engine::handleInputs()
{
    if (Zappy::Input::isKeyPressed(KEY_ESCAPE))
    {
        return 1;
    }

    if (Zappy::Input::isKeyPressed(KEY_SPACE))
    {
        _sound.play();
    }

    if (Zappy::Input::isKeyPressed(KEY_F))
    {
        _window.toggleFullscreen();
    }

    if (Zappy::Input::isKeyPressed(KEY_C))
    {
        nextCameraMode();
    }

    return 0;
}

/**
* Renders the resources on a tile.
* @param tile The tile to render resources for.
* @param x The x-coordinate of the tile.
* @param y The y-coordinate of the tile.
* @param z The z-coordinate of the tile.
**/
void Zappy::Engine::renderResource(const Zappy::Tile &tile, float x, float y, float z) const
{
    for (const auto &[resourceType, modelPtr] : _resourceModels) {
        if (tile.getResource(resourceType) > 0) {
            float radius = 0.65f; 
            float halfRad = radius * sinf(PI / 6.0f);
            float heightRad = radius * cosf(PI / 6.0f);

            float xOffset = 0.0f;
            float zOffset = 0.0f;

            float scale = 0.00f;

            switch (resourceType) {
                case Zappy::RESSOURCE::FOOD:      
                    xOffset = 0.0f;     
                    zOffset = 0.0f;    
                    scale = 0.015f;   
                    break;
                case Zappy::RESSOURCE::LINEMATE:  
                    xOffset = radius;   
                    zOffset = 0.0f;       
                    scale = 0.5f;   
                    break;
                case Zappy::RESSOURCE::DERAUMERE: 
                    xOffset = halfRad;  
                    zOffset = heightRad;  
                    scale = 1.0f;   
                    break;
                case Zappy::RESSOURCE::SIBUR:     
                    xOffset = -halfRad;
                    zOffset = heightRad;
                    scale = 0.5f;
                    break;
                case Zappy::RESSOURCE::MENDIANE:  
                    xOffset = -radius;  
                    zOffset = 0.0f;
                    scale = 0.02f;
                    break;
                case Zappy::RESSOURCE::PHIRAS:    
                    xOffset = -halfRad; 
                    zOffset = -heightRad; 
                    scale = 0.1f;
                    break;
                case Zappy::RESSOURCE::THYSTAME:  
                    xOffset = halfRad;
                    zOffset = -heightRad;
                    scale = 0.05f;
                    break;
            }
            modelPtr->draw({x + xOffset, y, z + zOffset}, scale, WHITE);
        }
    }
}

/**
* Renders a tiles with resources in the game.
* @param game The game instance to render.
**/
void Zappy::Engine::renderTile(const Zappy::Game &game) const
{
    const Zappy::Map &map = game.getMap();
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();

    if (mapWidth == 0 || mapHeight == 0)
        return;

    float totalWidth = (mapWidth - 1) * (_hexTile.getWidth() + _gap);
    float totalDepth = (mapHeight - 1) * (_hexTile.getDepth() + _gap);

    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            float xPos = x * (_hexTile.getWidth() + _gap) - (totalWidth / 2.0f);
            float zPos = y * (_hexTile.getDepth() + _gap) - (totalDepth / 2.0f);

            _hexTile.draw({xPos, 0.0f, zPos}, 1.0f, WHITE);

            const Zappy::Tile &tile = map.getTile(x, y);

            renderResource(tile, xPos, 0.15f, zPos);
        }
    }
}

/**
* Renders the players in the game.
* @param game The game instance to render.
**/
void Zappy::Engine::renderPlayer(const Zappy::Game &game) const
{
    const Zappy::Map &map = game.getMap();
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();

    if (mapWidth == 0 || mapHeight == 0)
        return;

    float totalWidth = (mapWidth - 1) * (_hexTile.getWidth() + _gap);
    float totalDepth = (mapHeight - 1) * (_hexTile.getDepth() + _gap);

    for (const auto &[id, player] : game.getPlayers()) {
        float xPos = player.getX() * (_hexTile.getWidth() + _gap) - (totalWidth / 2.0f);
        float zPos = player.getY() * (_hexTile.getDepth() + _gap) - (totalDepth / 2.0f);
        _playerModel->draw({xPos, 0.15f, zPos}, 0.5f, WHITE);
    }
}

/**
* Renders the eggs in the game.
* @param game The game instance to render.
**/
void Zappy::Engine::renderEgg(const Zappy::Game &game) const
{
    const Zappy::Map &map = game.getMap();
    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();

    if (mapWidth == 0 || mapHeight == 0)
        return;

    float totalWidth = (mapWidth - 1) * (_hexTile.getWidth() + _gap);
    float totalDepth = (mapHeight - 1) * (_hexTile.getDepth() + _gap);

    for (const auto &[id, egg] : game.getEggs()) {
        float xPos = egg.getX() * (_hexTile.getWidth() + _gap) - (totalWidth / 2.0f);
        float zPos = egg.getY() * (_hexTile.getDepth() + _gap) - (totalDepth / 2.0f);
        _eggModel->draw({xPos, 0.3f, zPos}, 1.0f, WHITE);
    }
}

/**
* Renders the 3D game scene.
* @param game The game instance to render.
**/
void Zappy::Engine::render3D(const Zappy::Game &game)
{
    renderTile(game);
    renderPlayer(game);
    renderEgg(game);
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