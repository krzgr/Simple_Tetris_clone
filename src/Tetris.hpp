#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <array>
#include <chrono>

class Tetris
{
public:
	static const int rows = 20;
	static const int cols = 10;
	static const int brickSize = 35;
	static const int padding = 3;
	static const int sideBarHeight = 50;

	static const unsigned int framerateLimit = 60;
	static const int zeroLevelFramesPerGridcell = 48;
	
	static const std::array<std::array<int, 4>, 7> tetrominos;

	static const sf::Color backgroundColor;
	static const sf::Color colors[];
	static const int sizeColors;
private:
	sf::RenderWindow window;
	sf::RectangleShape brick;
	sf::Event event;
	sf::Clock clock;
	std::vector<std::vector<uint8_t>> grid;

	int score;
	int level;
	int linesBeforeLevelIncreases;
	int delay;

	bool softDrop;

	std::array<std::pair<int, int>, 4> tetromino;
	uint8_t tetrominoColorID;
	uint8_t tetrominoID;

	std::mt19937 rng;
	std::uniform_int_distribution<int> colorDistribution;
	std::uniform_int_distribution<int> tetrominoDistribution;
private:
	void moveLeft();
	void moveRight();

	void tryRotation();
	void rotateRight();
	void rotateLeft();

	void genNewTetromino();
	void drop();

	bool isTetrominoColliding();

	bool isRowFilled(int row);
	void clearRow(int row);

	void reset();
	void nextLevel();
	void calcHowManyLinesInThisLevelLeft();
	void calcDelay();

	void draw();

public:
	void run();

	Tetris();
	~Tetris();
};