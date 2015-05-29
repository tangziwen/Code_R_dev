#include "GameScene.h"
#include "TileMgr.h"
#include "ItemMgr.h"
#include "EnemyTypeMgr.h"
enum class ArrowBtnTag{
    Left,
    Right,
    Up,
    Down
};

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    loadConfig();
    m_painter = new GameController();
    m_painter->camMove(-15, 0);
    m_painter->updateOneTurn();

    addChild(m_painter->getGameLayer());
    initGUI();
    addMarks();
    return true;
}


void GameScene::arrowButtonCallback(Ref* pSender)
{
    Node * sender = (Node *)pSender;
    auto tag = (ArrowBtnTag)sender->getTag();
    switch (tag)
    {
    case ArrowBtnTag::Left:
    {
        m_painter->playerMove(-1, 0); 
    }
        break;
    case ArrowBtnTag::Right:
    {
        m_painter->playerMove(1, 0);
    }
        break;
    case ArrowBtnTag::Down:
    {
        m_painter->playerMove(0, -1);
    }
        break;
    case ArrowBtnTag::Up:
    {
        m_painter->playerMove(0, 1);
    }
        break;
    }
    m_painter->updateOneTurn();

}

void GameScene::loadConfig()
{
    TileMgr::get()->loadTileConfig("tiles/tiles.xml");
    ItemMgr::get()->loadConfig("items/items.xml");
    EnemyTypeMgr::get()->loadConfig("enemies/enemies.xml");
}

void GameScene::initGUI()
{
    auto leftBtn = MenuItemImage::create(
        "left.png",
        "left.png",
        CC_CALLBACK_1(GameScene::arrowButtonCallback, this));
    leftBtn->setAnchorPoint(Vec2(0, 0));
    leftBtn->setPosition(Vec2(0, leftBtn->getContentSize().height));
    leftBtn->setTag((int)ArrowBtnTag::Left);

    auto rightBtn = MenuItemImage::create(
        "right.png",
        "right.png",
        CC_CALLBACK_1(GameScene::arrowButtonCallback, this));
    rightBtn->setAnchorPoint(Vec2(0, 0));
    rightBtn->setPosition(Vec2(2 * rightBtn->getContentSize().width, rightBtn->getContentSize().height));
    rightBtn->setTag((int)ArrowBtnTag::Right);

    auto upBtn = MenuItemImage::create(
        "up.png",
        "up.png",
        CC_CALLBACK_1(GameScene::arrowButtonCallback, this));
    upBtn->setAnchorPoint(Vec2(0, 0));
    upBtn->setPosition(Vec2(upBtn->getContentSize().width, upBtn->getContentSize().height * 2));
    upBtn->setTag((int)ArrowBtnTag::Up);

    auto downBtn = MenuItemImage::create(
        "down.png",
        "down.png",
        CC_CALLBACK_1(GameScene::arrowButtonCallback, this));
    downBtn->setAnchorPoint(Vec2(0, 0));
    downBtn->setPosition(Vec2(downBtn->getContentSize().width, 0));
    downBtn->setTag((int)ArrowBtnTag::Down);

    auto restBtn = MenuItemImage::create(
        "rest.png",
        "rest.png",
        CC_CALLBACK_1(GameScene::resetButtonCallback, this));
    restBtn->setAnchorPoint(Vec2(0, 0));
    restBtn->setPosition(Vec2(restBtn->getContentSize().width, restBtn->getContentSize().height * 3));

    // create menu, it's an autorelease object
    auto menu = Menu::create(leftBtn, rightBtn, upBtn, downBtn, restBtn, NULL);
    menu->setPosition(Vec2::ZERO);


    this->addChild(menu, 1);
}

void GameScene::resetButtonCallback(cocos2d::Ref* pSender)
{
    m_painter->updateOneTurn();
}

void GameScene::addMarks()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto label = Label::createWithTTF("Code R by tangziwen", "fonts/arial.ttf", 16);
    label->setPosition(visibleSize.width - label->getContentSize().width, label->getContentSize().height);
    addChild(label, 100);
}
