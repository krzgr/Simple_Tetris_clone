#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <vector>
#include <random>
#include <array>
#include <chrono>
#include <string>

class Tetris
{
public:
	static const int rows = 20;
	static const int cols = 10;
	static const int brickSize = 35;
	static const int padding = 3;
	static const int sideBarWidth = 300;
	static const int nextTetrominoRenderY = padding + 14 * (brickSize + padding);
	static const int nextTetrominoRenderX = padding + (cols + 3) * (brickSize + padding);

	static const int textCharSize = 32;
	static const sf::Color textColor;

	static const int textTopOffset = 100;
	static const int textLeftOffset = 70;
	static const int offsetBetweenUpperString = 50;

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
	sf::Font scoreFont;
	sf::Text scoreText;
	std::vector<std::vector<uint8_t>> grid;

	int score;
	int level;
	int linesBeforeLevelIncreases;
	int delay;

	bool softDrop;

	std::array<std::pair<int, int>, 4> tetromino;
	uint8_t tetrominoColorID;
	uint8_t tetrominoID;

	std::array<std::pair<int, int>, 4> nextTetromino;
	uint8_t nextTetrominoColorID;
	uint8_t nextTetrominoID;

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