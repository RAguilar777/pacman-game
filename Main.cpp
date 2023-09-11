#include <chrono>
#include <SFML/Graphics.hpp>
#include "Global.hpp"

int main() {
	unsigned lag = 0;

	std::chrono::time_point<std::chrono::steady_clock> prev_time;

	std::array<std::string, HEIGHT> map;

	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(CELL_SIZE * WIDTH * SCREEN_RESIZE, CELL_SIZE * HEIGHT));

}