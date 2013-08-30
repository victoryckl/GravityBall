#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Box2dLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    pCloseItem->setPosition( ccp(getWinSize().width - 60, getWinSize().height - 60) );
	pCloseItem->setScale(1.5);

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

	initBox2dLayer();

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("hello, i am a question", "Thonburi", 34);

    // ask director the window size
    CCSize size = getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp((size.width - pCloseItem->getContentSize().width) / 2, size.height - 40) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(size.width/2, size.height/2) );

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::initBox2dLayer()
{
	CCPoint visibleOrigin = getVisibleOrigin();
	CCSize visibleSize = getVisibleSize();

	Box2dLayer * box2dLayer = Box2dLayer::create();
	addChild(box2dLayer, 2, kTagBox2dLayer);
	box2dLayer->setScale(15);
	box2dLayer->setAnchorPoint(ccp(0, 0));
	box2dLayer->setPosition( ccp(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height/3) );
}