
#include "GameCamera.h"

void GameCamera::setPos(int x, int y)
{
    m_pos_x = x;
    m_pos_y = y;
}

GameCamera::GameCamera()
: m_pos_x(0), m_pos_y(0)
{

}

int GameCamera::getX()
{
    return m_pos_x;
}

int GameCamera::getY()
{
    return m_pos_y;
}

