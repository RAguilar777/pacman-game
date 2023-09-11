#pragma once

// Everything related to the map is here

// Creates the Map
void create_map(const std::array<std::array<Cell, HEIGHT>, WIDTH>& i_map, sf::RenderWindow& i_window);

// Map Collision
bool map_collision(bool pellets, bool door, short x, short y, std::array<std::array<Cell, HEIGHT>, WIDTH>& i_map);