#ifndef ENEMY_TYPE_H
#define ENEMY_TYPE_H
#include <string>

class EnemyType
{
public:
    EnemyType(unsigned int id);
    std::string getName();
    void setName(std::string newName);

    std::string getDescription();
    void setItemDescription(std::string new_description);

    std::string getResPath();
    void setResPath(std::string new_path);

    int getDamage();
    void setDamage(int new_damage);

    int getHp();
    void setHp(int hp);

    unsigned int getID();
private:
    std::string m_name;
    std::string m_description;
    std::string m_resPath;
    unsigned int m_ID;
    float m_damage;
    float m_hp;
};
#endif