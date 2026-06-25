/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** AudioDevice
*/

#include "AudioDevice.hpp"

/**
* Constructor for the AudioDevice class.
**/
Zappy::AudioDevice::AudioDevice()
{
    InitAudioDevice();

    if (!IsAudioDeviceReady()) {
        throw ErrorMajor("Failed to initialize audio device.");
    }
}

/**
* Destructor for the AudioDevice class.
**/
Zappy::AudioDevice::~AudioDevice()
{
    if (IsAudioDeviceReady()) {
        CloseAudioDevice();
    }
}

/**
* Checks if the audio device is ready for use.
* @return True if the audio device is ready, false otherwise.
**/
bool Zappy::AudioDevice::isReady() const
{
    return IsAudioDeviceReady();
}

/**
* Sets the master volume for the audio device.
* @param volume The desired master volume level (0.0f to 1.0f).
**/
void Zappy::AudioDevice::setMasterVolume(float volume)
{
    SetMasterVolume(volume);
}
