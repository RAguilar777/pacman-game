#pragma once

// Related to the map of the maze itself
constexpr unsigned unsigned char CELL_SIZE = 16;
constexpr unsigned char HEIGHT = 21;
constexpr unsigned char WIDTH = 21;


// Data related to the ghosts
constexpr unsigned char GHOST_1 = 2;
constexpr unsigned char GHOST_2 = 1;
constexpr unsigned char GHOST_3 = 4;
constexpr unsigned char GHOST_ANIMATION = 6;
constexpr unsigned char GHOST_ANIMATION_SPEED = 4;
constexpr unsigned char GHOST_SPEED = 1;
constexpr unsigned char ESCAPE_SPEED = 4;

// The higher the value, the slower the ghost.
constexpr unsigned char FRIGHTENED_SPEED = 3;

// Data Related to Pacman + Screen
constexpr unsigned char PACMAN_ANIMATION = 6;
constexpr unsigned char PACMAN_ANIMATION_SPEED = 4;
constexpr unsigned char DEATH_FRAMES = 12;
constexpr unsigned char PACMAN_SPEED = 2;
constexpr unsigned char SCREEN_RESIZE = 2;
constexpr unsigned char FONT = 16;

// More frame data
constexpr unsigned short CHASE_DURATION = 1024;
constexpr unsigned short ENERGIZE_DURATION = 512;
constexpr unsigned short FRAME_DURATION = 16667;
constexpr unsigned short FLASH_START = 64;
constexpr unsigned short LONG_SCATTER_DURATION = 512;
constexpr unsigned short SHORT_SCATTER_DURATION = 256;


// Represents maze cell
enum Cell
{
	Door,
	Empty,
	Energizer,
	Pellet,
	Wall
};

// Where pacman is currently located
struct Position
{
	short x;
	short y;

	
	bool operator==(const Position & i_position)
	{
		return this->x == i_position.x && this->y == i_position.y;
	}
};