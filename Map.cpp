#include <array>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "header-files/Global.hpp"


// Creates the Map
void draw_map(const std::array<std::array<Cell, HEIGHT>, WIDTH>& i_map, sf::RenderWindow& i_window)
{
	sf::Sprite sprite;

	sf::Texture texture;
	texture.loadFromFile("resources/Map" + std::to_string(CELL_SIZE) + ".png");

	sprite.setTexture(texture);

	for (unsigned char a = 0; a < WIDTH; a++)
	{
		for (unsigned char b = 0; b < HEIGHT; b++)
		{
			sprite.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));

			// Crop out what we need from the texture.
			switch (i_map[a][b])
			{
			case Cell::Door:
			{
				sprite.setTextureRect(sf::IntRect(2 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

				i_window.draw(sprite);

				break;
			}
			case Cell::Energizer:
			{
				sprite.setTextureRect(sf::IntRect(CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

				i_window.draw(sprite);

				break;
			}
			case Cell::Pellet:
			{
				sprite.setTextureRect(sf::IntRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE));

				i_window.draw(sprite);

				break;
			}
			case Cell::Wall:
			{
				bool down = 0;
				bool left = 0;
				bool right = 0;
				bool up = 0;

				if (b < HEIGHT - 1)
				{
					if (Cell::Wall == i_map[a][1 + b])
					{
						down = 1;
					}
				}

				// Since we have warp tunnels, we need to draw them as if they're connected
				if (0 < a)
				{
					if (Cell::Wall == i_map[a - 1][b])
					{
						left = 1;
					}
				}
				else
				{
					left = 1;
				}

				if (a < WIDTH - 1)
				{
					if (Cell::Wall == i_map[1 + a][b])
					{
						right = 1;
					}
				}
				else
				{
					right = 1;
				}

				if (0 < b)
				{
					if (Cell::Wall == i_map[a][b - 1])
					{
						up = 1;
					}
				}

				// DISTRIBUTIVE PROPERTY!
				sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));

				i_window.draw(sprite);
			}
			}
		}
	}
}


// Map Collision
bool map_collision(bool i_collect_pellets, bool i_use_door, short i_x, short i_y, std::array<std::array<Cell, HEIGHT>, WIDTH>& i_map)
{
	bool output = 0;

	// Getting the exact position.
	float cell_x = i_x / static_cast<float>(CELL_SIZE);
	float cell_y = i_y / static_cast<float>(CELL_SIZE);

	// A ghost/Pacman can intersect 4 cells at most.
	for (unsigned char a = 0; a < 4; a++)
	{
		short x = 0;
		short y = 0;

		switch (a)
		{
		// Top left cell
		case 0: 
		{
			x = static_cast<short>(floor(cell_x));
			y = static_cast<short>(floor(cell_y));

			break;
		}
		// Top right cell
		case 1: 
		{
			x = static_cast<short>(ceil(cell_x));
			y = static_cast<short>(floor(cell_y));

			break;
		}
		// Bottom left cell
		case 2: 
		{
			x = static_cast<short>(floor(cell_x));
			y = static_cast<short>(ceil(cell_y));

			break;
		}
		// Bottom right cell
		case 3: 
		{
			x = static_cast<short>(ceil(cell_x));
			y = static_cast<short>(ceil(cell_y));
		}
		}

		// Making sure that the position is inside the map.
		if (0 <= x && 0 <= y && HEIGHT > y && WIDTH > x)
		{
			// Here we only care about the walls
			if (0 == i_collect_pellets)
			{
				if (Cell::Wall == i_map[x][y])
				{
					output = 1;
				}
				else if (0 == i_use_door && Cell::Door == i_map[x][y])
				{
					output = 1;
				}
			}
			// Here we only care about the collectables
			else 
			{
				if (Cell::Energizer == i_map[x][y])
				{
					output = 1;

					i_map[x][y] = Cell::Empty;
				}
				else if (Cell::Pellet == i_map[x][y])
				{
					i_map[x][y] = Cell::Empty;
				}
			}
		}
	}

	return output;
}