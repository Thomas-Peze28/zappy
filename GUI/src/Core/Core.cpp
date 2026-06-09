/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Core
*/

#include "Core.hpp"

bool Zappy::Core::_signal = false;

/**
* Displays help information for the Zappy GUI.
* @param argc The number of command-line arguments.
* @param argv The array of command-line arguments.
* @return True if help was displayed, false otherwise.
**/
bool Zappy::Core::displayHelp(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "USAGE: ./zappy_gui -p <port> -h <host>" << std::endl;
        return true;
    }
    for (unsigned int i = 0; argv[i] != NULL; i++)
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            std::cout << "USAGE: ./zappy_gui -p <port> -h <host>" << std::endl;
            return true;
        }
    return false;
}

int Zappy::Core::display()
{
    std::cout << "Display." << std::endl;

    Zappy::Window window(1280, 720, "Zappy - 3D GRAPHICS", 60);
    Zappy::Camera camera({15.0f, 15.0f, 15.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f);

    Zappy::Model3D hexTile("assets/object/GrassTile.glb");

    // Dimensions de la carte
    int mapWidth = 10;
    int mapHeight = 10;

    // Espacement entre chaque tuile pour des couloirs de vide égaux
    float gap = 0.3f;

    // Calcul des dimensions totales de la grille pour pouvoir la centrer
    float totalWidth = (mapWidth - 1) * (hexTile.getWidth() + gap);
    float totalDepth = (mapHeight - 1) * (hexTile.getDepth() + gap);

    // Boucle principale
    while (!WindowShouldClose() && !_signal) {
        camera.update(CAMERA_ORBITAL);
        window.beginDrawing();
            window.clear(SKYBLUE);
            camera.beginMode();    
            // Dessin de la grille de tuiles
            for (int y = 0; y < mapHeight; y++) {
                for (int x = 0; x < mapWidth; x++) {
                    float xPos = x * (hexTile.getWidth() + gap) - (totalWidth / 2.0f);
                    float zPos = y * (hexTile.getDepth() + gap) - (totalDepth / 2.0f);
                    hexTile.draw({xPos, 0.0f, zPos}, 1.0f, WHITE);
                }
            }
            EndMode3D();
            window.drawFPS(10, 10);
        window.endDrawing();
    }
    return 0;
}

/**
* Handles the signal for interrupting the Zappy GUI.
* @param signal The signal received.
**/
void Zappy::Core::signalHandler(int signal)
{
    if (signal == SIGINT) {
        _signal = true;
    }
}

/**
* Runs the Zappy GUI.
* @return 0 on success, or an error code on failure.
**/
int Zappy::Core::run()
{
    std::signal(SIGINT, signalHandler);
    std::cout << "Zappy GUI is running with Raylib..." << std::endl;

    display();
    return 0;
}
