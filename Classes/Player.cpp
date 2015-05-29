#include "Player.h"
#include "cocos2d.h"
Player::Player(int x, int y, PlayerDesc desc)
    :m_pos_x(x), m_pos_y(y), m_eyeSight(7)
{
    setDesc(desc);
    m_hp = getMaxHp();
}

void Player::setPos(int x, int y)
{
    m_pos_x = x;
    m_pos_y = y;
}

int Player::getX()
{
    return m_pos_x;
}

int Player::getY()
{
    return m_pos_y;
}

void Player::setEyeSight(int distance)
{
    m_eyeSight = distance;
}

int Player::getEyeSight()
{
    return m_eyeSight;
}

int Player::getDamage()
{
    return m_strength * 5;
}

void Player::setStrength(int strength)
{
    m_strength = strength;
}

int Player::getStrength()
{
    return m_strength;
}

void Player::setAgility(int agility)
{
    m_agility = agility;
}

int Player::getAgility()
{
    return m_agility;
}

void Player::setWisedom(int wisdom)
{
    m_wisedom = wisdom;
}

int Player::getWisedom()
{
    return m_wisedom;
}

void Player::setDesc(PlayerDesc desc)
{
    setStrength(desc.strength);
    setAgility(desc.agility);
    setWisedom(desc.agility);
}

int Player::getMaxHp()
{
    return m_strength*8;
}

void Player::hurt(int damage)
{
    m_hp -= damage;
    if (m_hp <= 0)
    {
        CCLOG("dead");
    }
}
