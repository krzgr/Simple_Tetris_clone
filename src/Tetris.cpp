#include "Tetris.hpp"

const sf::Color Tetris::backgroundColor = sf::Color(28, 47, 57);

const sf::Color Tetris::textColor = sf::Color(255, 255, 255);

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
    std::array<int, 4>{5,1,3,7}, // Long
    std::array<int, 4>{3,0,1,2}, // O
    std::array<int, 4>{3,1,2,5}, // T
    std::array<int, 4>{3,0,1,5}, // L
    std::array<int, 4>{3,1,4,5}, // J
    std::array<int, 4>{2,3,1,4}, // Z
    std::array<int, 4>{2,3,0,5}  // S
};

const int Tetris::sizeColors = sizeof(Tetris::colors) / sizeof(Tetris::colors[0]);

void Tetris::draw()
{
    window.clear(backgroundColor);
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
        brick.setPosition(sf::Vector2f(float(piece.first * (brickSize + padding) + padding),
                                       float(piece.second * (brickSize + padding) + padding)));
        window.draw(brick);
    }
    
    /*Rendering sidebar*/

    scoreText.setString("Score");
    window.draw(scoreText);
    scoreText.move(sf::Vector2f(0.0f, (float)offsetBetweenUpperString));
    
    scoreText.setString(std::to_string(score));
    window.draw(scoreText);
    scoreText.move(sf::Vector2f(0.0f, (float)offsetBetweenUpperString));

    scoreText.setString("Level");
    window.draw(scoreText);
    scoreText.move(sf::Vector2f(0.0f, (float)offsetBetweenUpperString));

    scoreText.setString(std::to_string(level));
    window.draw(scoreText);
    scoreText.move(sf::Vector2f(0.0f, (float)offsetBetweenUpperString));

    scoreText.setString("Lines left");
    window.draw(scoreText);
    scoreText.move(sf::Vector2f(0.0f, (float)offsetBetweenUpperString));

    scoreText.setString(std::to_string(linesBeforeLevelIncreases));
    window.draw(scoreText);
    scoreText.move(sf::Vector2f(0.0f, (float)offsetBetweenUpperString));

    scoreText.setString("Next");
    window.draw(scoreText);
    scoreText.move(sf::Vector2f(0.0f, -6.0f * (float)offsetBetweenUpperString));

    /*Next tetromino rendering*/

    int x = nextTetromino[0].first;
    int y = nextTetromino[0].second;


    brick.setFillColor(colors[nextTetrominoColorID]);

    for (auto& piece : nextTetromino)
    {
        brick.setPosition(sf::Vector2f(nextTetrominoRenderX + (piece.first - x) * (brickSize + padding),
                nextTetrominoRenderY + (piece.second - y) * (brickSize + padding)));
        window.draw(brick);
    }

    window.display();
}

void Tetris::run()
{
    window.create(sf::VideoMode((brickSize + padding) * cols + padding + sideBarWidth, (brickSize + padding) * rows + padding), 
        "Tetris v1.0", sf::Style::Titlebar | sf::Style::Close);

    window.setFramerateLimit(framerateLimit);

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
                    case sf::Keyboard::Up:    tryRotation(); break;
                    case sf::Keyboard::Down:
                        if(!softDrop)
                        {
                            softDrop = true; calcDelay();
                        } break;
                    case sf::Keyboard::Left:  moveLeft(); break;
                    case sf::Keyboard::Right: moveRight(); break;
                    }
                }
                else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down)
                    {
                        softDrop = false;
                        calcDelay();
                    }
                draw();
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

    score = 0;
    level = 0;

    calcHowManyLinesInThisLevelLeft();
    calcDelay();

    genNewTetromino();
    genNewTetromino();
}

void Tetris::nextLevel()
{
    level++;
    calcHowManyLinesInThisLevelLeft();
    calcDelay();
}

void Tetris::calcHowManyLinesInThisLevelLeft()
{
    if (level <= 9)
        linesBeforeLevelIncreases = level * 10 + 10;
    else if (level <= 15)
        linesBeforeLevelIncreases = 100;
    else if (level <= 25)
        linesBeforeLevelIncreases = level * 10 - 50;
    else
        linesBeforeLevelIncreases = 200;
}

void Tetris::calcDelay()
{
    unsigned int framesPerGridcell;

    if (level <= 8)
        framesPerGridcell = zeroLevelFramesPerGridcell - 5 * level;
    else if (level <= 18)
        framesPerGridcell = 9 - (level + 2) / 3;
    else if (level <= 28)
        framesPerGridcell = 2;
    else
        framesPerGridcell = 1;

    if(softDrop)
        framesPerGridcell = 2;

    delay = (framesPerGridcell * 1000) / framerateLimit;
}

Tetris::Tetris()
    :event(), brick(sf::Vector2f(brickSize, brickSize)), grid(rows), 
        tetrominoDistribution(0, (int)tetrominos.size() - 1), colorDistribution(1, sizeColors - 1),
        rng((unsigned)std::chrono::system_clock::now().time_since_epoch().count()),
        level(0), score(0), softDrop(false)
{
    grid = std::vector<std::vector<uint8_t>>(rows, std::vector<uint8_t>(cols, 0));

    calcHowManyLinesInThisLevelLeft();
    calcDelay();

    if (!scoreFont.loadFromFile("../res/Ubuntu-Regular.ttf"))
        exit(-1);

    scoreText.setFont(scoreFont);
    scoreText.setCharacterSize(textCharSize);
    scoreText.setFillColor(textColor);

    scoreText.setPosition(sf::Vector2f(padding + cols * (brickSize + padding) + textLeftOffset, (float)textTopOffset));

    /*Generating here empty nextTetromino piece*/
   
    for (auto& piece : nextTetromino)
    {
        piece.first = 0;
        piece.second = 0;
    }

    nextTetrominoID = 1;

    /*In function genNewTetromino(), tetromino will copy data from nextTetromino*/
    /*and nextTetromino will be generated*/
    /*We do it twice in ordet to have two tetrominos generated (next and current)*/

    genNewTetromino();
    genNewTetromino();
}

Tetris::~Tetris()
{

}

void Tetris::rotateLeft()
{
    if (tetrominoID == 1)
        return;
    else if ((tetrominoID == 0 && tetromino[0].second == tetromino[1].second)  // Long
          || (tetrominoID >= 5 && tetromino[0].second == tetromino[1].second)) // S and Z 
    {
        rotateRight();
        return;
    }

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

void Tetris::rotateRight()
{
    if (tetrominoID == 1)
        return;
    else if ((tetrominoID == 0 && tetromino[0].first == tetromino[1].first)  // Long
          || (tetrominoID >= 5 && tetromino[0].first == tetromino[1].first)) // S and Z
    {
        rotateLeft();
        return;
    }

    int x = tetromino[0].first;
    int y = tetromino[0].second;

    for (int i = 1; i < tetromino.size(); i++)
    {
        const int x1 = -tetromino[i].second + y + x;
        const int y1 = tetromino[i].first - x + y;
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

void Tetris::tryRotation()
{
    rotateRight();

    if (isTetrominoColliding())
        rotateLeft();
}

void Tetris::genNewTetromino()
{
    /*We move next tetromino into current used tetromino*/
    for (int i = 0; i < tetromino.size(); i++)
        tetromino[i] = nextTetromino[i];

    tetrominoColorID = nextTetrominoColorID;
    tetrominoID = nextTetrominoID;

    nextTetrominoID = (uint8_t)tetrominoDistribution(rng);
    nextTetrominoColorID = (uint8_t)colorDistribution(rng);

    for (int i = 0; i < tetromino.size(); i++)
    {
        nextTetromino[i].first = tetrominos[nextTetrominoID][i] % 2 + (cols / 2) - 1;
        nextTetromino[i].second = tetrominos[nextTetrominoID][i] / 2;
    }

    if (tetrominoID == 2 || tetrominoID == 3 || tetrominoID == 4) // T, L and J
        rotateLeft();
    else
        rotateRight();

    if (tetrominoID >= 2) // NOT Long, O
    {
        for (auto& piece : tetromino)
            piece.second--;
    }
    else if (tetrominoID == 0) // Long
    {
        for (auto& piece : tetromino)
            piece.second -= 2;
    }

    if (isTetrominoColliding())
        reset();
}

void Tetris::drop()
{
    for (auto& piece : tetromino)
        piece.second++;

    if (softDrop)
        score += 10;

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

        int destroyedLines = 0;

        for(int i = max_y; i >= min_y; i--)
            if (isRowFilled(i))
            {
                clearRow(i);
                i++;
                destroyedLines++;
                linesBeforeLevelIncreases--;
                if (linesBeforeLevelIncreases == 0)
                {
                    level++;
                    calcHowManyLinesInThisLevelLeft();
                    calcDelay();
                }
            }

        switch (destroyedLines)
        {
            case 1: score += (level + 1) * 40; break;
            case 2: score += (level + 1) * 100; break;
            case 3: score += (level + 1) * 300; break;
            case 4: score += (level + 1) * 1200; break;
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
