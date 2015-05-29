#ifndef TILE_MGR_H
#define TILE_MGR_H
#include <vector>
#include "GameTile.h"
class TileMgr
{
public:
    static TileMgr * get();
    void loadTileConfig(const char *src);
    GameTile * findTile(unsigned int tileId);
    GameTile * findTile(const char * name);
    bool isObstacle(unsigned int id);
private:
    static TileMgr * instance;
    TileMgr();
    std::vector<GameTile *> m_tilesList;
};
#endif // !TILE_MGR_H
