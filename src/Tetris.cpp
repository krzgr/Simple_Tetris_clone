#include "Tetris.hpp"

const sf::Color Tetris::colors[] =
{
    sf::Color(28, 15, 6),
    sf::Color(234, 213, 0),
    sf::Color(219, 132, 1),
    sf::Color(90, 176, 189),
    sf::Color(137, 64, 135),
    sf::Color(204, 11, 16),
    sf::Color(56, 153, 46),
    sf::Color(21, 100, 166)
};

const std::array<std::array<int, 4>, 7> Tetris::tetrominos
{
    std::array<int, 4>{3,1,5,7},
    std::array<int, 4>{3,1,2,5},
    std::array<int, 4>{3,0,1,2},
    std::array<int, 4>{3,0,1,5},
    std::array<int, 4>{3,1,4,5},
    std::array<int, 4>{3,0,2,5},
    std::array<int, 4>{3,1,2,4}
};

const int Tetris::sizeColors = sizeof(Tetris::colors) / sizeof(Tetris::colors[0]);

void Tetris::draw()
{
    window.clear(colors[0]);
    brick.setPosition(sf::Vector2f(0.0f, 0.0f));

    for (auto& row : grid)
    {
        for (auto& val : row)
        {
            brick.setFillColor(colors[val]);
            window.draw(brick);
            brick.move(sf::Vector2f(brickSize, 0.0f));
        }
        brick.setPosition(sf::Vector2f(0.0f, brick.getPosition().y + (float)brickSize));
    }

    brick.setFillColor(colors[tetrominoColorID]);

    for (auto& piece : tetromino)
    {
        brick.setPosition(sf::Vector2f(piece.first * brickSize, piece.second * brickSize));
        window.draw(brick);
    }
        

    window.display();
}

void Tetris::run()
{
    window.create(sf::VideoMode(brickSize * cols, brickSize * rows), "Tetris v1.0", sf::Style::Titlebar | sf::Style::Close);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        draw();
    }
}

void Tetris::reset()
{
    for (auto& row : grid)
        for (auto& val : row)
            val = 0;

    genNewTetromino();
}

Tetris::Tetris()
    :event(), brick(sf::Vector2f(brickSize, brickSize)), grid(rows), 
        tetrominoDistribution(1, tetrominos.size() - 1), colorDistribution(1, sizeColors - 1), rng()

{
    for (int i = 0; i < rows; i++)
    {
        grid.emplace_back(std::vector<uint8_t>(cols));

        for (int j = 0; j < cols; j++)
            grid[i].emplace_back(0);
    }

    genNewTetromino();
}

Tetris::~Tetris()
{

}

void Tetris::undoRotation()
{
    int x = tetromino[0].first;
    int y = tetromino[0].second;

    for (int i = 1; i < tetromino.size(); i++)
    {
        const int x1 = tetromino[i].second - y + x;
        const int y1 = -tetromino[i].first + x + y;
        tetromino[i].first = x1;
        tetromino[i].second = y1;
    }
}

void Tetris::moveLeft()
{
    for (auto& piece : tetromino)
        piece.first--;

    if(isTetrominoColliding())
        for (auto& piece : tetromino)
            piece.first++;
}

void Tetris::moveRight()
{
    for (auto& piece : tetromino)
        piece.first++;

    if (isTetrominoColliding())
        for (auto& piece : tetromino)
            piece.first--;
}

void Tetris::rotate()
{
    int x = tetromino[0].first;
    int y = tetromino[0].second;

    for (int i = 1; i < tetromino.size(); i++)
    {
        const int x1 = -tetromino[i].second + y + x;
        const int y1 = tetromino[i].first - x + y;
        tetromino[i].first = x1;
        tetromino[i].second = y1;
    }

    if (isTetrominoColliding())
        undoRotation();
}

void Tetris::genNewTetromino()
{
    int tetrominoID = tetrominoDistribution(rng);
    tetrominoColorID = (uint8_t)colorDistribution(rng);

    for (int i = 0; i < 4; i++)
    {
        tetromino[i].first = tetrominos[tetrominoID][i] % 2 + (cols / 2) - 1;
        tetromino[i].second = tetrominos[tetrominoID][i] / 2;
    }

    if (isTetrominoColliding())
        reset();
}

void Tetris::drop()
{
    for (auto& piece : tetromino)
        piece.second++;

    if (isTetrominoColliding())
    {
        for (auto& piece : tetromino)
            grid[piece.second - 1][piece.first] = tetrominoColorID;

        genNewTetromino();
    }
}

bool Tetris::isTetrominoColliding()
{

    for (auto& piece : tetromino)
    {
        if (piece.first < 0 || piece.first >= cols || piece.second < 0 || piece.second >= rows)
            return true;
        if (grid[piece.second][piece.first] != 0)
            return true;
    }
    
    return false;
}

