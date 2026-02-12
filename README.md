#  MiniArène2D – Intelligence Artificielle & FSM

##  Description du projet

**MiniArène2D** est un jeu 2D développé en **C++17** avec la bibliothèque **SFML 3.2**.

Le projet met en œuvre une **Finite State Machine (FSM)** permettant de contrôler plusieurs personnages non-joueurs (PNJ) dans une arène.  
Le joueur est contrôlé au clavier et interagit avec des ennemis pilotés par une intelligence artificielle modulaire et réutilisable.

---

##  Technologies utilisées

- **C++17**
- **SFML 3.2**
- **Visual Studio 2022**
- Plateforme cible : **Windows x64**

---

##  Prérequis

Avant de compiler le projet, assurez-vous d’avoir :

- **Visual Studio 2022**
- **SFML 3.2**
- Une configuration en **x64**

- #  Configuration de SFML

1. Télécharger SFML 3.2
2. Extraire la bibliothèque (exemple : `C:\SFML-3.2`)
3. Configurer le projet dans Visual Studio

### Include Directories

C:\SFML-3.2\include

### Library Directories

C:\SFML-3.2\lib

### Linker → Additional Dependencies

sfml-graphics.lib
sfml-window.lib
sfml-system.lib

---

#  Compilation

1. Ouvrir le fichier `.sln`
2. Sélectionner :
   - Configuration : **Release**
   - Plateforme : **x64**
3. Build Solution

---

#  Lancer le jeu

- Appuyer sur **Ctrl + F5**

---
