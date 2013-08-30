#include "BaseLayer.h"

USING_NS_CC;

BaseLayer::BaseLayer(){}
BaseLayer::~BaseLayer(){}

bool BaseLayer::init(){

	bool sRect = false;

	do {
		CC_BREAK_IF( !CCLayer::init() );

		sRect = true;
	} while ( 0 );

	return sRect;
}

CCSize BaseLayer::getWinSize()
{
    return CCDirector::sharedDirector()->getWinSize();
}

CCSize BaseLayer::getVisibleSize()
{
	return CCDirector::sharedDirector()->getVisibleSize();
}

CCPoint BaseLayer::getVisibleOrigin()
{
	return CCDirector::sharedDirector()->getVisibleOrigin();
}

void BaseLayer::setBackgroundImage( const char* back_image_name )
{
	CCSprite* sp = CCSprite::create( back_image_name );
	sp->setPosition( ccp( getWinSize().width / 2 , getWinSize().height / 2 ) );
	addChild( sp );
}