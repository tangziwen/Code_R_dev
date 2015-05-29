#include "GameMap.h"

#include "GameInfo.h"

#include "TileMgr.h"
#include <queue>
extern "C"
{
#include "astar/astar.h"
};

#define IS_VOID(X) (x==0)
GameMap::GameMap(int width, int height)
:m_width(width), m_height(height)
{
    //map size at least occupy least one screen
    if (m_width < SCREEN_TILE_WIDTH)
    {
        m_width = SCREEN_TILE_WIDTH;
    }
    if (m_height < SCREEN_TILE_HEIGHT)
    {
        m_height = SCREEN_TILE_HEIGHT;
    }
    m_mapData = new unsigned int[m_width * m_height];
}

void GameMap::setCellData(int x, int y, unsigned int data)
{
    if (x<0 || x >getWidth())
        return;
    if (y<0 || y >getHeight())
        return;
    m_mapData[y*getWidth() + x] = data;
}

int GameMap::getHeight()
{
    return m_height;
}

int GameMap::getWidth()
{
    return m_width;
}

unsigned int GameMap::getCellData(int x, int y)
{
    return m_mapData[y*getWidth() + x];
}


void GameMap::generateMap(GeneratePolicy policy)
{
    switch (policy)
    {
    case GameMap::GeneratePolicy::Dungeon:
        genDungeon();
        break;
    case GameMap::GeneratePolicy::City:
        genCity();
        break;
    case GameMap::GeneratePolicy::OpenField:
        genOpenFiled();
        break;
    case GeneratePolicy::Random:
        genRandom();
        break;
    default:
        break;
    }
}

void GameMap::genRandom()
{
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            if (rand() % 10>5)
            {
                setCellData(j, i, TileMgr::get()->findTile("grass")->getID());
            }
            else
            {
                setCellData(j, i, TileMgr::get()->findTile("room")->getID());
            }
        }
    }
}

static int clamp(int min, int max, int value)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void GameMap::genDungeon()
{

    //fill all obstacle.
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            setCellData(j, i, TileMgr::get()->findTile("wall")->getID());
        }
    }
    m_dungeonGenerator = new DungeonGenerator(m_width, m_height);
    m_dungeonGenerator->generate();
    auto a = m_dungeonGenerator->mapData();
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            if (a[i*m_width + j] == 0)
            {
                setCellData(j, i, TileMgr::get()->findTile("room")->getID());
            }
        }
    }
    m_dungeonGenerator->getEntrance(m_entranceX, m_entranceY);
}

void GameMap::genCity()
{

}

void GameMap::genOpenFiled()
{

}


void GameMap::getEntrance(int &x, int &y)
{
    x = m_entranceX;
    y = m_entranceY;
}

bool GameMap::isObstacle(int x, int y)
{
    if (x >= 0 && x < m_width && y >=0 &&y < m_height)
    {
        auto id = getCellData(x, y);
        if (TileMgr::get()->isObstacle(id))
        {
            return true;
        }
        for (int i = 0; i < m_obstacleLists.size(); i++)
        {
            DynamicObstacle * o = m_obstacleLists[i];
            if (o->m_x == x && o->m_y == y)
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        return true;
    }
}

void GameMap::getSightLevel(int x, int y, int target_x, int target_y, int sight_distance, int &sightLevel)
{
    Vec2 playerPosition(x, y);
    Vec2 targetPosition(target_x, target_y);
    Vec2 direction = targetPosition - playerPosition;
    int length = direction.length();
    if (length > sight_distance)
    {
        sightLevel = 4;
        return;
    }
    direction.normalize();

    Vec2 tmp = playerPosition;
    while (1)
    {
        int tmp_x = int(tmp.x + 0.5);
        int tmp_y = int(tmp.y + 0.5);
        if ((tmp_x != target_x || tmp_y != target_y) && isObstacle(tmp_x, tmp_y))
        {
            sightLevel = 4;
            return;
        }
        if (tmp_x == target_x && tmp_y == target_y)
        {
            float ratio = length*1.0 / sight_distance;
            if (ratio < 0.2)
            {
                sightLevel = 0;
            }
            else if (ratio >= 0.2 && ratio < 0.5)
            {
                sightLevel = 1;
            }
            else if (ratio >= 0.5 && ratio < 0.9)
            {
                sightLevel = 2;
            }
            else
            {
                sightLevel = 3;
            }
             
            return;
        }
        tmp += direction;
    }
}

void GameMap::getRandomPosition(int &x, int &y)
{
    m_dungeonGenerator->getRandomPassablePos(x, y);
}


struct MapNode {
    int index;
    int x, y;
};

struct AstarMap {
    std::vector<MapNode> nodes;
    int height;
    int width;
    GameMap * m_map;
};

int gscore(void *ud, int indexfrom, int indexEnd) {
    return 1;
}

int hscore(void *ud, int src, int dst) {
    AstarMap *map = (AstarMap *)ud;
    int x1 = map->nodes[src].x;
    int y1 = map->nodes[src].y;
    int x2 = map->nodes[dst].x;
    int y2 = map->nodes[dst].y;
    return abs(x2 - x1) + abs  (y2 - y1);
}

AstarArray* adjacency(void *ud, int pos) {
    AstarMap *map = (AstarMap *)ud;
    AstarArray *arr = create_array(4);
    int x = pos %map->width;
    int y = pos / map->width;
    if (!map->m_map->isObstacle(x,y-1))
    {
        array_pushback(arr, (y - 1)*map->width + x);
    }
    if (!map->m_map->isObstacle(x, y + 1))
    {
        array_pushback(arr, (y + 1)*map->width + x);
    }
    if (!map->m_map->isObstacle(x-1, y ))
    {
        array_pushback(arr, y*map->width + x - 1);
    }
    if (!map->m_map->isObstacle(x + 1, y))
    {
        array_pushback(arr, y*map->width + x + 1);
    }
    return arr;
}
void GameMap::getPath(int x, int y, int targetX, int targetY, std::vector<GamePoint>&pathList)
{
    pathList.clear();
    AstarMap *map = new AstarMap();
    map->width = m_width;
    map->height = m_height;
    map->m_map = this;
    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            int index = j*m_width + i;
            MapNode a;
            a.x = j;
            a.y = i;
            a.index = index;
            map->nodes.push_back(a);
        }
    }
    astar_init(gscore, hscore, adjacency);
    AstarArray *path = astar_search(map, y*m_width + x, targetY *m_width + targetX);
    if (path)
    {
        for (int i = path->len - 1; i >= 0; i--)
        {
            int index = path->arr[i];
            int x = index % m_width;
            int y = index / m_width;
            pathList.push_back(GamePoint(x, y));
        }
    }
}

void GameMap::addDynamicObstacle(DynamicObstacle * obstacle)
{
    m_obstacleLists.push_back(obstacle);
}

void GameMap::removeDynamicObstacle(DynamicObstacle * obstacle)
{
    for (auto i = m_obstacleLists.begin(); i != m_obstacleLists.end(); i++)
    {
        DynamicObstacle * o = (*i);
        if (o == obstacle)
        {
            m_obstacleLists.erase(i);
            return ;
        }
    }
}

bool GameMap::isStaticObstacle(int x, int y)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        auto id = getCellData(x, y);
        return TileMgr::get()->isObstacle(id);
    }
    else
    {
        return true;
    }
}
