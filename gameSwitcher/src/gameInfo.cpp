
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include "gameInfo.h"

using namespace std;

GameInfoData loadGameInfo(string filePath)
{
    GameInfoData game;

    std::ifstream file(filePath);
    if (file.is_open())
    {
        game.active = true;
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
                game.drive = line;
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
        file << game.drive << std::endl;
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

bool strEndsWith(string str, string suffix)
{
    if (str.length() < suffix.length())
    {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

vector<GameInfoData> loadGameListAtPath(string folderPath)
{
    vector<filesystem::directory_entry> entries;
    vector<GameInfoData> games;

    // Check if directory exists
    if (filesystem::exists(folderPath))
    {
        for (const auto &entry : filesystem::directory_iterator(folderPath))
        {
            if (entry.path().extension() == ".cfg")
            {
                entries.push_back(entry);
            }
        }

        std::sort(entries.begin(), entries.end(), [](const filesystem::directory_entry &a, const filesystem::directory_entry &b)
                  { return filesystem::last_write_time(a) > filesystem::last_write_time(b); });

        for (const auto &entry : entries)
        {
            if (entry.path().extension() == ".cfg")
            {
                GameInfoData gameInfo = loadGameInfo(entry.path());
                if (strEndsWith(gameInfo.core, "libretro.so"))
                {
                    games.push_back(gameInfo);
                }
            }
        }
    }
    return games;
}