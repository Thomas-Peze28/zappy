/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Text
*/

#include "Text.hpp"

/**
* @brief Construct a new Text object
* @param font The font to use for rendering the text
* @param text The string to display
* @param position The position of the text on the screen
* @param fontSize The size of the font
* @param color The color of the text
**/
Zappy::Text::Text(const Font& font, const std::string& text, Vector2 position, float fontSize, Color color)
        : _font(font), _string(text), _position(position), _fontSize(fontSize), _color(color)
{
}

/**
* @brief Draw the text on the screen
**/
void Zappy::Text::draw() const
{
    _font.draw(_string, _position, _fontSize, 1.0f, _color);
}

/**
* @brief Set the string to display
* @param text The string to display
**/
void Zappy::Text::setString(const std::string& text)
{
    _string = text;
}

/**
* @brief Set the position of the text
* @param position The position of the text on the screen
**/
void Zappy::Text::setPosition(Vector2 position)
{
    _position = position;
}

/**
* @brief Set the color of the text
* @param color The color of the text
**/
void Zappy::Text::setColor(Color color)
{
    _color = color;
}

/**
* @brief Set the font size of the text
* @param size The size of the font
**/
void Zappy::Text::setFontSize(float size)
{
    _fontSize = size;
}

/**
* @brief Get the string to display
* @return The string to display
**/
const std::string& Zappy::Text::getString() const
{
    return _string;
}

/**
* @brief Get the position of the text
* @return The position of the text on the screen
**/
Vector2 Zappy::Text::getPosition() const
{
    return _position;
}

/**
* @brief Get the color of the text
* @return The color of the text
**/
Color Zappy::Text::getColor() const
{
    return _color;
}

/**
* @brief Get the font size of the text
* @return The size of the font
**/
float Zappy::Text::getFontSize() const
{
    return _fontSize;
}
