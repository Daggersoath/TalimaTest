#pragma once

#include <cstdint>
#include <cstring>
#include <vector>
#include <utility>
#include <random>

#include <iostream>

static constexpr char MAZE_WALL = '\xB2';
static constexpr char MAZE_PATH = '\xB0';
//static constexpr char MAZE_WALL = 'W';
//static constexpr char MAZE_PATH = ' ';
static constexpr char MAZE_START = 'S';
static constexpr char MAZE_DESTINATION = 'X';
static constexpr char MAZE_PLAYER = '*';
static constexpr char MAZE_FOUNDPATH = 'P';


template<size_t WIDTH = 32, size_t HEIGHT = 32>
class Maze
{
public:
    Maze();
    ~Maze();

    void Print();

    bool TestPath(const std::vector<std::pair<size_t, size_t>>& path);

    typedef char(*MazePtr)[HEIGHT];
    MazePtr GetMaze() { return mMaze; }

    const std::pair<size_t, size_t>& GetPlayerPos() { return mPlayerPos; }
    std::pair<size_t, size_t> GetDestination() { return std::make_pair(WIDTH - 1, HEIGHT / 2); }

private:
    void DepthFirstGenerate(const std::pair<size_t, size_t>& pos);

    bool CanMoveUp(const std::pair<size_t, size_t>& pos);
    bool CanMoveDown(const std::pair<size_t, size_t>& pos);
    bool CanMoveLeft(const std::pair<size_t, size_t>& pos);
    bool CanMoveRight(const std::pair<size_t, size_t>& pos);

    char mMaze[WIDTH][HEIGHT];
    std::pair<size_t, size_t> mPlayerPos;
};

template<std::uint64_t WIDTH, std::uint64_t HEIGHT>
inline Maze<WIDTH, HEIGHT>::Maze()
{
    std::memset(mMaze, MAZE_WALL, sizeof(mMaze[0][0]) * WIDTH * HEIGHT);
    mMaze[WIDTH - 1][HEIGHT / 2] = MAZE_PATH;
    DepthFirstGenerate(std::make_pair(WIDTH - 1, HEIGHT / 2));
    mMaze[WIDTH - 1][HEIGHT / 2] = MAZE_DESTINATION;
    for (size_t x = 0; x < WIDTH; ++x) {
        bool bFound = false;
        for (size_t y = 0; y < HEIGHT; ++y) {
            if (mMaze[x][y] == MAZE_PATH) {
                bFound = true;
                mPlayerPos.first = x;
                mPlayerPos.second = y;
                mMaze[x][y] = MAZE_PLAYER;
                break;
            }
        }
        if (bFound) {
            break;
        }
    }
}

template<std::uint64_t WIDTH, std::uint64_t HEIGHT>
inline Maze<WIDTH, HEIGHT>::~Maze()
{
}

template<std::uint64_t WIDTH, std::uint64_t HEIGHT>
inline void Maze<WIDTH, HEIGHT>::Print()
{
    for (size_t y = 0; y < HEIGHT; ++y) {
        for (size_t x = 0; x < WIDTH; ++x) {
            std::cout << mMaze[x][y];
        }
        std::cout << std::endl;
    }
}

template<size_t WIDTH, size_t HEIGHT>
inline bool Maze<WIDTH, HEIGHT>::TestPath(const std::vector<std::pair<size_t, size_t>>& path)
{
    if (path.empty()) {
        std::cout << "No nodes in path!" << std::endl;
        return false;
    }

    if (path.front() != mPlayerPos) {
        std::cout << "Path doesn't start at player position!" << std::endl;
        return false;
    }

    if (path.back() != GetDestination()) {
        std::cout << "Path doesn't end at destination!" << std::endl;
        return false;
    }

    for (size_t i = 1; i < path.size(); ++i) {
        long long xDist = std::abs(static_cast<long long>(path[i].first - path[i - 1].first));
        long long yDist = std::abs(static_cast<long long>(path[i].second - path[i - 1].second));
        if (xDist > 1 || yDist > 1 || xDist + yDist > 1) {
            std::cout << "Invalid movement from " << path[i - 1].first << "x" << path[i - 1].second
                << " to " << path[i].first << "x" << path[i].second << std::endl;
            return false;
        }
        if (mMaze[path[i].first][path[i].second] == MAZE_WALL) {
            std::cout << "Trying to move into a wall, you can't do that silly <3 " << path[i].first << "x" << path[i].second << " moving(" << xDist << "," << yDist << ")" << std::endl;
            return false;
        }
        mMaze[path[i].first][path[i].second] = MAZE_FOUNDPATH;
    }

    return true;
}

/*
template<std::uint64_t WIDTH, std::uint64_t HEIGHT>
inline void Maze<WIDTH, HEIGHT>::DepthFirstGenerate(const std::pair<size_t, size_t>& pos)
{
    unsigned char movedDirs = 0;
    unsigned char moveDir[4] = {
        (unsigned char)(std::rand() % 4),
        (unsigned char)(std::rand() % 4),
        (unsigned char)(std::rand() % 4),
        (unsigned char)(std::rand() % 4)
    };

    movedDirs = 1 << moveDir[0];
    for (unsigned char index = 1; index < 4; ++index) {
        while (movedDirs & (1 << moveDir[index])) moveDir[index] = ++moveDir[index] % 4;
    }

    for (unsigned char index = 0; index < 4; ++index) {
        switch (moveDir[index])
        {
        case 0:
            if (CanMoveUp(pos)) {
                mMaze[pos.first][pos.second + 1] = MAZE_PATH;
                mMaze[pos.first][pos.second + 2] = MAZE_PATH;
                DepthFirstGenerate(std::make_pair(pos.first, pos.second + 2));
            }
            break;
        case 1:
            if (CanMoveDown(pos)) {
                mMaze[pos.first][pos.second - 1] = MAZE_PATH;
                mMaze[pos.first][pos.second - 2] = MAZE_PATH;
                DepthFirstGenerate(std::make_pair(pos.first, pos.second - 2));
            }
            break;
        case 2:
            if (CanMoveLeft(pos)) {
                mMaze[pos.first - 1][pos.second] = MAZE_PATH;
                mMaze[pos.first - 2][pos.second] = MAZE_PATH;
                DepthFirstGenerate(std::make_pair(pos.first - 2, pos.second));
            }
            break;
        case 3:
            if (CanMoveRight(pos)) {
                mMaze[pos.first + 1][pos.second] = MAZE_PATH;
                mMaze[pos.first + 2][pos.second] = MAZE_PATH;
                DepthFirstGenerate(std::make_pair(pos.first + 2, pos.second));
            }
            break;
        default:
            break;
        }
    }
}*/

template<std::uint64_t WIDTH, std::uint64_t HEIGHT>
inline void Maze<WIDTH, HEIGHT>::DepthFirstGenerate(const std::pair<size_t, size_t>& pos)
{
    std::vector<std::pair<size_t, size_t>> generationPath;
    generationPath.push_back(pos);

    while (!generationPath.empty()) {
        std::pair<size_t, size_t> curPos = generationPath.back();

        unsigned char moveDir = (unsigned char)(std::rand() % 4);
        bool found = false;
        for (unsigned char i = 0; i < 4; ++i) {
            unsigned char index = (moveDir + i) & 3;
            if (index == 0 && CanMoveUp(curPos)
                || index == 1 && CanMoveDown(curPos)
                || index == 2 && CanMoveLeft(curPos)
                || index == 3 && CanMoveRight(curPos)) {
                moveDir = index;
                found = true;
                break;
            }
        }

        if (!found) {
            generationPath.pop_back();
            continue;
        }

        switch (moveDir)
        {
        case 0:
                mMaze[curPos.first][curPos.second + 1] = MAZE_PATH;
                mMaze[curPos.first][curPos.second + 2] = MAZE_PATH;
                generationPath.push_back(std::make_pair(curPos.first, curPos.second + 2));
            break;
        case 1:
                mMaze[curPos.first][curPos.second - 1] = MAZE_PATH;
                mMaze[curPos.first][curPos.second - 2] = MAZE_PATH;
                generationPath.push_back(std::make_pair(curPos.first, curPos.second - 2));
            break;
        case 2:
                mMaze[curPos.first - 1][curPos.second] = MAZE_PATH;
                mMaze[curPos.first - 2][curPos.second] = MAZE_PATH;
                generationPath.push_back(std::make_pair(curPos.first - 2, curPos.second));
            break;
        case 3:
                mMaze[curPos.first + 1][curPos.second] = MAZE_PATH;
                mMaze[curPos.first + 2][curPos.second] = MAZE_PATH;
                generationPath.push_back(std::make_pair(curPos.first + 2, curPos.second));
            break;
        default:
            break;
        }
    }
}

template<std::uint64_t WIDTH, std::uint64_t HEIGHT>
inline bool Maze<WIDTH, HEIGHT>::CanMoveUp(const std::pair<size_t, size_t>& pos)
{
    return HEIGHT - pos.second > 2 && mMaze[pos.first][pos.second + 2] == MAZE_WALL;
}

template<std::uint64_t WIDTH, std::uint64_t HEIGHT>
inline bool Maze<WIDTH, HEIGHT>::CanMoveDown(const std::pair<size_t, size_t>& pos)
{
    return pos.second > 2 && mMaze[pos.first][pos.second - 2] == MAZE_WALL;
}

template<std::uint64_t WIDTH, std::uint64_t HEIGHT>
inline bool Maze<WIDTH, HEIGHT>::CanMoveLeft(const std::pair<size_t, size_t>& pos)
{
    return pos.first >= 2 && mMaze[pos.first - 2][pos.second] == MAZE_WALL;
}

template<std::uint64_t WIDTH, std::uint64_t HEIGHT>
inline bool Maze<WIDTH, HEIGHT>::CanMoveRight(const std::pair<size_t, size_t>& pos)
{
    return WIDTH - pos.first >= 2 && mMaze[pos.first + 2][pos.second] == MAZE_WALL;
}

