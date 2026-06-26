/*
** EPITECH PROJECT, 2026
** G-YEP-400-LIL-4-1-zappy-4
** File description:
** Hud
*/

#include "Hud.hpp"
#include "Game.hpp"
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
		int fontSize = 15;
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

/**
* @brief Draws the inventory for a specific tile.
* @param x The x-coordinate of the tile.
* @param y The y-coordinate of the tile.
* @param tile The tile instance.
**/
void Zappy::Hud::drawTileInventory(int x, int y, const Zappy::Tile &tile) const
{
    int panelWidth = 250;
    int panelHeight = 350;
    int startX = 10;
    int startY = 40;

    Zappy::Shape3D::drawRectangle(startX, startY, panelWidth, panelHeight, Fade(BLACK, 0.7f));
    Zappy::Shape3D::drawRectangleLines(startX, startY, panelWidth, panelHeight, WHITE);

    std::string title = "=== TILE [" + std::to_string(x) + ", " + std::to_string(y) + "] ===";
    DrawText(title.c_str(), startX + 20, startY + 20, 20, RAYWHITE);

    int textY = startY + 70;
    int fontSize = 20;
    int spacing = 35;

    auto drawResourceInfo = [&](const std::string& name, Zappy::RESSOURCE res, Color color) {
        std::string line = name + " : " + std::to_string(tile.getResource(res));
        DrawText(line.c_str(), startX + 20, textY, fontSize, color);
        textY += spacing;
    };

    drawResourceInfo("Food", Zappy::RESSOURCE::FOOD, ORANGE);
    drawResourceInfo("Linemate", Zappy::RESSOURCE::LINEMATE, PURPLE);
    drawResourceInfo("Deraumere", Zappy::RESSOURCE::DERAUMERE, BLUE);
    drawResourceInfo("Sibur", Zappy::RESSOURCE::SIBUR, GREEN);
    drawResourceInfo("Mendiane", Zappy::RESSOURCE::MENDIANE, RED);
    drawResourceInfo("Phiras", Zappy::RESSOURCE::PHIRAS, GOLD);
    drawResourceInfo("Thystame", Zappy::RESSOURCE::THYSTAME, SKYBLUE);
}

/**
* @brief Draws the inventory for a specific player.
* @param player The player instance.
* @param game The game instance.
**/
void Zappy::Hud::drawPlayerInventory(const Zappy::Player &player, const Zappy::Game &game) const
{
    int panelWidth = 250;
    int panelHeight = 350;
    int startX = GetScreenWidth() - panelWidth - 10;
    int startY = 40;

    Zappy::Shape3D::drawRectangle(startX, startY, panelWidth, panelHeight, Fade(DARKBLUE, 0.7f));
    Zappy::Shape3D::drawRectangleLines(startX, startY, panelWidth, panelHeight, SKYBLUE);

    std::string title = "Player #" + std::to_string(player.getId());
	DrawText(title.c_str(), startX + 20, startY + 20, 22, YELLOW);

    int textY = startY + 60;
    DrawText(("Team: " + player.getTeamName()).c_str(), startX + 20, textY, 18, WHITE);
    textY += 25;
    DrawText(("Level: " + std::to_string(player.getLevel())).c_str(), startX + 20, textY, 18, WHITE);
    textY += 35;

    DrawText("--- Inventory ---", startX + 20, textY, 18, LIGHTGRAY);
    textY += 30;

    int fontSize = 20;
    int spacing = 25;

    auto drawResourceInfo = [&](const std::string &name, Zappy::RESSOURCE res, Color color, int row) {
        std::string line = name + " : " + std::to_string(player.getResource(res)) + " (" + std::to_string(game.getTeamRessource(player.getTeamName(), res)) + ")";
        DrawText(line.c_str(), startX + 20, textY + spacing * row, fontSize, color);
    };

    drawResourceInfo("Food", Zappy::RESSOURCE::FOOD, ORANGE, 0);
    drawResourceInfo("Linemate", Zappy::RESSOURCE::LINEMATE, PURPLE, 1);
    drawResourceInfo("Deraumere", Zappy::RESSOURCE::DERAUMERE, BLUE, 2);
    drawResourceInfo("Sibur", Zappy::RESSOURCE::SIBUR, GREEN, 3);
    drawResourceInfo("Mendiane", Zappy::RESSOURCE::MENDIANE, RED, 4);
    drawResourceInfo("Phiras", Zappy::RESSOURCE::PHIRAS, GOLD, 5);
    drawResourceInfo("Thystame", Zappy::RESSOURCE::THYSTAME, SKYBLUE, 6);
}

/**
* @brief Draws the winning team message.
* @param teamName The name of the winning team.
**/
void Zappy::Hud::drawWinningTeam(const std::string &teamName) const
{
	int panelWidth = 400;
	int panelHeight = 200;
	int startX = (GetScreenWidth() - panelWidth) / 2;
	int startY = (GetScreenHeight() - panelHeight) / 2;

	Zappy::Shape3D::drawRectangle(startX, startY, panelWidth, panelHeight, Fade(BLACK, 0.8f));
	Zappy::Shape3D::drawRectangleLines(startX, startY, panelWidth, panelHeight, WHITE);

	std::string title = "=== GAME OVER ===";
	DrawText(title.c_str(), startX + 20, startY + 20, 30, RED);

	std::string message = "Winning Team: " + teamName;
	DrawText(message.c_str(), startX + 20, startY + 80, 24, YELLOW);
}