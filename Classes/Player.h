#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerDesc.h"
class Player
{
public:
    Player(int x, int y, PlayerDesc desc);
    void setDesc(PlayerDesc desc);
    void setPos(int x, int y);
    void setEyeSight(int distance);
    int getEyeSight();
    int getX();
    int getY();
    int getDamage();

    void setStrength(int strength);
    int getStrength();
    void setAgility(int agility);
    int getAgility();
    void setWisedom(int wisdom);
    int getWisedom();
    int getMaxHp();
    void setHp(int hp);
    int getHp();
    void hurt(int damage);
protected:
private:
    int m_hp;
    int m_strength;
    int m_agility;
    int m_wisedom;
    int m_eyeSight;
    int m_pos_x;
    int m_pos_y;
};
#endif