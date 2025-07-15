#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cstdlib>

#define STARTING_LENGTH 5
#define TICKRATE 300 // miliseconds
#define SPAWN_RATE 5 // how many ticks before spawning food


const int map_width = 60, map_height = 20;
constexpr int map_size = map_width * map_height;
constexpr int starting_position = 4 + (4 * map_width);
const int wall_number = map_size + 1;
const int food_number = map_size + 2;

enum class Direction {
	// Numbers are used to determine the axis left and right are horizontal (X % 2 = 0), up and down are vertical (X % = 1)
	Left = 0,
	Down = 1,
	Up = 3,
	Right = 2
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
	Direction get_heading() const {
		return heading;
	}

	void set_heading(Direction new_heading) {
		heading = new_heading;
	}
	
	void grow() {
		length++;
	}
};

class Game {
private:
	Snake snake;
	int map[map_size] = { 0 };
	char buffer[map_size + map_height];
	bool game_over = false;
	int tickrate = TICKRATE;
	int difficulty_adjustment = 0;

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
				case food_number:
					buffer[position] = 'x';
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

	bool check_direction(Direction new_direction) {
		if (new_direction == snake.get_heading()) {
			return true; // No change in direction
		}
		int axis = (int)snake.get_heading() % 2 == 0 ? 0 : 1; // 0 for horizontal, 1 for vertical

		switch (new_direction) {
		case Direction::Left:
		case Direction::Right:
			if (axis == 0) {
				return false;
			}
			break;
		case Direction::Down:
		case Direction::Up:
			if (axis == 1) {
				return false;
			}
			break;
		}
		return true;
	}

	void player_input() {
		char input = '_';

		Sleep(tickrate - difficulty_adjustment); // Sleep for TICKRATE miliseconds
		if (_kbhit()) { //if there is a key in keyboard buffer
			input = _getch(); //get the char
		}	

		
		switch (input) {
		case 'k':
			if (check_direction(Direction::Up)) {
				snake.set_heading(Direction::Up);
			}
			break;
		case 'j':
			if (check_direction(Direction::Down)) {
				snake.set_heading(Direction::Down);
			}
			break;
		case 'h':
			if (check_direction(Direction::Left)) {
				snake.set_heading(Direction::Left);
			}
			break;
		case 'l':
			if (check_direction(Direction::Right)) {
				snake.set_heading(Direction::Right);
			}
			break;
		case 'q':
			game_over = true;
			break;
		}
	}

	bool check_collision() {
		int position = snake.get_position();
		if (map[position] == food_number) {
			snake.grow(); // Snake eats food
			difficulty_change(); // Check if difficulty should change
			map[position] = snake.get_length(); // Update snake length at position
		} else if (map[position] != 0) { // Collision with wall or itself
			return true; // Collision detected
		}
		else {
			map[position] = snake.get_length(); // Update snake position with its length
		}
		return false;
	}

	void spawn_food() {
		int x = rand() % (map_width - 2) + 1; // Random x position, avoiding walls
		int y = rand() % (map_height - 2) + 1; // Random y position, avoiding walls
		map[y * map_width + x] = food_number; 
	}

	void game_logic() {
		// TODO: Implement collion detection, food generation, and snake growth
		int tick = 0;
		while (!game_over) {			
			player_input(); // Get player input
			system("cls");
			snake.move();
			if (check_collision()) { game_over = true; break; }// Check for collisions
			draw_snake();
			if (tick == SPAWN_RATE) {
				spawn_food();
				tick = 0;
			}
			prepare_map_buffer();
			draw_map();
			tick++;
		}
	}

	void difficulty_change() {
		if (snake.get_length() % 5 == 0 && snake.get_length() > STARTING_LENGTH) {
			if (tickrate > 50) { // Minimum tick rate to avoid too fast gameplay
				tickrate -= 50; // Increase game speed
			}
		}
	}

	int get_score() {
		return snake.get_length() - STARTING_LENGTH; // Returns the score based on the snake's length
	}

};

void welcome_message() {
	printf("Welcome to Simple Snake Game!\n");
	printf("Controls: Use 'h' for left, 'j' for down, 'k' for up, and 'l' for right. Press 'q' to quit.\n");
	printf("\nPress any key to start...\n");
	_getch(); // Wait for user input
	system("cls"); // Clear the console
}

void score_screen(int score) {
	system("cls");
	printf("Game Over! Your score: %d\n", score);
	Sleep(1000); // Wait for 1 second to avoid exiting by mistake
	printf("Press any key to exit...\n");
	_getch(); // Wait for user input
	system("cls"); // Clear the console
}

int main(int argc, char** argv) {
	Snake snake(Direction::Right, starting_position, STARTING_LENGTH);
	Game game(snake);
	welcome_message();
	game.draw_map(); // Initial map display
	game.game_logic();
	score_screen(game.get_score()); // Display score screen
}