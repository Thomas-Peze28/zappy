/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Shape3D
*/

#include "Shape3D.hpp"

/**
* @brief Draws a wireframe sphere at the specified position with the given radius, rings, slices, and color.
* @param position The position of the sphere.
* @param radius The radius of the sphere.
* @param rings The number of rings in the sphere.
* @param slices The number of slices in the sphere.
* @param color The color of the sphere.
**/
void Zappy::Shape3D::drawSphereWires(Vector3 position, float radius, int rings, int slices, Color color)
{
    ::DrawSphereWires(position, radius, rings, slices, color);
}

/**
* @brief Draws a wireframe cylinder at the specified position with the given dimensions and color.
* @param position The position of the cylinder.
* @param radiusTop The radius of the top of the cylinder.
* @param radiusBottom The radius of the bottom of the cylinder.
* @param height The height of the cylinder.
* @param slices The number of slices in the cylinder.
* @param color The color of the cylinder.
**/
void Zappy::Shape3D::drawCylinderWires(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color)
{
    ::DrawCylinderWires(position, radiusTop, radiusBottom, height, slices, color);
}

/**
* @brief Draws a rectangle at the specified position with the given dimensions and color.
* @param posX The x-coordinate of the rectangle.
* @param posY The y-coordinate of the rectangle.
* @param width The width of the rectangle.
* @param height The height of the rectangle.
* @param color The color of the rectangle.
**/
void Zappy::Shape3D::drawRectangle(int posX, int posY, int width, int height, Color color)
{
    ::DrawRectangle(posX, posY, width, height, color);
}

/**
* @brief Draws the outline of a rectangle at the specified position with the given dimensions and color.
* @param posX The x-coordinate of the rectangle.
* @param posY The y-coordinate of the rectangle.
* @param width The width of the rectangle.
* @param height The height of the rectangle.
* @param color The color of the rectangle.
**/
void Zappy::Shape3D::drawRectangleLines(int posX, int posY, int width, int height, Color color)
{
    ::DrawRectangleLines(posX, posY, width, height, color);
}
