#include "Enemy.h"
#include "GameInfo.h"
Enemy::Enemy(EnemyType * type)
:m_posX(0), m_posY(0), m_state(EnemyState::Idle)
{
    m_type = type;
    m_hp = type->getHp();
    m_isItDead = false;
    m_chasePath.clear();
    m_obstacle = new DynamicObstacle(m_posX,m_posY);
}

int Enemy::getDamage()
{
    return m_type->getDamage();
}

void Enemy::hurt(int damage)
{
    m_hp -= damage;
    if (m_hp <= 0)
    {
        m_isItDead = true;
    }
}

void Enemy::step(Player * player, GameMap *the_map)
{

    switch (m_state)
    {
    case Enemy::EnemyState::Idle:
    {
        if (seenPlayer(player, the_map))
        {
            m_state = EnemyState::Chase;
        }
        else{
            // Randomly walk around
            randomlyWalk(the_map);
        }
    }
    break;
    case Enemy::EnemyState::Chase:
    {
        if (isNeighborToPlayer(player))
        {
            m_chasePath.clear();
            //attack immediatly
            player->hurt(100);
            //turn to attack state
            this->m_state = EnemyState::Attack;
        }
        else{
            //get the path to player.
            the_map->getPath(m_posX, m_posY, player->getX(), player->getY(), m_chasePath);
            setPos(m_chasePath[1].x, m_chasePath[1].y);
        }
    }
    break;
    case Enemy::EnemyState::Attack:
    {
        if (isNeighborToPlayer(player))
        {
            //attack.
            player->hurt(100);
        }
        else
        {
            //chase again.
            this->m_state = EnemyState::Chase;
        }
    }
    break;
    default:
    break;
    }
}

EnemyType * Enemy::getType()
{
    return m_type;
}

void Enemy::setPos(int x, int y)
{
    m_posX = x;
    m_posY = y;
    m_sprite->setPosition(Vec2(x*TILE_WIDTH, y*TILE_HEIGHT));
    m_obstacle->m_x = x;
    m_obstacle->m_y = y;
}

void Enemy::getPos(int &x, int &y)
{
    x = m_posX;
    y = m_posY;
}

bool Enemy::isItDead()
{
    return m_isItDead;
}

void Enemy::setSprite(Sprite * sprite)
{
    m_sprite = sprite;
}

Sprite * Enemy::getSprite()
{
    return m_sprite;
}

Enemy::EnemyState Enemy::getState()
{
    return m_state;
}

bool Enemy::seenPlayer(Player * player, GameMap * the_map)
{
    Vec2 p(player->getX(), player->getY());
    Vec2 e(m_posX, m_posY);
    if (p.distance(e) < 5)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Enemy::isNeighborToPlayer(Player * player)
{
    int x_dist = abs(player->getX() - m_posX);
    int y_dist = abs(player->getY() - m_posY);
    if (x_dist + y_dist <= 1)
    {
        return true; 
    } 
    else
    {
        return false;
    }
}

DynamicObstacle * Enemy::getDynamicObstacle()
{
    return m_obstacle;
}

void Enemy::randomlyWalk(GameMap *the_map)
{
    int rand_dir =  rand()%5;
    int x, y;
    switch (rand_dir)
    {
    case 0:
    x = m_posX;
    y = m_posY -1;
    break;
    case 1:
    x = m_posX;
    y = m_posY + 1;
    break;
    case 2:
    x = m_posX -1;
    y = m_posY;
    break;
    case 3:
    x = m_posX + 1;
    y = m_posY;
    break;
    case 4:
    //stay here
    x = m_posX;
    y = m_posY;
    break;
    default:
    break;
    }
    if (!the_map->isObstacle(x, y))
    {
        setPos(x,y);
    }
}
