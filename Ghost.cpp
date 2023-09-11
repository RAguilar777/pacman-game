#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "header-files/Global.hpp"
#include "header-files/Pacman.hpp"
#include "header-files/Ghost.hpp"
#include "header-files/Map.hpp"

Ghost::Ghost(unsigned char i_id):
	id(i_id)
{
}

bool Ghost::pacman_collision(const Position & i_pacman_position)
{
	// Collision checking algorithm.
	if (position.x > i_pacman_position.x - CELL_SIZE && position.x < CELL_SIZE + i_pacman_position.x)
	{
		if (position.y > i_pacman_position.y - CELL_SIZE && position.y < CELL_SIZE + i_pacman_position.y)
		{
			return 1;
		}
	}

	return 0;
}

float Ghost::get_target_distance(unsigned char i_direction)
{
	short x = position.x;
	short y = position.y;

	// Moves the ghost and determines distance from pacman
	switch (i_direction)
	{
	case 0:
	{
		x += GHOST_SPEED;

		break;
	}
	case 1:
	{
		y -= GHOST_SPEED;

		break;
	}
	case 2:
	{
		x -= GHOST_SPEED;

		break;
	}
	case 3:
	{
		y += GHOST_SPEED;
	}
	}

	// Pythagoras' theorem was used here
	return static_cast<float>(sqrt(pow(x - target.x, 2) + pow(y - target.y, 2)));
}

// Creates the ghost
void Ghost::draw(bool i_flash, sf::RenderWindow& i_window)
{
	//Current frame of the animation.
	unsigned char body_frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(GHOST_ANIMATION_SPEED)));

	sf::Sprite body;
	sf::Sprite face;

	sf::Texture texture;
	texture.loadFromFile("resources/Ghost" + std::to_string(CELL_SIZE) + ".png");

	body.setTexture(texture);
	body.setPosition(position.x, position.y);
	//Animation is basically a quick display of similar images.
	//So that's what we're doing here.
	body.setTextureRect(sf::IntRect(CELL_SIZE * body_frame, 0, CELL_SIZE, CELL_SIZE));

	face.setTexture(texture);
	face.setPosition(position.x, position.y);

	if (0 == freightened_mode)
	{
		switch (id)
		{
		case 0:
		{
			// Red color
			body.setColor(sf::Color(255, 0, 0));

			break;
		}
		case 1:
		{
			// Pink color
			body.setColor(sf::Color(255, 182, 255));

			break;
		}
		case 2:
		{
			// Cyan color 
			body.setColor(sf::Color(0, 255, 255));

			break;
		}
		case 3:
		{
			// Orange color
			body.setColor(sf::Color(255, 182, 85));
		}
		}

		face.setTextureRect(sf::IntRect(CELL_SIZE * direction, CELL_SIZE, CELL_SIZE, CELL_SIZE));

		i_window.draw(body);
	}
	else if (1 == freightened_mode)
	{
		body.setColor(sf::Color(36, 36, 255));
		// The face remains the same regardless of where the ghost is going right now.
		face.setTextureRect(sf::IntRect(4 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

		if (1 == i_flash && 0 == body_frame % 2)
		{
			body.setColor(sf::Color(255, 255, 255));
			face.setColor(sf::Color(255, 0, 0));
		}
		else
		{
			body.setColor(sf::Color(36, 36, 255));
			face.setColor(sf::Color(255, 255, 255));
		}

		i_window.draw(body);
	}
	else
	{
		face.setTextureRect(sf::IntRect(CELL_SIZE * direction, 2 * CELL_SIZE, CELL_SIZE, CELL_SIZE));
	}

	i_window.draw(face);

	// This is to prevent overflowing
	animation_timer = (1 + animation_timer) % (GHOST_ANIMATION * GHOST_ANIMATION_SPEED);
}

void Ghost::reset(const Position& i_home, const Position& i_home_exit)
{
	movement_mode = 0;
	// Everyone can use the door except the red gohst.
	use_door = 0 < id;

	direction = 0;
	int frightened_mode = 0;
	int frightened_speed_timer = 0;

	animation_timer = 0;

	home = i_home;
	home_exit = i_home_exit;
	target = i_home_exit;
}

void Ghost::set_position(short i_x, short i_y)
{
	position = { i_x, i_y };
}

void Ghost::switch_mode()
{
	movement_mode = 1 - movement_mode;
}

void Ghost::update(unsigned char i_level, std::array<std::array<Cell, HEIGHT>, WIDTH> & i_map, Ghost & i_ghost_0, Pacman & i_pacman)
{
	bool move = 0;

	// If this is greater than 1, that means that the ghost has reached the intersection.
	// We don't consider the way back as an available way.
	unsigned char available_ways = 0;
	unsigned char speed = GHOST_SPEED;

	std::array<bool, 4> walls{};

	// Here the ghost starts and stops being frightened.
	if (0 == freightened_mode && i_pacman.get_energizer_timer() == ENERGIZE_DURATION / pow(2, i_level))
	{
		freightened_timer = FRIGHTENED_SPEED;

		freightened_mode = 1;
	}
	else if (0 == i_pacman.get_energizer_timer() && 1 == freightened_mode)
	{
		freightened_mode = 0;
	}

	// The modulo operator is used in case the gohst goes outside the grid.
	if (2 == freightened_mode && 0 == position.x % ESCAPE_SPEED && 0 == position.y % ESCAPE_SPEED)
	{
		speed = ESCAPE_SPEED;
	}

	update_target(i_pacman.get_direction(), i_ghost_0.get_position(), i_pacman.get_position());

	walls[0] = map_collision(0, use_door, speed + position.x, position.y, i_map);
	walls[1] = map_collision(0, use_door, position.x, position.y - speed, i_map);
	walls[2] = map_collision(0, use_door, position.x - speed, position.y, i_map);
	walls[3] = map_collision(0, use_door, position.x, speed + position.y, i_map);

	if (1 != freightened_mode)
	{
		// 4 was used because using a number between 0 and 3 will make the gohst move in a direction it can't move.
		unsigned char optimal_direction = 4;

		move = 1;

		for (unsigned char a = 0; a < 4; a++)
		{
			// Ghosts can't turn back!
			if (a == (2 + direction) % 4)
			{
				continue;
			}
			else if (0 == walls[a])
			{
				if (4 == optimal_direction)
				{
					optimal_direction = a;
				}

				available_ways++;

				if (get_target_distance(a) < get_target_distance(optimal_direction))
				{
					// The optimal direction is the direction that's closest to the target.
					optimal_direction = a;
				}
			}
		}

		if (1 < available_ways)
		{
			// When the ghost is at the intersection, it chooses the optimal direction.
			direction = optimal_direction;
		}
		else
		{
			if (4 == optimal_direction)
			{
				// "Unless they have to" part.
				direction = (2 + direction) % 4;
			}
			else
			{
				direction = optimal_direction;
			}
		}
	}
	else
	{
		// Used rand to avoid including the entire library for a small segment of the code
		unsigned char random_direction = rand() % 4;

		if (0 == freightened_timer)
		{
			//The ghost can move after a certain number of frames.
			move = 1;

			freightened_timer = FRIGHTENED_SPEED;

			for (unsigned char a = 0; a < 4; a++)
			{
				//  Ghost can't turn back even if they're frightened.
				if (a == (2 + direction) % 4)
				{
					continue;
				}
				else if (0 == walls[a])
				{
					available_ways++;
				}
			}

			if (0 < available_ways)
			{
				while (1 == walls[random_direction] || random_direction == (2 + direction) % 4)
				{
					// Continuously picking a random direction until we can use it.
					random_direction = rand() % 4;
				}

				direction = random_direction;
			}
			else
			{
				// If there's no other way, it turns back.
				direction = (2 + direction) % 4;
			}
		}
		else
		{
			freightened_timer--;
		}
	}

	// If possible, move the ghost
	if (1 == move)
	{
		switch (direction)
		{
		case 0:
		{
			position.x += speed;

			break;
		}
		case 1:
		{
			position.y -= speed;

			break;
		}
		case 2:
		{
			position.x -= speed;

			break;
		}
		case 3:
		{
			position.y += speed;
		}
		}

		// Warping.
		// When the ghost leaves the map, we move it to the other side.
		if (-CELL_SIZE >= position.x)
		{
			position.x = CELL_SIZE * WIDTH - speed;
		}
		else if (position.x >= CELL_SIZE * WIDTH)
		{
			position.x = speed - CELL_SIZE;
		}
	}

	if (1 == pacman_collision(i_pacman.get_position()))
	{
		// When ghost interacts with Pacman, he dies
		if (0 == freightened_mode)
		{
			i_pacman.set_dead(1);
		}
		// Otherwise, the ghost starts running towards the house
		else 
		{
			use_door = 1;

			freightened_mode = 2;

			target = home;
		}
	}
}

void Ghost::update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position)
{
	// If the ghost can use the door.
	if (1 == use_door) 
	{
		if (position == target)
		{
			// If the ghost reaches the exit, it can no longer use the door
			if (home_exit == target) 
			{
				use_door = 0; 
			}
			// If the ghost has reached its home, it stops being frightened and leaves the house
			else if (home == target)
			{
				freightened_mode = 0; 

				target = home_exit; 
			}
		}
	}
	else
	{
		// Scatter mode
		if (0 == movement_mode) 
		{
			// Each ghost goes to its assigned corner
			switch (id)
			{
			case 0:
			{
				target = { CELL_SIZE * (WIDTH - 1), 0 };

				break;
			}
			case 1:
			{
				target = { 0, 0 };

				break;
			}
			case 2:
			{
				target = { CELL_SIZE * (WIDTH - 1), CELL_SIZE * (HEIGHT - 1) };

				break;
			}
			case 3:
			{
				target = { 0, CELL_SIZE * (HEIGHT - 1) };
			}
			}
		}
		// Chase mode
		else 
		{
			switch (id)
			{
			// The red ghost will chase Pacman
			case 0: 
			{
				target = i_pacman_position;

				break;
			}
			// The pink ghost will chase the 4th cell in front of Pacman.
			case 1: 
			{
				target = i_pacman_position;

				switch (i_pacman_direction)
				{
				case 0:
				{
					target.x += CELL_SIZE * GHOST_1;

					break;
				}
				case 1:
				{
					target.y -= CELL_SIZE * GHOST_1;

					break;
				}
				case 2:
				{
					target.x -= CELL_SIZE * GHOST_1;

					break;
				}
				case 3:
				{
					target.y += CELL_SIZE * GHOST_1;
				}
				}

				break;
			}
			// The blue ghost pathing; second cell in front of Pacman
			case 2: 
			{
				target = i_pacman_position;

				switch (i_pacman_direction)
				{
				case 0:
				{
					target.x += CELL_SIZE * GHOST_2;

					break;
				}
				case 1:
				{
					target.y -= CELL_SIZE * GHOST_2;

					break;
				}
				case 2:
				{
					target.x -= CELL_SIZE * GHOST_2;

					break;
				}
				case 3:
				{
					target.y += CELL_SIZE * GHOST_2;
				}
				}

				// We're sending a vector from the red ghost to the second cell in front of Pacman.
				// Then doubling it.
				target.x += target.x - i_ghost_0_position.x;
				target.y += target.y - i_ghost_0_position.y;

				break;
			}
			// Orange ghost will chase Pacman until it gets close to him, then switches to scatter mode
			case 3: 
			{
				// Using the Pythagoras' theorem again.
				if (CELL_SIZE * GHOST_3 <= sqrt(pow(position.x - i_pacman_position.x, 2) + pow(position.y - i_pacman_position.y, 2)))
				{
					target = i_pacman_position;
				}
				else
				{
					target = { 0, CELL_SIZE * (HEIGHT - 1) };
				}
			}
			}
		}
	}
}

Position Ghost::get_position()
{
	return position;
}