#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "BaseLayer.h"

class HelloWorldScene : public CCScene
{
public: 
	bool init();
	CREATE_FUNC(HelloWorldScene);

	void menuCloseCallback(CCObject* pSender);
	void menuResetCallback(CCObject* pSender);

	void addBox2dLayer();
};


class HelloWorld : public BaseLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
	// implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
