#include <array>
#include <string>
#include <SFML/Graphics.hpp>
#include "header-files/Global.hpp"
#include "header-files/Pacman.hpp"
#include "header-files/Sketch.hpp"

std::array<std::array<Cell, HEIGHT>, WIDTH> convert_sketch(const std::array<std::string, HEIGHT>& i_map_sketch, std::array<Position, 4>& i_ghost_positions, Pacman& i_pacman)
{
	//Is it okay if I put {} here? I feel like I'm doing something illegal.
	//But if I don't put it there, Visual Studio keeps saying "lOcAl vArIaBlE Is nOt iNiTiAlIzEd".
	std::array<std::array<Cell, HEIGHT>, WIDTH> output_map{};

	for (unsigned char a = 0; a < HEIGHT; a++)
	{
		for (unsigned char b = 0; b < WIDTH; b++)
		{
			// By default, every cell is empty.
			output_map[b][a] = Cell::Empty;

			switch (i_map_sketch[a][b])
			{
			case '#':
			{
				output_map[b][a] = Cell::Wall;

				break;
			}
			case '=':
			{
				output_map[b][a] = Cell::Door;

				break;
			}
			case '.':
			{
				output_map[b][a] = Cell::Pellet;

				break;
			}
			// Red ghost
			case '0':
			{
				i_ghost_positions[0].x = CELL_SIZE * b;
				i_ghost_positions[0].y = CELL_SIZE * a;

				break;
			}
			// Pink ghost
			case '1':
			{
				i_ghost_positions[1].x = CELL_SIZE * b;
				i_ghost_positions[1].y = CELL_SIZE * a;

				break;
			}
			// Cyan ghost
			case '2':
			{
				i_ghost_positions[2].x = CELL_SIZE * b;
				i_ghost_positions[2].y = CELL_SIZE * a;

				break;
			}
			// Orange ghost
			case '3':
			{
				i_ghost_positions[3].x = CELL_SIZE * b;
				i_ghost_positions[3].y = CELL_SIZE * a;

				break;
			}
			// Pacman!
			case 'P':
			{
				i_pacman.set_position(CELL_SIZE * b, CELL_SIZE * a);

				break;
			}
			case 'o':
			{
				output_map[b][a] = Cell::Energizer;
			}
			}
		}
	}

	return output_map;
}