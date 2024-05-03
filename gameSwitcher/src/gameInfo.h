#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

struct GameInfoData
{
    std::string name;
    std::string core;
    std::string coreName;
    std::string path;
    std::string folder;
    std::string fileName;
};

GameInfoData loadGameInfo(std::string filePath);
GameInfoData writeGameInfo(std::string filePath, GameInfoData game);

#endif