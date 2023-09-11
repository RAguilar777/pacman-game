#pragma once

// Ghosts and their activities belong here
class Ghost
{
	//It can be the scatter mode or the chase mode.
	bool movement_mode;
	//"Can I use the door, pwease?"
	bool use_door;

	//Current direction.
	unsigned char direction;
	// Freightened state
	unsigned char freightened_mode;
	//To make the ghost move more slowly, we'll move it after a certain number of frames. So we need a timer.
	unsigned char freightened_timer;
	

	unsigned char id;

	unsigned short animation_timer;

	//The ghost will go here when escaping.
	Position home;
	
	Position home_exit;

	//Current position.
	Position position;
	//Current target.
	Position target;

public:
	Ghost(unsigned char i_id);

	bool pacman_collision(const Position& i_pacman_position);

	float get_target_distance(unsigned char i_direction);

	void draw(bool i_flash, sf::RenderWindow& i_window);
	void reset(const Position& i_home, const Position& i_home_exit);
	void set_position(short i_x, short i_y);
	void switch_mode();
	void update(unsigned char i_level, std::array<std::array<Cell, HEIGHT>, WIDTH>& i_map, Ghost& i_ghost_0, Pacman& i_pacman);
	void update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position);

	Position get_position();
};


// This class manages the Ghosts for the game
class GhostManager
{
	//The ghosts will switch between the scatter mode and the chase mode before permanently chasing Pacman.
	//So we need this to keep track of the waves.
	unsigned char current_wave;

	//Damn, I really used a lot of timers.
	unsigned short wave_timer;

	std::array<Ghost, 4> ghosts;
public:
	GhostManager();

	void draw(bool i_flash, sf::RenderWindow& i_window);
	void reset(unsigned char i_level, const std::array<Position, 4>& i_ghost_positions);
	void update(unsigned char i_level, std::array<std::array<Cell, HEIGHT>, WIDTH>& i_map, Pacman& i_pacman);
};