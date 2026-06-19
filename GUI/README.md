# Zappy GUI

Client graphique du projet Zappy. Cette partie du dépôt se connecte au serveur de jeu, récupère l'état du monde en temps réel, puis l'affiche avec raylib en vue 3D.

## Architecture

### `Core`

`Core` est le contrôleur principal.

Il s'occupe de:

- parser les arguments de lancement;
- gérer l'aide `--help`;
- capter `SIGINT` pour fermer proprement l'application;
- dispatcher les messages réseau vers les bons handlers.

### `Network`

`Network` gère la connexion TCP au serveur.

Il fournit:

- une connexion au serveur via `host` et `port`;
- une lecture non bloquante des messages;
- une file interne pour consommer les messages ligne par ligne;
- l'envoi des commandes réseau du client graphique.

### `Game`

`Game` contient l'état logique affiché à l'écran:

- la carte (`Map`);
- les joueurs;
- les oeufs;
- l'unité de temps.

### `Engine`

`Engine` encapsule le rendu raylib.

Il gère:

- l'ouverture de la fenêtre;
- la caméra;
- le rendu 2D/3D;
- les modèles des tuiles, ressources, joueurs et oeufs;
- le son et la musique;
- les entrées clavier/souris.

## Dépendances

Pour compiler ce projet, il faut au minimum:

- `g++` avec support C++20;
- `make`;
- `raylib` et ses dépendances système;
- les bibliothèques liées dans le Makefile: `GL`, `m`, `pthread`, `dl`, `rt`, `X11`.

## Installation de raylib

### Ubuntu / Debian

Installation des outils de base:

```bash
sudo apt update
sudo apt install build-essential git cmake
```

Installation de la raylib si le paquet est disponible sur la distribution:

```bash
sudo apt install libraylib-dev
```

Si le paquet n'existe pas dans les dépôts, compile raylib depuis les sources:

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build
cd build
cmake .. -DBUILD_EXAMPLES=OFF
make -j"$(nproc)"
sudo make install
sudo ldconfig
```

## Compilation

Depuis le dossier `GUI`:

```bash
make
```

Le binaire produit est `zappy_gui`.

Autres règles du Makefile:

- `make clean` supprime les objets;
- `make fclean` supprime les objets et le binaire;
- `make re` force une recompilation complète;
- `make test_gui` lance un serveur de test, l'IA et le GUI.

## Lancement

Le GUI a besoin d'un serveur Zappy déjà lancé.

Affichage de l'aide:

```bash
./zappy_gui --help
```

Lancement du client graphique:

```bash
./zappy_gui -h localhost -p 5252
```

Arguments disponibles:

- `-h` ou `--host` pour l'adresse du serveur;
- `-p` ou `--port` pour le port du serveur.

## Protocole côté GUI

Au moment de la connexion, le client graphique envoie les commandes initiales attendues par le serveur:

- `GRAPHIC` pour s'annoncer comme client graphique;
- `msz` pour récupérer la taille de la carte;
- `mct` pour récupérer le contenu des tuiles;
- `tna` pour récupérer les noms des équipes;
- `sgt` pour récupérer l'unité de temps.

Ensuite, le serveur envoie les événements du monde: apparition de joueurs, déplacement, consommation de ressources, mort, ponte d'oeufs, etc. Ces messages sont convertis en état local puis affichés par l'engine.

## Structure du projet

```text
src/
├── Core/      # parsing des arguments, boucle principale, dispatch des messages
├── Engine/    # fenêtre, caméra, rendu et entrées
├── Game/      # état du monde: carte, joueurs, oeufs
├── Graphics/  # encapsulation raylib pour audio, texte, sprites, modèles, etc.
└── Network/   # connexion TCP et file de messages
```

## Dépannage

- Si la compilation échoue avec `-lraylib`, vérifie que raylib est bien installé et visible par ton linker.
- Si la fenêtre ne s'ouvre pas, vérifie la présence des dépendances X11/OpenGL sur Linux.
- Si le client affiche une erreur d'adresse, vérifie l'hôte et le port du serveur.
- Si la connexion échoue au démarrage, assure-toi que le serveur Zappy tourne déjà.

## Auteur

Ce rendu graphique vous à été proposé par :

- [@Romane Cauvez](https://github.com/Romanecauvez)
- [@Thomas Peze](https://github.com/Thomas-Peze28)