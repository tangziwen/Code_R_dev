#ifndef  GAME_ITEM_H
#define GAME_ITEM_H
#include "ItemType.h"
#include "cocos2d.h"
struct GameItem
{
    ItemType * item;
    cocos2d::Sprite * sprite;
    int x;
    int y;
};
#endif // ! GAME_ITEM_H
