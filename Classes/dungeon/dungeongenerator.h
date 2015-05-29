#ifndef DUNGEONGENERATOR_H
#define DUNGEONGENERATOR_H

#include <vector>
class DungeonGenerator
{
public:
    DungeonGenerator(int x, int y);
    void generate();
    void display();
    unsigned int *mapData() const;
    void setMapData(unsigned int *mapData);

    int width() const;
    void setWidth(int width);

    int height() const;
    void setHeight(int height);

    int getIndex(int x, int y);

    int randomPick(int chance);

    void getEntrance(int &x, int &y);

    void getRandomPassablePos(int &x, int &y);
private:
    unsigned int * m_mapData;
    std::vector <int>m_emptyCellList;
    int m_width;
    int m_height;
    int m_entrance_x;
    int m_entrance_y;
};

#endif // DUNGEONGENERATOR_H
