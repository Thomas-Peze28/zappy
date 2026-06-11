/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Music
*/

#include "Music.hpp"

/**
* Constructor for the Music class. Loads a music from the specified file path.
* @param filePath The path to the music file.
* @throws ErrorMajor if the music fails to load.
* @note The music is loaded using the raylib library. If the music fails to load, an error is thrown.
**/
Zappy::Music::Music(const std::string &filePath)
{
    _music = LoadMusicStream(filePath.c_str());

    if (_music.frameCount == 0) {
        throw ErrorMajor("Failed to load music: " + filePath);
    }
}

/**
* Destructor for the Music class. Unloads the music to free resources.
**/
Zappy::Music::~Music()
{
    UnloadMusicStream(_music);
}

/**
* Plays the music.
**/
void Zappy::Music::play() const
{
    PlayMusicStream(_music);
}

/**
* Updates the music stream.
**/
void Zappy::Music::update() const
{
    UpdateMusicStream(_music);
}

/**
* Stops the music if it is currently playing.
**/
void Zappy::Music::stop() const
{
    StopMusicStream(_music);
}

/**
* Sets the volume for the music.
* @param volume The desired volume level (0.0f to 1.0f).
**/
void Zappy::Music::setVolume(float volume)
{
    SetMusicVolume(_music, volume);
}
