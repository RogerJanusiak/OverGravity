#include "../includes/Level.h"
#include "../includes/GlobalConstants.h"

#include <fstream>
#include <SDL_log.h>
#include <SDL_rect.h>
#include <sstream>

class Entity;

Level::Level(GlobalGameState& ggs, const int level) : ggs(ggs) {
    selectWeights(level);
    loadFromCSV(level);

    for(auto & enemySpawn : enemySpawns) {
        allSpawns.push_back(&enemySpawn);
    }
    for(auto & playerSpawn : playerSpawns) {
        allSpawns.push_back(&playerSpawn);
    }

}

void Level::loadFromCSV(int level) {

    char* appDir = SDL_GetBasePath();
    std::string currentPath(appDir);
    SDL_free(appDir);

    std::string filePath;
    if(level == 1) {
        filePath = currentPath + "resources/levels/airduct.csv";
    } else if(level == 2) {
        filePath = currentPath + "resources/levels/airport.csv";
    } else if(level == 3) {
        filePath = currentPath + "resources/levels/lab.csv";
    } else if (level == 4) {
        filePath = currentPath + "resources/levels/lobby.csv";
    }

    std::ifstream file((filePath).c_str());
    if (!file.is_open()) {
        SDL_Log("Could not load level file!");
    }
    constexpr int MAX_ROWS = 100;
    constexpr int MAX_COLS = 16;
    std::string data[MAX_ROWS][MAX_COLS];
    std::string line;
    int row = 0;
    // Store the CSV data from the CSV file to the 2D array
    while (getline(file, line) && row < MAX_ROWS) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        while (getline(ss, cell, ',') && col < MAX_COLS) {
            data[row][col] = cell;
            col++;
        }
        row++;
    }
    file.close();

    if(!levelMap.empty()) {
        levelMap.clear();
    }

    for (int i = 0; i < row; i++) {
        std::vector<int> mapRow;
        for(int j = 0; j < MAX_COLS; j++) {
            int multiplier = j;
            if(std::stoi(data[i][j]) == 0) {
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),ggs.renderer);
                mapRow.push_back(0);
            } else if(std::stoi(data[i][j]) == 3) {
                playerSpawns.emplace_back(scale(multiplier*TILE_SIZE-25),scale(i*TILE_SIZE+(TILE_SIZE-17-60)),scale(50),scale(60),1);
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),ggs.renderer);
                mapRow.push_back(1);
            } else if(std::stoi(data[i][j]) == 2) {
                enemySpawns.emplace_back(scale(multiplier*TILE_SIZE+(TILE_SIZE-50)/2),scale(i*TILE_SIZE+(TILE_SIZE-17-50)),scale(50),scale(50),2);
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),ggs.renderer);
                mapRow.push_back(2);
            }else if(std::stoi(data[i][j]) == 4) {
                SDL_Rect tempRect = {scale(multiplier*TILE_SIZE+(TILE_SIZE-50)/2),scale(i*TILE_SIZE+(TILE_SIZE-17-50)),scale(50),scale(50)};
                teleports.emplace_back(tempRect);
                platforms.emplace_back(multiplier*TILE_SIZE,i*TILE_SIZE+(TILE_SIZE-17),ggs.renderer);
                mapRow.push_back(3);
            } else {
                mapRow.push_back(-1);
            }
        }
        levelMap.push_back(mapRow);
    }
}

void Level::selectWeights(const int level) {
    if(level == 1) {
        spawnWeights = &ductWeights;
    } else if(level == 2) {
        spawnWeights = &airportWeights;
    }
}

void Level::updateSpawns(const std::list<Entity*>& allCharacterEntities) const {
    for (const auto & allSpawn : allSpawns) {
        allSpawn->setOccupied(false);
        for (const auto & allCharacterEntitie : allCharacterEntities) {
            if(Entity::isColliding(allSpawn->getRect(),allCharacterEntitie->getRect())) {
                allSpawn->setOccupied(true);
            }
        }
    }
}

void Level::render() const {
    for (auto& platform : platforms) {
        platform.render();
    }

    for(const auto& spawn : allSpawns) {
        spawn->render(ggs.renderer);
    }

    for(auto& teleport : teleports) {
        SDL_SetRenderDrawColor(ggs.renderer, 106, 13, 173, 200);
        SDL_RenderFillRect(ggs.renderer, &teleport);
    }
}
