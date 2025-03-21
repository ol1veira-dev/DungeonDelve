// This file is part of DungeonDelveC.
// Copyright (C) 2024 - 2025 Guilherme Oliveira Santos

// DungeonDelveC is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"
#include "stdint.h"

//==============================================================================
// Forward declaration of the structs, this is necessary to avoid circular      
// dependencies and to allow the structs to reference each other or reference 
// themselves (speacially in the case of function pointers)
// 
// Oriented objects programming in C goes BRUH 
//
typedef struct Entity Entity;
typedef struct Enemy Enemy;
typedef struct Player Player;
typedef struct Tile Tile;
typedef struct MapNode MapNode;
typedef struct GameVariables GameVariables;

#define MAX_INPUT_CHARS 12

typedef enum {
    MENU_MAIN,
    MENU_DIFFICULTY,
    MENU_WORLD_SETTINGS,
    MENU_MULTIPLAYER,
    MENU_HOSTING,
    MENU_CONNECTING,
} MenuState;
//
//
//==============================================================================

struct Entity{
    Texture2D texture;      // Texture to represent the player
    Rectangle frameRec;     // Sprite frame rectangle (Used to animate the player)   
    Vector2 spawn_point;   
    Vector2 position;       
    Vector2 last_position;  
    Sound take_damage_sound;
    Sound death_sound;
    float health;           // health is always a number % 0.5 == 0             
    float stamina;          // stamina is always a number % 0.5 == 0
    float mana;             // mana is always a number % 0.5 == 0
    float damage;             
    float speed;
    bool isAlive;
    bool isAttacking;
    bool isMoving;
    bool isPlayer;  
};

struct Enemy {
    Entity entity;          // Entity struct to store the enemy information
    unsigned int Y_frame;    // Current frame of the enemy
};

struct Player {
    Entity entity;          // Entity struct to store the player information
    Sound* walk_sounds;     // Vector of the player walking
    Sound attack_sound;     // Sound of the player attacking
    unsigned int last_animation;     // Last animation of the player
    int current_animation;  // Current animation of the player

    uint8_t *(*update)(Player*, float, unsigned int, MapNode *map);  // Function pointer to update the player
    void (*updateCamera)(Camera2D*, Player*, float);
    void (*draw)(Player*);                // Function pointer to draw the player

};

struct Tile{
    Rectangle rect;         // Rectangle of the tile (Used for collision detection)
    bool blocking;          // Used to check if the tile can block entities
    bool isBreakable;       // Used to check if the tile can be broken
    bool isStair;           // Used to check if the tile is a stair
    bool isHole;            // Used to check if the tile is a hole
};

struct MapNode{
    Tile** tile_info;       // Tile info is used to store the tile information (blocking, breakable, etc.)
    Vector2** positions;    // Positions of the each tile in the map, calculated with: (x * tile_size, y * tile_size)
    Texture2D* textures;    // Textures of the tiles
    int node_id;            // ID of the node, will be used in the future to identify new maps when the player uses a stair  
    int matrix_width;       // Width of the matrix
    int matrix_height;      // The height and width are the same because the map is a square
    int** matrix;           // Matrix is used to store the map layout (0:void, (1-8): floor, (9-11): wall, etc.)
    int num_enemies;        // Number of enemies in the map
    Enemy** enemies;         // Array of enemies in the map

    void (*updateEnemies)(MapNode*, float, unsigned int, Player*); // Function pointer to update the enemies in the map
    void (*drawEnemies)(MapNode*);                                  // Function pointer to draw the enemies in the map
    void (*drawMap)(MapNode*, Camera2D);                            // Function pointer to draw the map
};

struct GameVariables{
    unsigned int current_frame;
    unsigned int frame_counter;
    float delta_time;
    void (*update)(GameVariables*); // Function pointer to update the game variables (delta_time, frame_counter, current_frame

};


typedef struct {
    MenuState currentState;  // New state tracker
    bool isRaining;
    
    bool isServer;
    bool isClient;
    char serverIP[16];
    
    char ipInput[MAX_INPUT_CHARS + 1];
    int sock;
    int letterCount;
    int MapSize;
    int MapSeed;
    int MaxSeed;
    int MapMaxSize;
    int selectedOption;
    int verticalCenter;
    int animFrames;
    int nextFrameDataOffset;
    float RainingAlpha;
    uint16_t map_level;
    uint8_t difficulty;
    uint8_t currentAnimFrame;
    uint8_t frameDelay;
    uint8_t frameCounter;
    MapNode* TileMapGraph;
    Color backgroundColor;
} MenuData;

typedef struct {
    Sound changeOptionSound;
    Sound selectOptionSound;
    Sound lightningSound;
    Music backgroundMusic;
} MenuSounds;

#endif // STRUCTS_H