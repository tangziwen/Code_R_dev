#ifndef TILE_H
#define  TILE_H
#include <string>

class GameTile
{
public:
    GameTile(unsigned int ID, std::string name, std::string resPath, std::string description, bool is_obstacle);
    unsigned int getID();
    std::string &getName();
    std::string &getResPath();
    std::string &getDescription();
    bool isObstacle();
private:
    unsigned int m_ID;
    std::string m_name;
    std::string m_resPath;
    std::string m_description;
    bool m_isObstacle;
};
#endif // !TILE_H
