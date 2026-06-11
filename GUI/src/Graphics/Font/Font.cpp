/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Font
*/

#include "Font.hpp"

/**
* Initializes a Font object by loading a font from the specified file path.
* @param fontPath The path to the font file.
* @throws ErrorMajor if the font fails to load.
**/
Zappy::Font::Font(const std::string &fontPath)
{
    _font = LoadFont(fontPath.c_str());

    if (_font.texture.id == 0) {
        throw ErrorMajor("Failed to load font: " + fontPath);
    }
}

/**
* Destructor for the Font class. Unloads the font to free resources.
**/
Zappy::Font::~Font()
{
    UnloadFont(_font);
}

/**
* Draws the specified text at the given position with the specified font size and color.
* @param text The text to draw.
* @param x The x-coordinate of the text position.
* @param y The y-coordinate of the text position.
* @param fontSize The size of the font.
* @param color The color of the text.
**/
void Zappy::Font::draw(const std::string &text, Vector2 position, float fontSize, float spacing, Color color) const
{
    DrawTextEx(_font, text.c_str(), position, fontSize, spacing, color);
}

/**
* Measures the dimensions of the specified text with the given font size.
* @param text The text to measure.
* @param fontSize The size of the font.
* @return A Vector2 containing the width and height of the text.
**/
Vector2 Zappy::Font::measure(const std::string &text, int fontSize, float spacing) const
{
    return MeasureTextEx(_font, text.c_str(), fontSize, spacing);
}
