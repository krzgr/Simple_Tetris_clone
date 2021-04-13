#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <array>

class Tetris
{
public:
	static const int rows = 20;
	static const int cols = 16;
	static const int brickSize = 35;

	static const std::array<std::array<int, 4>, 7> tetrominos;

	static const sf::Color colors[];
	static const int sizeColors;
private:
	sf::RenderWindow window;
	sf::RectangleShape brick;
	sf::Event event;
	std::vector<std::vector<uint8_t>> grid;

	std::array<std::pair<int, int>, 4> tetromino;
	uint8_t tetrominoColorID;

	std::mt19937 rng;
	std::uniform_int_distribution<short int> colorDistribution;
	std::uniform_int_distribution<int> tetrominoDistribution;
private:
	void moveLeft();
	void moveRight();
	void rotate();
	void undoRotation();
	void genNewTetromino();
	void drop();

	bool isTetrominoColliding();

	bool checkLine();
	void clearLine();

	void reset();

	void draw();

public:
	void run();

	Tetris();
	~Tetris();
};