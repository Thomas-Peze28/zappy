/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Camera
*/

#include "Camera.hpp"

/**
* Initializes a new Camera object with default parameters.
**/
Zappy::Camera::Camera()
{
    _camera.position = (Vector3){ 15.0f, 15.0f, 15.0f };
    _camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    _camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    _camera.fovy = 45.0f;
    _camera.projection = CAMERA_PERSPECTIVE;
}

/**
* Initializes a new Camera object with custom parameters.
* @param position The position of the camera.
* @param target The target the camera is looking at.
* @param up The up vector of the camera.
* @param fovy The field of view angle in the y-axis.
**/
Zappy::Camera::Camera(Vector3 position, Vector3 target, Vector3 up, float fovy)
{
    _camera.position = position;
    _camera.target = target;
    _camera.up = up;
    _camera.fovy = fovy;
    _camera.projection = CAMERA_PERSPECTIVE;
}

/**
* Updates the camera's position and orientation.
* @param mode The update mode.
**/
void Zappy::Camera::update(int mode)
{
    UpdateCamera(&_camera, mode);
}

/**
* Begins the 3D mode.
**/
void Zappy::Camera::beginMode() const
{
    BeginMode3D(_camera);
}

/**
* Ends the 3D mode.
**/
void Zappy::Camera::endMode() const
{
    EndMode3D();
}

/**
* Sets the position of the camera.
* @param position The new position of the camera.
**/
void Zappy::Camera::setPosition(Vector3 position)
{
    _camera.position = position;
}

/**
* Sets the target of the camera.
* @param target The new target of the camera.
**/
void Zappy::Camera::setTarget(Vector3 target)
{
    _camera.target = target;
}

/**
* Gets the position of the camera.
* @return The position of the camera.
**/
Vector3 Zappy::Camera::getPosition() const
{
    return _camera.position;
}

/**
* Gets the target of the camera.
* @return The target of the camera.
**/
Vector3 Zappy::Camera::getTarget() const
{
    return _camera.target;
}
