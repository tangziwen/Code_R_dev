#ifndef ENEMY_H
#define ENEMY_H
#include "EnemyType.h"
#include "Player.h"
#include "GameMap.h"
#include "cocos2d.h"
#include "DynamicObstacle.h"
class Enemy{
public:
    enum class EnemyState
    {
        Idle,
        Chase,
        Attack,
    };
    Enemy(EnemyType * type);
    EnemyType * getType();
    void randomlyWalk(GameMap *the_map);
    virtual bool isItDead();
    virtual int getDamage();
    virtual void hurt(int damage);
    virtual void step(Player * player, GameMap *the_map);
    virtual bool seenPlayer(Player * player, GameMap * the_map);
    void setPos(int x, int y);
    void getPos(int &x, int &y);
    void setSprite(Sprite * sprite);
    Sprite * getSprite();
    EnemyState getState();
    void setState(EnemyState newState);
    DynamicObstacle * getDynamicObstacle();
private:
    bool isNeighborToPlayer(Player * player);
private:
    std::vector <GamePoint> m_chasePath;
    EnemyState m_state;
    Sprite * m_sprite;
    int m_posX, m_posY;
    bool m_isItDead;
    int m_hp;
    EnemyType * m_type;
    DynamicObstacle * m_obstacle;
};
#endif