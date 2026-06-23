/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** Hud
*/

#include "Hud.hpp"
#include "Input.hpp"

/**
* @brief Constructor for the Hud class.
**/
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

/**
* @brief Updates the HUD.
**/
void Zappy::Hud::update()
{
	_panelWidth = (float)GetScreenWidth() / 4.0f;
	_buttonRect.x = (float)GetScreenWidth() - (_buttonRect.width + 10.0f);
	Vector2 mouse = Input::getMousePosition();
	if (Input::isMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (mouse.x >= _buttonRect.x && mouse.x <= _buttonRect.x + _buttonRect.width
			&& mouse.y >= _buttonRect.y && mouse.y <= _buttonRect.y + _buttonRect.height) {
			toggleOpen();
		}
	}
	float dt = GetFrameTime();
	float diff = _targetX - _currentX;
	if (std::fabs(diff) > 0.5f) {
		float move = _animSpeed * dt;
		if (std::fabs(diff) <= move)
			_currentX = _targetX;
		else {
            if (diff > 0)
                _currentX += move;
            else
                _currentX -= move;
        }
	}
}

/**
* @brief Sets the messages to be displayed in the HUD.
* @param messages The vector of strings containing the messages.
**/
void Zappy::Hud::setMessages(const std::vector<std::string> &messages)
{
	_messages = messages;
}

/**
* @brief Draws the HUD.
**/
void Zappy::Hud::draw() const
{
	DrawRectangleRec(_buttonRect, LIGHTGRAY);
	float cx = _buttonRect.x + 8.0f;
	for (int i = 0; i < 3; ++i) {
		DrawRectangle((int)cx, (int)(_buttonRect.y + 8 + i * 10), 24, 3, BLACK);
	}
	Rectangle panel = { _currentX, 0.0f, _panelWidth, (float)GetScreenHeight() };
	DrawRectangleRec(panel, _panelColor);

	if (!_messages.empty() && _currentX < GetScreenWidth()) {
		float textX = _currentX + 16.0f;
		float textY = 16.0f;
		int fontSize = 18;
		for (const auto &message : _messages) {
			DrawText(message.c_str(), (int)textX, (int)textY, fontSize, WHITE);
			textY += fontSize + 8;
			if (textY + fontSize > GetScreenHeight())
				break;
		}
	}
}

/**
* @brief Checks if the HUD is open.
* @return True if the HUD is open, false otherwise.
*/
bool Zappy::Hud::isOpen() const
{
	return _open;
}

/**
* @brief Toggles the open state of the HUD.
**/
void Zappy::Hud::toggleOpen()
{
	_open = !_open;
	if (_open)
		_targetX = (float)GetScreenWidth() - _panelWidth;
	else
		_targetX = (float)GetScreenWidth();
}

/**
* @brief Sets the opening state of the HUD.
* @param open The new opening state of the HUD.
**/
void Zappy::Hud::setOpening(bool open)
{
	_open = open;
}

// TODO: essayer de fix le toggle bar au moment du passage en fullscreen

// void Zappy::Hud::setPanelWidth(float width)
// {
//     _panelWidth = width;
// }

// void Zappy::Hud::setButtonRectX(const Rectangle &rect)
// {
//     _buttonRect.x = rect.x;
// }

// void Zappy::Hud::setCurrentX(const Rectangle &rect)
// {
//     _currentX = rect.x;
// }

// const Rectangle &Zappy::Hud::getButtonRect(void) const
// {
//     return _buttonRect;
// }
