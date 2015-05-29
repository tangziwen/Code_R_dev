#include "EnemyType.h"

EnemyType::EnemyType(unsigned int id)
    :m_ID(id)
{

}

std::string EnemyType::getName()
{
    return m_name;
}

void EnemyType::setName(std::string newName)
{
    m_name = newName;
}

std::string EnemyType::getDescription()
{
    return m_description;
}

void EnemyType::setItemDescription(std::string new_description)
{
    m_description = new_description;
}

std::string EnemyType::getResPath()
{
    return m_resPath;
}

void EnemyType::setResPath(std::string new_path)
{
    m_resPath = new_path;
}

int EnemyType::getDamage()
{
    return m_damage;
}

void EnemyType::setDamage(int new_damage)
{
    m_damage = new_damage;
}

int EnemyType::getHp()
{
    return m_hp;
}

void EnemyType::setHp(int hp)
{
    m_hp = hp;
}

unsigned int EnemyType::getID()
{
    return m_ID;
}
