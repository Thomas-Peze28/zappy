/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Engine
*/

#pragma once

    #include <cstdlib>
    #include <string>
    #include <iostream>
    #include <csignal>
    #include <vector>
    #include <unordered_map>
    #include <memory>
    #include <cmath>
    #include <filesystem>

    #include "RTCError.hpp"

    #include "Network.hpp"
    #include "Window.hpp"
    #include "Camera.hpp"
    #include "Model3D.hpp"
    #include "Font.hpp"
    #include "Text.hpp"
    #include "Input.hpp"
    #include "Sprite.hpp"
    #include "Hud.hpp"
    #include "AudioDevice.hpp"
    #include "Sound.hpp"
    #include "Music.hpp"
    #include "Game.hpp"
    #include "Tile.hpp"
    #include "Shape3D.hpp"

    #define PLAYER_JUMP_ANIM 0
    #define PLAYER_IDLE_ANIM 1
    #define PLAYER_WALK_ANIM 2

namespace Zappy
{
    class Engine
    {
        public:
            Engine();
            Engine(int width, int height, const std::string &title);
            ~Engine() = default;

            bool isOpen() const;
            int handleInputs(Zappy::Game &game, Zappy::Network &server);
            void render(const Zappy::Game &game);
            void renderTile(const Zappy::Game &game) const;
            void renderResource(const Zappy::Tile &tile, float x, float y, float z) const;
            void renderPlayer(const Zappy::Game &game) const;
            void renderEgg(const Zappy::Game &game) const;

            void renderWavesAnimation(Vector3 position, Color color, float radius, float speed) const;
            void renderWireSphereAnimation(Vector3 position, Color color, float radius, float speed) const;

            void nextCameraMode();

            void get_map_content(Zappy::Game &game, Zappy::Network &server);
        
        private:
            void render3D(const Zappy::Game &game);
            void render2D(const Zappy::Game &game);
            std::string resolveAssetPath(const std::string &relativePath);
            void handleMouseSelection(const Zappy::Game &game);

            Zappy::AudioDevice _audioDevice;
            Zappy::Window _window;
            Zappy::Camera _camera;

            Zappy::Model3D _hexTile;
            std::unordered_map<Zappy::RESSOURCE, std::unique_ptr<Zappy::Model3D>> _resourceModels;
        
            std::unique_ptr<Zappy::Model3D> _playerModel;
            mutable std::unordered_map<int, int> _playerAnimFrames;
    
            std::unique_ptr<Zappy::Model3D> _eggModel;

            Zappy::Font _mainFont;
            Zappy::Text _title;
            Zappy::Sprite _sprite;
            Zappy::Music _music;
            Zappy::Sound _sound;

            Zappy::Hud _hud;

            float _gap;

            int _selectedX;
            int _selectedY;
            int _selectedPlayerId;
    };
};
