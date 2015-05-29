#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#include "cocos2d.h"
#include "GameController.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void arrowButtonCallback(cocos2d::Ref* pSender);
    
    void resetButtonCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
private:
    void initGUI();
    void loadConfig();
    void addMarks();
private:
    GameMap * m_Map;
    Player * m_player;
    GameCamera * m_camera;
    GameController * m_painter;

};

#endif // __HELLOWORLD_SCENE_H__
