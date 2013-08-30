#ifndef __BASE_LAYER_H__
#define __BASE_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class BaseLayer : public CCLayer
{
public:
	BaseLayer();
	~BaseLayer();

	virtual bool init();

	CCSize getWinSize();
	CCSize getVisibleSize();
	CCPoint getVisibleOrigin();

	void setBackgroundImage( const char* back_image_name );
};

#endif // __BASE_LAYER_H__
