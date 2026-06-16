/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Sound
*/

#include "Sound.hpp"

/**
* Constructor for the Sound class. Loads a sound from the specified file path.
* @param filePath The path to the sound file.
* @throws ErrorMajor if the sound fails to load.
* @note The sound is loaded using the raylib library. If the sound fails to load, an error is thrown.
**/
Zappy::Sound::Sound(const std::string &filePath)
{
    _sound = LoadSound(filePath.c_str());

    if (_sound.frameCount == 0) {
        throw ErrorMajor("Failed to load sound: " + filePath);
    }
}

/**
* Destructor for the Sound class. Unloads the sound to free resources.
**/
Zappy::Sound::~Sound()
{
    UnloadSound(_sound);
}

/**
* Plays the sound.
**/
void Zappy::Sound::play() const
{
    PlaySound(_sound);
}

/**
* Stops the sound if it is currently playing.
**/
void Zappy::Sound::stop() const
{
    StopSound(_sound);
}

/**
* Sets the volume for the sound.
* @param volume The desired volume level (0.0f to 1.0f).
**/
void Zappy::Sound::setVolume(float volume)
{
    SetSoundVolume(_sound, volume);
}
