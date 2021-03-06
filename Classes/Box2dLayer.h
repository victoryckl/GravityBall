#ifndef __BOX2D_LAYER_H__
#define __BOX2D_LAYER_H__

#include <Box2D/Box2D.h>
#include "GLES-Render.h"

#include "cocos2d.h"
#include "BaseLayer.h"
#include "Box2dPhysicsSprite.h"


/// Test settings. Some can be controlled in the GUI.
struct Settings
{
	Settings() :
	viewCenter(0.0f, 20.0f),
	hz(60.0f),
	velocityIterations(8),
	positionIterations(3),
	drawShapes(1),
	drawJoints(1),
	drawAABBs(0),
	drawPairs(0),
	drawContactPoints(0),
	drawContactNormals(0),
	drawContactForces(0),
	drawFrictionForces(0),
	drawCOMs(0),
	drawStats(0),
	drawProfile(0),
	enableWarmStarting(1),
	enableContinuous(1),
	enableSubStepping(0),
	pause(0),
	singleStep(0)
{}

b2Vec2 viewCenter;
float32 hz;
int32 velocityIterations;
int32 positionIterations;
int32 drawShapes;
int32 drawJoints;
int32 drawAABBs;
int32 drawPairs;
int32 drawContactPoints;
int32 drawContactNormals;
int32 drawContactForces;
int32 drawFrictionForces;
int32 drawCOMs;
int32 drawStats;
int32 drawProfile;
int32 enableWarmStarting;
int32 enableContinuous;
int32 enableSubStepping;
int32 pause;
int32 singleStep;
};


enum 
{
	kTagBox2dLayer,
	e_count = 1,
};

class Box2dLayer;

// This is called when a joint in the world is implicitly destroyed
// because an attached body is destroyed. This gives us a chance to
// nullify the mouse joint.
class DestructionListener : public b2DestructionListener
{
public:
	void SayGoodbye(b2Fixture* fixture) { B2_NOT_USED(fixture); }
	void SayGoodbye(b2Joint* joint);

	Box2dLayer * box2dlayer;
};

class Box2dLayer : public BaseLayer
{
public:
	Box2dLayer();
	~Box2dLayer();
    virtual bool init();
	bool init(float width, float height);
    CREATE_FUNC(Box2dLayer);
	static Box2dLayer * create(float width, float height);

	void initWorld();
	void initMaze();
	void initBall();

	void createBox(float x, float y);
	void createCircle(float x, float y);

	void update(float dt);
	void Step(Settings* settings);
	void draw();

	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

	virtual bool MouseDown(const b2Vec2& p);
	virtual void MouseUp(const b2Vec2& p);
	void MouseMove(const b2Vec2& p);

	virtual void didAccelerate(CCAcceleration* pAccelerationValue);

	virtual void JointDestroyed(b2Joint* joint) { B2_NOT_USED(joint); }

	//"0,0 0,448 64,448 64,576" -> ccCArray points
	ccCArray * tokenizerPoints(int baseX, int baseY, CCString* pointsStr);

	void createPolyline(ccCArray * pArray);

public:
	b2World* m_world;
	b2Body* m_groundBody;
	GLESDebugDraw m_debugDraw;

	b2Body* m_bodies[e_count];

	float m_width, m_height;
	b2MouseJoint* m_mouseJoint;
	b2Vec2 m_mouseWorld;

	DestructionListener m_destructionListener;

	struct {
		float32 x,y,w,h;
	}box;

	CCTMXTiledMap * m_pMap;
	CCTMXObjectGroup* m_pGroup;
	CCArray* m_pObjects;
	CCDictionary* m_pDict;
	CCObject* m_pObj;
	ccCArray * m_pPoints;
};

#endif // __BOX2D_LAYER_H__
