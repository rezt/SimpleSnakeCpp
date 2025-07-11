#include <iostream>
#include <string>

#define STARTING_LENGTH 5
// TODO: Define map clear depending on the system


const int map_width = 60, map_height = 20;
constexpr int map_size = map_width * map_height;
constexpr int starting_position = 4 + (4 * map_width);
const int wall_number = map_size + 1;

enum class Direction {
	Left,
	Down,
	Up,
	Right
};

class Snake {
	private:
		Direction heading;
		int position;
		int length;

public:
	Snake(Direction start_heading, int start_position, int start_length)
		: heading(start_heading), position(start_position), length(start_length) {}
	void move() {
		switch (heading) {
		case Direction::Left:
			position--;
			break;
		case Direction::Down:
			position += map_width;
			break;
		case Direction::Up:
			position -= map_width;
			break;
		case Direction::Right:
			position++;
			break;
		}
	}
	int get_position() const {
		return position;
	}
	int get_length() const {
		return length;
	}
	void set_heading(Direction new_heading) {
		heading = new_heading;
	}
};

class Game {
private:
	Snake snake;
	int map[map_size] = { 0 };
	char buffer[map_size + map_height];
	bool game_over = false;

public:
	Game(Snake& snake) : snake(snake) {
		draw_snake();
		generate_walls();
		prepare_map_buffer();
	}

	void prepare_map_buffer() {
		for (int y = 0; y < map_height; y++) {
			for (int x = 0; x < map_width; x++) {
				int position = y * map_width + x;
				switch (map[position]) {
				case 0:
					buffer[position] = '.';
					break;
				case wall_number:
					buffer[position] = '#';
					break;
				default:
					map[position]--;
					buffer[position] = 'O';
				}
			}
		}
	}

	void generate_walls() {
		for (int y = 0; y < map_height; y++) {
			if (y == 0 || y == map_height - 1) {
				for (int x = 0; x < map_width; x++) {
					int position = y * map_width + x;
					map[position] = wall_number;
				}
				continue;
			}
			for (int x = 0; x < map_width; x++) {
				if (x == 0 || x == map_width - 1) {
					map[y * map_width + x] = wall_number;
					continue;
				}
			}
		}
	}

	void draw_snake() {
		map[snake.get_position()] = snake.get_length();
	}

	void draw_map() {
		for (int y = 0; y < map_height; y++) {
			for (int x = 0; x < map_width; x++) {
				printf("%c", buffer[y * map_width + x]);
			}
			printf("\n");
		}
	}

	void player_input() {
		// TODO: Implement player input handling
	}

	void game_logic() {
		// TODO: Implement collion detection, food generation, and snake growth
		while (!game_over) {
			system("cls");
			player_input();
			snake.move();
			draw_snake();
			prepare_map_buffer();
			draw_map();
		}
	}

};





int main(int argc, char** argv) {
	Snake snake(Direction::Right, starting_position, STARTING_LENGTH);
	Game game(snake);
	// TODO: Implement Welcome message and instructions
	game.draw_map();
	game.game_logic();
}