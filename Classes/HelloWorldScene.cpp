#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Box2dLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;


bool HelloWorldScene::init()
{
	if ( !CCScene::init() )
	{
		return false;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png","CloseSelected.png",this,menu_selector(HelloWorldScene::menuCloseCallback));
	CCMenuItemImage *pResetItem = CCMenuItemImage::create("ResetNormal.png","ResetSelected.png",this,menu_selector(HelloWorldScene::menuResetCallback));
	pCloseItem->setPosition( ccp(size.width - 60, size.height - 60) );
	pResetItem->setPosition( ccp(size.width - 60, size.height - 180) );
	pCloseItem->setScale(1.5);
	pResetItem->setScale(1.5);

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, pResetItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	addChild(HelloWorld::create());

	addBox2dLayer();

	return true;
}

void HelloWorldScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void HelloWorldScene::menuResetCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(HelloWorldScene::create());
}

void HelloWorldScene::addBox2dLayer()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	Box2dLayer * box2dLayer = Box2dLayer::create(size.width, size.height);
	addChild(box2dLayer, 2, kTagBox2dLayer);
	box2dLayer->setScale(1.0);
	box2dLayer->setAnchorPoint(ccp(0.5, 0));
	box2dLayer->setPosition( ccp(size.width/2, 0) );
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
    // 2. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("hello, i am a question", "Thonburi", 34);
    // ask director the window size
    CCSize size = getWinSize();
    // position the label on the center of the screen
    pLabel->setPosition( ccp((size.width - 60) / 2, size.height - 40) );
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