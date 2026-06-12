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

    #include "RTCError.hpp"

    #include "Window.hpp"
    #include "Camera.hpp"
    #include "Model3D.hpp"
    #include "Font.hpp"
    #include "Text.hpp"
    #include "Input.hpp"
    #include "Sprite.hpp"
    #include "AudioDevice.hpp"
    #include "Sound.hpp"
    #include "Music.hpp"
    #include "Game.hpp"
    #include "Tile.hpp"

namespace Zappy
{
    class Engine
    {
        public:
            Engine();
            Engine(int width, int height, const std::string &title);
            ~Engine() = default;

            bool isOpen() const;
            int handleInputs();
            void render(const Zappy::Game &game);
            void renderTile(const Zappy::Game &game) const;
        
        private:
            void render3D(const Zappy::Game &game);
            void render2D(const Zappy::Game &game);

            Zappy::AudioDevice _audioDevice;
            Zappy::Window _window;
            Zappy::Camera _camera;
            
            Zappy::Model3D _hexTile;
            std::unordered_map<Zappy::RESSOURCE, std::unique_ptr<Zappy::Model3D>> _resourceModels;

            Zappy::Font _mainFont;
            Zappy::Text _title;
            Zappy::Sprite _sprite;
            Zappy::Music _music;
            Zappy::Sound _sound;

            float _gap;
    };
};
