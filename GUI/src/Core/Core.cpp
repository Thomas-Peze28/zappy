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

    // Initialisation de la fenêtre
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Zappy - 3D GRAPHICS");

    // Configuration de la caméra 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 15.0f, 15.0f, 15.0f }; // Position initiale avec du recul
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Pivot central exact autour de l'origine
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Axe vertical orienté vers le haut
    camera.fovy = 45.0f;                                // Champ de vision
    camera.projection = CAMERA_PERSPECTIVE;

    // Chargement du modèle 3D
    Model hexTile = LoadModel("assets/object/GrassTile.glb");

    SetTargetFPS(60);

    // Calcul des dimensions réelles de la tuile
    BoundingBox bbox = GetModelBoundingBox(hexTile);
    float hexWidth = bbox.max.x - bbox.min.x;
    float hexDepth = bbox.max.z - bbox.min.z;

    // Dimensions de la carte
    int mapWidth = 10;
    int mapHeight = 10;

    // Espacement entre chaque tuile pour des couloirs de vide égaux
    float gap = 0.3f;

    // Calcul des dimensions totales de la grille pour pouvoir la centrer
    float totalWidth = (mapWidth - 1) * (hexWidth + gap);
    float totalDepth = (mapHeight - 1) * (hexDepth + gap);

    // Boucle principale
    while (!WindowShouldClose() && !_signal) {
        // Mise à jour de la caméra orbitale autour du centre
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();
            ClearBackground(SKYBLUE);

            BeginMode3D(camera);
                
                // Dessin de la grille de tuiles
                for (int y = 0; y < mapHeight; y++) {
                    for (int x = 0; x < mapWidth; x++) {
                        float xPos = x * (hexWidth + gap) - (totalWidth / 2.0f);
                        float zPos = y * (hexDepth + gap) - (totalDepth / 2.0f);
                        DrawModel(hexTile, (Vector3){ xPos, 0.0f, zPos }, 1.0f, WHITE);
                    }
                }

            EndMode3D();

            DrawFPS(10, 10); // Affiche les FPS 
        EndDrawing();
    }
    UnloadModel(hexTile);
    CloseWindow();
    return 0;
}
