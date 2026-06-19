/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** Hud
*/

#include "Hud.hpp"

using Zappy::Input;

Zappy::Hud::Hud()
{
	_open = false;
	_panelWidth = (float)GetScreenWidth() / 4.0f;
	_currentX = (float)GetScreenWidth();
	_targetX = _currentX;
	_animSpeed = 900.0f;
	_buttonRect = { (float)GetScreenWidth() - 50.0f, 10.0f, 40.0f, 40.0f };
	_panelColor = { 0, 0, 0, 150 };
}

Zappy::Hud::~Hud()
{
}

void Zappy::Hud::update()
{
	_panelWidth = (float)GetScreenWidth() / 4.0f;
	_buttonRect.x = (float)GetScreenWidth() - (_buttonRect.width + 10.0f);
	Vector2 mouse = Input::getMousePosition();
	if (Input::isMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (mouse.x >= _buttonRect.x && mouse.x <= _buttonRect.x + _buttonRect.width
			&& mouse.y >= _buttonRect.y && mouse.y <= _buttonRect.y + _buttonRect.height) {
			_open = !_open;
			if (_open)
				_targetX = (float)GetScreenWidth() - _panelWidth;
			else
				_targetX = (float)GetScreenWidth();
		}
	}
	float dt = GetFrameTime();
	float diff = _targetX - _currentX;
	if (fabs(diff) > 0.5f) {
		float move = _animSpeed * dt;
		if (fabs(diff) <= move)
			_currentX = _targetX;
		else {
            if (diff > 0)
                _currentX += move;
            else
                _currentX -= move;
        }
	}
}

void Zappy::Hud::draw() const
{
	DrawRectangleRec(_buttonRect, LIGHTGRAY);
	float cx = _buttonRect.x + 8.0f;
	for (int i = 0; i < 3; ++i) {
		DrawRectangle((int)cx, (int)(_buttonRect.y + 8 + i * 10), 24, 3, BLACK);
	}
	Rectangle panel = { _currentX, 0.0f, _panelWidth, (float)GetScreenHeight() };
	DrawRectangleRec(panel, _panelColor);
}

bool Zappy::Hud::isOpen() const
{
	return _open;
}
