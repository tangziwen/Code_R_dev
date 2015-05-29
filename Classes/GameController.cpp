#include <algorithm>
#include "GameController.h"
#include "GameInfo.h"
#include "TileMgr.h"
#include "ItemMgr.h"
#include "EnemyTypeMgr.h"
GameController::GameController()
{
    m_layer = Layer::create();
    PlayerDesc desc(20, 30, 15);
    m_player = new Player(0, 0, desc);
    m_gameCamera = new GameCamera();
    reGen();
}

void GameController::genGraphics()
{
    for (int i = 0; i < m_gameMap->getHeight(); i++)
    {
        for (int j = 0; j < m_gameMap->getWidth(); j++)
        {
            auto sprite = Sprite::create(TileMgr::get()->findTile(m_gameMap->getCellData(j, i))->getResPath());
            sprite->setAnchorPoint(Vec2(0, 0));
            sprite->setPosition(Vec2(j*TILE_WIDTH, i*TILE_HEIGHT));
            m_layer->addChild(sprite);
            m_tileList.push_back(sprite);
        }
    }
    m_playerSprite = Sprite::create("player.png");
    m_playerSprite->setAnchorPoint(Vec2(0, 0));
    m_playerSprite->setPosition(Vec2(m_player->getX()*TILE_WIDTH, m_player->getY()*TILE_HEIGHT));
    m_layer->addChild(m_playerSprite, PLAYER_ZORDER);
}

Layer * GameController::getGameLayer()
{
    return m_layer;
}

void GameController::updateOneTurn()
{
    //update player's world space position
    m_playerSprite->setPosition(m_player->getX()*TILE_WIDTH, m_player->getY()*TILE_HEIGHT);

    //transform to view space
    m_layer->setPosition(Vec2(-1*m_gameCamera->getX()*TILE_WIDTH,-1*m_gameCamera->getY()*TILE_HEIGHT));
    
    //draw shadow
    for (int i = m_gameCamera->getY(); i < m_gameCamera->getY() + SCREEN_TILE_HEIGHT; i++)
    {
        for (int j = m_gameCamera->getX(); j < m_gameCamera->getX() + SCREEN_TILE_WIDTH; j++)
        {
            int shadowLevel = 0;
            m_gameMap->getSightLevel(m_player->getX(), m_player->getY(), j, i, m_player->getEyeSight(), shadowLevel);
            switch (shadowLevel)
            {
            case 0:
                m_tileList[i*(m_gameMap->getWidth()) + j]->setColor(Color3B(255, 255, 255));
                break;
            case 1:
                m_tileList[i*(m_gameMap->getWidth()) + j]->setColor(Color3B(195, 195, 195));
                break;
            case 2:
                m_tileList[i*(m_gameMap->getWidth()) + j]->setColor(Color3B(135, 135, 135));
                break;
            case 3:
                m_tileList[i*(m_gameMap->getWidth()) + j]->setColor(Color3B(70, 70, 70));
                break;
            case 4:
                m_tileList[i*(m_gameMap->getWidth()) + j]->setColor(Color3B(35, 35, 35));
                break;
            }
            
        }
    }
    //draw item
    for (int i = 0; i < m_itemList.size(); i++)
    {
        int shadowLevel;
        m_gameMap->getSightLevel(m_player->getX(), m_player->getY(), m_itemList[i]->x, m_itemList[i]->y, m_player->getEyeSight(), shadowLevel);
        if (shadowLevel == 4)
        {
            m_itemList[i]->sprite->setVisible(false);
        }
        else
        {
            m_itemList[i]->sprite->setVisible(true);
        }
    }

    processEnemies();
}

void GameController::camMove(int offset_x, int offset_y)
{
    int x = m_gameCamera->getX();
    int y = m_gameCamera->getY();

    x += offset_x;
    y += offset_y;

    int dist_to_left = x;
    int dist_to_right = m_gameMap->getWidth() - x;
    int dist_to_top = m_gameMap->getHeight() - y;
    int dist_to_bottom = y;

    //check the camera-map boundary
    if (dist_to_left >= 0 && dist_to_right >= SCREEN_TILE_WIDTH && dist_to_bottom >= 0 && dist_to_top >= SCREEN_TILE_HEIGHT)
    {
        m_gameCamera->setPos(x, y);
    }
    else
    {
        //do nothing.
    }
}

void GameController::playerMove(int offset_x, int offset_y)
{
    int x = m_player->getX();
    int y = m_player->getY();

    x += offset_x;
    y += offset_y;
    auto enemy = isThatAEnemy(x, y);
    if (x >= 0 && x < m_gameMap->getWidth() && y >= 0 && y < m_gameMap->getHeight()
        && !m_gameMap->isObstacle(x, y) && !enemy)
    {
        m_player->setPos(x, y);

        //let the camera follow player's movement.
        cameraFollow(offset_x,offset_y);
    }
    else if (enemy) // meet enemy?
    {
        enemy->hurt(m_player->getDamage());
        //do nothing.
    }
}

void GameController::cameraFollow(int offset_x, int offset_y)
{
    //camera follow, only if the player is in the centre.
    if (offset_x < 0) //left
    {
        if (m_player->getX() - m_gameCamera->getX() <= SCREEN_TILE_WIDTH * 1 / 4)
        {
            camMove(offset_x, 0);
        }
    }
    else if (offset_x > 0) //right
    {
        if (m_player->getX() - m_gameCamera->getX() >= SCREEN_TILE_WIDTH * 3 / 4)
        {
            camMove(offset_x, 0);
        }
    }

    if (offset_y < 0) //down
    {
        if (m_player->getY() - m_gameCamera->getY() <= SCREEN_TILE_HEIGHT * 1 / 4)
        {
            camMove(0, offset_y);
        }
    }
    else if (offset_y > 0) //up
    {
        if (m_player->getY() - m_gameCamera->getY() >= SCREEN_TILE_HEIGHT * 3 / 4)
        {
            camMove(0, offset_y);
        }
    }
}

void GameController::playerSet(int pos_x, int pos_y)
{
    m_player->setPos(pos_x, pos_y);

    // camera locale the player.
    int cam_x = pos_x - (SCREEN_TILE_WIDTH / 2 + 1);
    int cam_y = pos_y - (SCREEN_TILE_HEIGHT / 2 + 1);
    cam_x = std::max(0,cam_x);
    cam_y = std::max(0, cam_y);
    m_gameCamera->setPos(cam_x, cam_y);
}

void GameController::addItem(ItemType * item, int x, int y)
{
    auto i = new GameItem();
    i->item = item;
    i->sprite = Sprite::create(item->getResPath());
    i->sprite->setAnchorPoint(Vec2(0, 0));
    i->sprite->setPosition(Vec2(x*TILE_WIDTH, y*TILE_HEIGHT));
    i->x = x;
    i->y = y;
    m_itemList.push_back(i);
    m_layer->addChild(i->sprite,ITEM_ZORDER);
}

void GameController::addItem(unsigned int id, int x, int y)
{
    addItem(ItemMgr::get()->findItem(id), x, y);
}

void GameController::addItem(std::string name, int x, int y)
{
    addItem(ItemMgr::get()->findItem(name.c_str()), x, y); 
}

void GameController::addEnemy(EnemyType * type, int x, int y)
{
    auto enemy = new Enemy(type);
    auto sprite = Sprite::create(enemy->getType()->getResPath());
    sprite->setAnchorPoint(Vec2(0, 0));
    enemy->setSprite(sprite);
    enemy->setPos(x, y);
    m_layer->addChild(sprite, ENEMY_ZORDER);
    m_enemyList.push_back(enemy);
    m_gameMap->addDynamicObstacle(enemy->getDynamicObstacle());
}

void GameController::addEnemy(unsigned int id, int x, int y)
{
    addEnemy(EnemyTypeMgr::get()->findItem(id), x, y);
}

void GameController::addEnemy(std::string name, int x, int y)
{
    addEnemy(EnemyTypeMgr::get()->findItem(name.c_str()), x, y);
}

void GameController::processEnemies()
{
    //remove dead enemy
    for (auto i = m_enemyList.begin(); i != m_enemyList.end();)
    {
        if ((*i)->isItDead())
        {
            Enemy * e = (*i);
            m_layer->removeChild(e->getSprite());
            m_gameMap->removeDynamicObstacle(e->getDynamicObstacle());
            delete e;
            i = m_enemyList.erase(i);
        }
        else
        {
            i++;
        }
    }
    //draw enemy
    for (int i = 0; i < m_enemyList.size(); i++)
    {
        //enemy step
        m_enemyList[i]->step(m_player, m_gameMap);

        int shadowLevel;
        int x, y;
        m_enemyList[i]->getPos(x, y);
        m_gameMap->getSightLevel(m_player->getX(), m_player->getY(), x, y, m_player->getEyeSight(), shadowLevel);
        if (shadowLevel == 4)
        {
            m_enemyList[i]->getSprite()->setVisible(false);
        }
        else
        {
            m_enemyList[i]->getSprite()->setVisible(true);
        }
    }
}

Enemy* GameController::isThatAEnemy(int x, int y)
{
    for (int i = 0; i < m_enemyList.size(); i++)
    {
        int e_x, e_y;
        m_enemyList[i]->getPos(e_x, e_y);
        if (e_x == x && e_y == y)
        {
            return m_enemyList[i];
        }
    }
    return NULL;
}

void GameController::reGen()
{
    removeAllEnemies();//remove the enemy.
    removeAllItems(); // remove all the items.

    m_layer->removeAllChildren();//remove all sprite.

    m_gameMap = new GameMap(30, 50);
    m_gameMap->generateMap(GameMap::GeneratePolicy::Dungeon);
    int entrance_x, entrance_y;
    m_gameMap->getEntrance(entrance_x, entrance_y);


    playerSet(entrance_x, entrance_y);

    //for test, add four apple
    for (int i = 0; i < 4; i++)
    {
        int x, y;
        m_gameMap->getRandomPosition(x, y);
        addItem("apple", x, y);
    }

    //for test, add four enemy
    for (int i = 0; i < 4; i++)
    {
        int x, y;
        m_gameMap->getRandomPosition(x, y);
        addEnemy("bat", x, y);
    }
    genGraphics();
}

void GameController::removeAllEnemies()
{
    for (auto i = m_enemyList.begin(); i != m_enemyList.end(); i++)
    {
        Enemy * e = (*i);
        e->getSprite()->removeFromParent();
        delete e;
    }
    m_enemyList.clear();
}

void GameController::removeAllItems()
{
    for (auto i = m_itemList.begin(); i != m_itemList.end(); i++)
    {
        GameItem*  e = (*i);
        e->sprite->removeFromParent();
        delete e;
    }
    m_enemyList.clear();
}
