/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Model3D
*/

#include "Model3D.hpp"

/**
* Initializes a Model3D object by loading a 3D model from the specified file path.
* It calculates the width and depth of the model based on its bounding box.
* @param filePath The path to the 3D model file.
* @throws ErrorMajor if the model fails to load.
* @note The model is loaded using the raylib library, and the dimensions are calculated from the model's bounding box.
*       If the model fails to load, the width and depth are set to 0.0f.
*       The model is unloaded in the destructor to free resources.
**/
Zappy::Model3D::Model3D(const std::string &filePath)
{
    _model = LoadModel(filePath.c_str());

    if (_model.meshes == nullptr || _model.meshCount == 0) {
        _width = 0.0f;
        _depth = 0.0f;
        throw ErrorMajor("Failed to load model: " + filePath);
    }

    BoundingBox bbox = GetModelBoundingBox(_model);
    _width = bbox.max.x - bbox.min.x;
    _depth = bbox.max.z - bbox.min.z;
}

/**
* Destructor for the Model3D class. Unloads the 3D model to free resources.
**/
Zappy::Model3D::~Model3D()
{
    UnloadModel(_model);
}


/**
* Draws the 3D model at the specified position, with the given scale and tint color.
* @param position The position in 3D space where the model should be drawn.
* @param scale The scale factor to apply to the model when drawing.
* @param tint The color tint to apply to the model when drawing.
* @note This function uses the raylib library's DrawModel function to render the model.
**/
void Zappy::Model3D::draw(const Vector3 &position, float scale, Color tint) const
{
    DrawModel(_model, position, scale, tint);
}

/**
* Returns the width of the 3D model.
* @return The width of the model as a float.
**/
float Zappy::Model3D::getWidth() const
{
    return _width;
}

/**
* Returns the depth of the 3D model.
* @return The depth of the model as a float.
**/
float Zappy::Model3D::getDepth() const
{
    return _depth;
}
