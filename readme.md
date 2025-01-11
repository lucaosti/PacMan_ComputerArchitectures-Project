# Computer architectures Project

![Victory](victory.gif)

## Description

This project was developed for the Computer Systems Architecture course at Politecnico di Torino.

## Features

1. **Maze Design**:
   - The maze contains exactly **240 Standard Pills** and **6 Power Pills**, randomly placed.
   - A central "teleport" feature allows Pac-Man to exit one side of the maze and re-enter from the other.

2. **Game Mechanics**:
   - Pac-Man's movement is joystick-controlled and continues until a wall is encountered or a new direction is chosen.
   - Pill consumption increases the score by 10 (Standard Pills) or 50 (Power Pills).
   - Every 1000 points, a new life is awarded.
   - The game starts in "PAUSE" mode and can be paused/resumed using the INT0 button.

3. **Game Objectives**:
   - Clear all pills in the maze to win (Victory screen).
   - If the 60-second countdown expires, the game ends with a "Game Over" screen.

4. **Customizations**:
   - Adjustable Pac-Man speed based on the simulation hardware.
   - Fully customizable global constants defined in the `costanti.h` file.

## How to Run

1. Use the **Keil µVision** software with the **LandTiger emulator** (Compilation target: `SW_Debug`).
2. Alternatively, deploy directly to a physical LandTiger board if available.

## Author

- **Name:** [Luca Ostinelli](https://lucaosti.github.io)

## Specifications

Detailed project specifications are available in the [pdf file](specifications.pdf).