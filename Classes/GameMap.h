#ifndef GAME_MAP_H
#define  GAME_MAP_H
#include "cocos2d.h"
#include "dungeon/dungeongenerator.h"
#include "GamePoint.h"
#include "DynamicObstacle.h"
USING_NS_CC;
class GameMap{
public:
    enum class GeneratePolicy{
        Dungeon,
        City,
        OpenField,
        Random
    };
    enum class Orient
    {
        Left,
        Right,
        Up,
        Down,
        Invalid
    };
    GameMap(int width, int height);
    void generateMap(GeneratePolicy policy);
    void setCellData(int x, int y, unsigned int data);
    unsigned int getCellData(int x, int y);
    int getHeight();
    int getWidth();
    void getEntrance(int &x, int &y);
    bool isObstacle(int x, int y);
    bool isStaticObstacle(int x, int y);
    void getRandomPosition(int &x, int &y);
    void getSightLevel(int x, int y, int target_x, int target_y, int sight_distance, int &sightLevel);
    void getPath(int x, int y, int targetX, int targetY, std::vector<GamePoint>&pathList);
    void addDynamicObstacle(DynamicObstacle * obstacle);
    void removeDynamicObstacle(DynamicObstacle * obstacle);
private:
    void genRandom();
    void genDungeon();
    void genCity();
    void genOpenFiled();
private:
    std::vector<DynamicObstacle * >m_obstacleLists;
    DungeonGenerator * m_dungeonGenerator;
    int m_entranceX;
    int m_entranceY;
    int m_width;
    int m_height;
    unsigned int *m_mapData;
};

#endif