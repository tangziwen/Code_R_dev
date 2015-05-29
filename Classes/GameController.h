#ifndef GAME_PAINTER_H
#define GAME_PAINTER_H
#include "cocos2d.h"
#include "GameMap.h"
#include "GameCamera.h"
#include "Player.h"
#include "ItemType.h"
#include "GameItem.h"
#include "Enemy.h"
class GameController
{
public:
    GameController();
    void genGraphics();
    Layer * getGameLayer();
    void updateOneTurn();
    void camMove(int offset_x, int offset_y); // move the camera
    void playerMove(int offset_x, int offset_y);
    void playerSet(int pos_x, int pos_y);
    void addEnemy(EnemyType * type, int x, int y);
    void addEnemy(unsigned int id, int x, int y);
    void addEnemy(std::string name, int x, int y);
    void addItem(ItemType * item, int x, int y);
    void addItem(unsigned int id, int x, int y);
    void addItem(std::string name, int x, int y);
    Enemy * isThatAEnemy(int x, int y);
    void reGen();
    void removeAllEnemies();
    void removeAllItems();
private:
    void cameraFollow(int offset_x, int offset_y);// camera follow the player.
    void processEnemies();
private:
    GameMap * m_gameMap;
    GameCamera * m_gameCamera;
    Player * m_player;
    Layer * m_layer;
    Sprite * m_playerSprite;
    std::vector<Sprite *> m_tileList;
    std::vector<GameItem *> m_itemList;
    std::vector<Enemy *> m_enemyList;
};
#endif