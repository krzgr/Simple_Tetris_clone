#include "Tetris.hpp"

const sf::Color backgrundColor = sf::Color(28, 47, 57);

const sf::Color Tetris::colors[] =
{
    sf::Color(38, 57, 61),
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
    window.clear(backgrundColor);
    brick.setPosition(sf::Vector2f((float)padding, (float)padding));

    for (auto& row : grid)
    {
        for (auto& val : row)
        {
            brick.setFillColor(colors[val]);
            window.draw(brick);
            brick.move(sf::Vector2f(float(brickSize + padding), 0.0f));
        }
        brick.setPosition(sf::Vector2f((float)padding, brick.getPosition().y + float(brickSize + padding)));
    }

    brick.setFillColor(colors[tetrominoColorID]);

    for (auto& piece : tetromino)
    {
        brick.setPosition(sf::Vector2f(float(piece.first * (brickSize + padding) + padding), float(piece.second * (brickSize + padding) + padding)));
        window.draw(brick);
    }
        

    window.display();
}

void Tetris::run()
{
    window.create(sf::VideoMode((brickSize + padding) * cols + padding, (brickSize + padding) * rows + padding), 
        "Tetris v1.0", sf::Style::Titlebar | sf::Style::Close);

    while (window.isOpen())
    {
        clock.restart();
        while (clock.getElapsedTime().asMilliseconds() < delay)
        {

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::KeyPressed)
                {
                    switch (event.key.code)
                    {
                    case sf::Keyboard::Up:    rotate(); break;
                    case sf::Keyboard::Down:  drop(); break;
                    case sf::Keyboard::Left:  moveLeft(); break;
                    case sf::Keyboard::Right: moveRight(); break;
                    }

                    draw();
                }
            }
        }

        drop();
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
        tetrominoDistribution(0, (int)tetrominos.size() - 1), colorDistribution(1, sizeColors - 1),
        rng((unsigned)std::chrono::system_clock::now().time_since_epoch().count())
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
        int min_y = rows;
        int max_y = 0;
        for (auto& piece : tetromino)
        {
            piece.second--;

            if (min_y > piece.second)
                min_y = piece.second;
            else if (max_y < piece.second)
                max_y = piece.second;

            grid[piece.second][piece.first] = tetrominoColorID;
        }

        for(int i = max_y; i >= min_y; i--)
            if (isRowFilled(i))
            {
                clearRow(i);
                i++;
            }

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

bool Tetris::isRowFilled(int row)
{
    for (auto& cell : grid[row])
        if (cell == 0)
            return false;
    return true;
}

void Tetris::clearRow(int row)
{
    for (int i = row; i > 0; i--)
        for (int j = 0; j < cols; j++)
            grid[i][j] = grid[i - 1][j];
}
