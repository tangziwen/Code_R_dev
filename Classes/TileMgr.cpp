#include "TileMgr.h"
#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

USING_NS_CC;
TileMgr * TileMgr::instance = NULL;
using namespace std;

TileMgr * TileMgr::get()
{
    if (!instance)
    {
        instance = new TileMgr();
        
    }
    return instance;
}

TileMgr::TileMgr()
{

}

void TileMgr::loadTileConfig(const char *src)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(src);
    auto root = doc.FirstChildElement("Tiles");
    auto tileElement = root->FirstChildElement();
    while (tileElement)
    {
        auto tmp = tileElement->FirstChildElement("is_obstacle")->GetText();
        bool isObstacle = false;
        if (tmp[0] == 't' || tmp[0] == 'T')
        {
            isObstacle = true;
        }
        std::string resPath = "tiles/res/";
        resPath.append(tileElement->FirstChildElement("res_path")->GetText());
        auto tile = new GameTile(m_tilesList.size(), tileElement->FirstChildElement("name")->GetText(), resPath, tileElement->FirstChildElement("description")->GetText(), isObstacle);
        m_tilesList.push_back(tile);
        tileElement = tileElement->NextSiblingElement();
    }
}

GameTile * TileMgr::findTile(unsigned int tileId)
{
    return m_tilesList[tileId];
}

GameTile * TileMgr::findTile(const char * name)
{
    for (int i = 0; i < m_tilesList.size(); i++)
    {
        if (m_tilesList[i]->getName().compare(name) == 0)
        {
            return m_tilesList[i];
        }
    }
}

bool TileMgr::isObstacle(unsigned int id)
{
    return findTile(id)->isObstacle();
}
