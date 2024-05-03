
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include "gameInfo.h"

using namespace std;

GameInfoData loadGameInfo(string filePath)
{
    GameInfoData game;

    std::ifstream file(filePath);
    if (file.is_open())
    {
        std::string line;
        int lineCount = 0;
        while (std::getline(file, line))
        {
            switch (lineCount)
            {
            case 0:
                game.name = line;
                break;
            case 1:
                game.core = line;
                break;
            case 2:
                game.coreName = line;
                break;
            case 3:
                game.path = line;
                break;
            case 4:
                game.folder = line;
                break;
            case 5:
                game.fileName = line;
                break;
            default:
                break;
            }
            lineCount++;
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }

    return game;
}

GameInfoData writeGameInfo(string filePath, GameInfoData game)
{
    ofstream file(filePath);
    if (file.is_open())
    {
        file << game.name << std::endl;
        file << game.core << std::endl;
        file << game.coreName << std::endl;
        file << game.path << std::endl;
        file << game.folder << std::endl;
        file << game.fileName << std::endl;

        // Ensure data is synced to disk immediately
        file.flush();
        file.close();
    }
    else
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }
    return game;
}