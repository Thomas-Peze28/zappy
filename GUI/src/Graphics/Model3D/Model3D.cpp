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

    _animCount = 0;
    _animations = LoadModelAnimations(filePath.c_str(), &_animCount);
    _currentAnimIndex = 0;
    _animFrameCounter = 0;
}

/**
* Destructor for the Model3D class. Unloads the 3D model to free resources.
**/
Zappy::Model3D::~Model3D()
{
    if (_animations != nullptr) {
        UnloadModelAnimations(_animations, _animCount);
    }
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
* Draws the 3D model at the specified position, with the given rotation, scale and tint color.
* @param position The position in 3D space where the model should be drawn.
* @param rotationAxis The axis around which to rotate the model.
* @param rotationAngle The angle of rotation in radians.
* @param scale The scale factor to apply to the model when drawing.
* @param tint The color tint to apply to the model when drawing.
* @note This function uses the raylib library's DrawModelEx function to render the model.
**/
void Zappy::Model3D::draw(const Vector3 &position, const Vector3 &rotationAxis, float rotationAngle, float scale, Color tint) const
{
    DrawModelEx(_model, position, rotationAxis, rotationAngle, {scale, scale, scale}, tint);
}

/**
* Draws the 3D model standing at the specified position, with the given rotation and scale.
* @param position The position in 3D space where the model should be drawn.
* @param rotationAngleY The angle of rotation around the Y-axis in degrees.
* @param scale The scale factor to apply to the model when drawing.
* @param color The color tint to apply to the model when drawing.
**/
void Zappy::Model3D::drawStanding(Vector3 position, float rotationAngleY, float scale, Color color) const
{
    Quaternion qStand = QuaternionFromAxisAngle({1.0f, 0.0f, 0.0f}, 90.0f * DEG2RAD);
    Quaternion qDir = QuaternionFromAxisAngle({0.0f, 1.0f, 0.0f}, rotationAngleY * DEG2RAD);
    Quaternion qFinal = QuaternionMultiply(qDir, qStand);

    Vector3 finalAxis;
    float finalAngle;
    QuaternionToAxisAngle(qFinal, &finalAxis, &finalAngle);

    DrawModelEx(_model, position, finalAxis, finalAngle * RAD2DEG, {scale, scale, scale}, color);
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

/**
* Applies the specified animation frame to the model.
* @param animIndex The index of the animation to apply.
* @param frame The frame number to apply.
**/
void Zappy::Model3D::applyAnimation(int animIndex, int frame)
{
    if (_animations != nullptr && animIndex < _animCount) {
        UpdateModelAnimation(_model, _animations[animIndex], frame);
    }
}

/**
* Returns the number of frames in the specified animation.
* @param animIndex The index of the animation.
* @return The number of frames in the animation.
**/
int Zappy::Model3D::getAnimFrameCount(int animIndex) const
{
    if (_animations != nullptr && animIndex < _animCount) {
        return _animations[animIndex].keyframeCount;
    }
    return 0;
}

/**
* Rotates the 3D model around the X-axis by the specified angle.
* @param angleDegrees The angle of rotation in degrees.
**/
void Zappy::Model3D::rotateModelX(float angleDegrees)
{
    _model.transform = MatrixMultiply(_model.transform, MatrixRotateX(angleDegrees * DEG2RAD));
}
