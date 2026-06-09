/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Sprite
*/

#include "Sprite.hpp"

/**
* Initializes a Sprite object by loading a 2D texture from the specified file path.
* @param filePath The path to the 2D texture file.
* @throws ErrorMajor if the texture fails to load.
* @note The texture is loaded using the raylib library, and the dimensions are calculated from the texture's width and height.
*       If the texture fails to load, the width and height are set to 0.0f.
*       The texture is unloaded in the destructor to free resources.
**/
Zappy::Sprite::Sprite(const std::string &filePath)
{
    _texture = LoadTexture(filePath.c_str());

    if (_texture.id == 0) {
        _width = 0.0f;
        _height = 0.0f;
        ErrorMajor("Failed to load texture: " + filePath);
    }

    _width = (float)_texture.width;
    _height = (float)_texture.height;
}

/**
* Destructor for the Sprite class. Unloads the texture to free resources.
**/
Zappy::Sprite::~Sprite()
{
    UnloadTexture(_texture);
}


/**
* Draws the sprite at the specified position, with the given scale and tint color.
* @param position The position in 3D space where the sprite should be drawn.
* @param scale The scale factor to apply to the sprite when drawing.
* @param tint The color tint to apply to the sprite when drawing.
* @note This function uses the raylib library's DrawTexture function to render the sprite.
**/
void Zappy::Sprite::draw(const Vector3 &position, float scale, Color tint) const
{
    DrawTextureEx(_texture, {position.x, position.y}, 0.0f, scale, tint);
}

/**
* Returns the width of the sprite.
* @return The width of the sprite as a float.
**/
float Zappy::Sprite::getWidth() const
{
    return _width;
}

/**
* Returns the height of the sprite.
* @return The height of the sprite as a float.
**/
float Zappy::Sprite::getHeight() const
{
    return _height;
}
