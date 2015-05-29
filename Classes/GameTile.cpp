#include "GameTile.h"

unsigned int GameTile::getID()
{
    return m_ID;
}

std::string & GameTile::getName()
{
    return m_name;
}

GameTile::GameTile(unsigned int ID, std::string name, std::string resPath, std::string description, bool is_obstacle)
:m_ID(ID), m_name(name), m_resPath(resPath), m_description(description), m_isObstacle(is_obstacle)
{
    
}

std::string & GameTile::getDescription()
{
    return m_description;
}

std::string & GameTile::getResPath()
{
    return m_resPath;
}

bool GameTile::isObstacle()
{
    return m_isObstacle;
}
