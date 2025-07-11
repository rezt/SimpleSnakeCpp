#include <iostream>
#include <string>

// Temporary array, for tests:
int map[] =
{
	100, 100, 100, 100, 100, 100, 100, 100,
	100, 0, 0, 0, 0, 0, 0, 100,
	100, 0, 0, 0, 0, 0, 0, 100,
	100, 0, 0, 0, 0, 0, 0, 100,
	100, 0, 0, 0, 0, 0, 0, 100,
	100, 0, 0, 0, 0, 0, 0, 100,
	100, 0, 0, 0, 0, 0, 0, 100,
	100, 100, 100, 100, 100, 100, 100, 100,
};

enum class Direction {
	Left,
	Down,
	Up,
	Right
};

const int map_width = 8, map_height = 8;
constexpr int map_size = map_width * map_height;
constexpr int starting_position = 4 + (4 * map_width);
int snake_position = starting_position;
int snake_length = 3;

char buffer[map_size + map_height];

void move_snake(Direction heading, int &position) {
	switch (heading) {
		case Direction::Left:
			position--;
		case Direction::Down:
			position += map_width;
		case Direction::Up:
			position -= map_width;
		case Direction::Right:
			position++;
	}
}

void prepare_map_buffer(char *display) {
	for (int y = 0; y < map_height; y++) {
		for (int x = 0; x < map_width; x++) {
			int position = y * map_width + x;
			switch (map[position]) {
				case 0:
					display[position] = '.';
					break;
				case 100:
					display[position] = '#';
					break;
				default:
					map[position]--;
					display[position] = 'O';
			}
		}
	}
}

void generate_map(int *map) {
	// Here we will be generating a map.
	//map[0] = 0;


}

void draw_snake(int position) {
	map[position] = snake_length;
}

void draw_map(char* display) {
	for (int y = 0; y < map_height; y++) {
		for (int x = 0; x < map_width; x++) {
			printf("%c", display[y * map_width + x]);	
		}
		printf("\n");
	}
}

void player_input() {
	// Placeholder
}

int main(int argc, char** argv) {
	draw_snake(snake_position);
	generate_map(map);
	prepare_map_buffer(buffer);
	draw_map(buffer);
}