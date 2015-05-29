#include "dungeongenerator.h"
#define EMPTY 0
#define WALL 1
#include <iostream>
#include <stdlib.h>
using namespace std;

DungeonGenerator::DungeonGenerator(int x, int y)
{
    m_mapData = new unsigned int[x * y];
    m_width = x;
    m_height = y;
    for(int i  = 0; i<m_height ;i++)
    {
        for(int j = 0; j< m_width; j++)
        {
            m_mapData[getIndex (j, i)] = WALL;
        }
    }
}

void DungeonGenerator::generate()
{
    unsigned int * tmpData = new unsigned int [m_width * m_height];
    int r1_cutoff = 5, r2_cutoff = 2;
    for(int i  = 0; i<m_height ;i++)
    {
        for(int j = 0; j< m_width; j++)
        {
            tmpData[getIndex (j, i)] = WALL;
        }
    }

    for(int i  = 0; i<m_height ;i++)
    {
        for(int j = 0; j< m_width; j++)
        {
            m_mapData[getIndex (j, i)] = randomPick(40);
        }
    }

    for(int  i = 0; i<m_height; i++)
    {
        m_mapData[getIndex (0,i)] = m_mapData[getIndex (m_width - 1,i)] = WALL;
    }

    for(int  i = 0; i<m_width; i++)
    {
        m_mapData[getIndex (i,0)] = m_mapData[getIndex (i,m_height - 1)] = WALL;
    }
    for (int the_gen_iter = 0; the_gen_iter < 4; the_gen_iter++)
    {
        int xi, yi, ii, jj;

        for (yi = 1; yi < m_height - 1; yi++)
        for (xi = 1; xi < m_width - 1; xi++)
        {
            int adjcount_r1 = 0,
                adjcount_r2 = 0;

            for (ii = -1; ii <= 1; ii++)
            for (jj = -1; jj <= 1; jj++)
            {
                if (m_mapData[getIndex(xi + jj, yi + ii)] != EMPTY)
                    adjcount_r1++;
            }
            for (ii = yi - 2; ii <= yi + 2; ii++)
            for (jj = xi - 2; jj <= xi + 2; jj++)
            {
                if (abs(ii - yi) == 2 && abs(jj - xi) == 2)
                    continue;
                if (ii < 0 || jj < 0 || ii >= m_height || jj >= m_width)
                    continue;
                if (m_mapData[getIndex(jj, ii)] != EMPTY)
                    adjcount_r2++;
            }
            if (adjcount_r1 >= r1_cutoff || adjcount_r2 <= r2_cutoff)
                tmpData[getIndex(xi, yi)] = WALL;
            else
                tmpData[getIndex(xi, yi)] = EMPTY;
        }
        for (int yi = 1; yi < m_height - 1; yi++)
        for (int xi = 1; xi < m_width - 1; xi++)
        {
            m_mapData[getIndex(xi, yi)] = tmpData[getIndex(xi, yi)];
        }
    }

    for (int yi = 1; yi < m_height - 1; yi++)
    for (int xi = 1; xi < m_width - 1; xi++)
    {
        if (m_mapData[getIndex(xi, yi)] == EMPTY)
        {
            m_emptyCellList.push_back(yi * m_width + xi);
        }
    }

    //set the entrance.
    getRandomPassablePos(m_entrance_x, m_entrance_y);
}

void DungeonGenerator::display()
{
    for(int i =0;i<m_height;i++)
    {
        for(int j=0;j<m_width;j++)
        {
            switch(m_mapData[getIndex (j, i)])
            {
            case EMPTY:
                cout<<" ";
                break;
            case WALL:
                cout<<"#";
                break;
            }
        }
        cout<<endl;
    }
}

unsigned int *DungeonGenerator::mapData() const
{
    return m_mapData;
}

void DungeonGenerator::setMapData(unsigned int *mapData)
{
    m_mapData = mapData;
}

int DungeonGenerator::width() const
{
    return m_width;
}

void DungeonGenerator::setWidth(int width)
{
    m_width = width;
}
int DungeonGenerator::height() const
{
    return m_height;
}

void DungeonGenerator::setHeight(int height)
{
    m_height = height;
}

int DungeonGenerator::getIndex(int x, int y)
{
    return  y * m_width + x;
}

int DungeonGenerator::randomPick(int chance)
{
    if(rand()%100 < chance)
    {
        return WALL;
    }else
    {
        return EMPTY;
    }
}

void DungeonGenerator::getEntrance(int &x, int &y)
{
    x = m_entrance_x;
    y = m_entrance_y;
}

void DungeonGenerator::getRandomPassablePos(int &x, int &y)
{
    int randIndex = rand() % m_emptyCellList.size();
    x = m_emptyCellList[randIndex] % m_width;
    y = m_emptyCellList[randIndex] / m_width;
    m_emptyCellList.erase(m_emptyCellList.begin() + randIndex);
}




