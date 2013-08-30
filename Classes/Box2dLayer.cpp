#include "Box2dLayer.h"
#include "Box2dPhysicsSprite.h"

USING_NS_CC;

Box2dLayer::Box2dLayer()
{
	m_width  = 20.0f;
	m_height = 20.0f;
}

Box2dLayer::~Box2dLayer()
{
	delete m_world;
	m_world = NULL;
}

bool Box2dLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !BaseLayer::init() )
    {
        return false;
    }

	setTouchEnabled(true);

	initWorld();
	initBody();

	scheduleUpdate();

    return true;
}

bool Box2dLayer::init(float width, float height)
{
	m_width = width/PTM_RATIO;
	m_height = height/PTM_RATIO;
	return init();
}

Box2dLayer * Box2dLayer::create(float width, float height)
{
	Box2dLayer *pRet = new Box2dLayer();
	if (pRet && pRet->init(width, height))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void Box2dLayer::initWorld()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);

	m_world->SetDebugDraw(&m_debugDraw);

	b2BodyDef bd;
	m_groundBody = m_world->CreateBody(&bd);

	b2EdgeShape shape;

	// Floor
	shape.Set(b2Vec2(-m_width/2, 0.0f), b2Vec2(m_width/2, 0.0f));
	m_groundBody->CreateFixture(&shape, 0.0f);

	// Left wall
	shape.Set(b2Vec2(-m_width/2, 0.0f), b2Vec2(m_width/2, m_height));
	m_groundBody->CreateFixture(&shape, 0.0f);

	// Right wall
	shape.Set(b2Vec2(m_width/2, 0.0f), b2Vec2(m_width/2, m_height));
	m_groundBody->CreateFixture(&shape, 0.0f);

	// Roof
	shape.Set(b2Vec2(-m_width/2, m_height), b2Vec2(m_width/2, m_height));
	m_groundBody->CreateFixture(&shape, 0.0f);
}

void Box2dLayer::initBody()
{
	m_mouseJoint = NULL;

	b2CircleShape shape;
	shape.m_radius = 1.0f;

	b2BodyDef bd;
	bd.type = b2_dynamicBody;

	b2Body * body = m_world->CreateBody(&bd);
	body->CreateFixture(&shape, 1.0f);
	body->SetLinearVelocity(b2Vec2(0.0f, +50.0f));

	Box2dPhysicsSprite *sprite = Box2dPhysicsSprite::create("ball.png");
	addChild(sprite);
	sprite->setBody(body);
	sprite->setPTMRatio(PTM_RATIO);
	sprite->setPosition( ccp( 0, 100) );
}

void Box2dLayer::Step(Settings* settings)
{
	float32 timeStep = settings->hz > 0.0f ? 1.0f / settings->hz : float32(0.0f);

	uint32 flags = 0;
	flags += settings->drawShapes            * b2Draw::e_shapeBit;
	flags += settings->drawJoints            * b2Draw::e_jointBit;
	flags += settings->drawAABBs            * b2Draw::e_aabbBit;
	flags += settings->drawPairs            * b2Draw::e_pairBit;
	flags += settings->drawCOMs                * b2Draw::e_centerOfMassBit;
	m_debugDraw.SetFlags(flags);

	m_world->Step(timeStep, settings->velocityIterations, settings->positionIterations);

	m_world->DrawDebugData();
}

Settings settings;

void Box2dLayer::update(float dt)
{
	Step(&settings);
}

void Box2dLayer::draw()
{
	BaseLayer::draw();

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

	kmGLPushMatrix();

	uint32 flags = 0;
	flags += settings.drawShapes            * b2Draw::e_shapeBit;
	flags += settings.drawJoints            * b2Draw::e_jointBit;
	flags += settings.drawAABBs            * b2Draw::e_aabbBit;
	flags += settings.drawPairs            * b2Draw::e_pairBit;
	flags += settings.drawCOMs               * b2Draw::e_centerOfMassBit;
	m_debugDraw.SetFlags(flags);
	m_world->DrawDebugData();

	kmGLPopMatrix();

	CHECK_GL_ERROR_DEBUG();
}

void Box2dLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10, true);
}

bool Box2dLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint local = touch->getLocation();
	CCLog("ccTouchBegan (%f, %f)", local.x, local.y);
	CCPoint nodePosition = convertToNodeSpace( local );
	CCLog("ccTouchBegan (%f, %f)", nodePosition.x, nodePosition.y);
	return MouseDown(b2Vec2(nodePosition.x/PTM_RATIO,nodePosition.y/PTM_RATIO));
}

void Box2dLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint local = touch->getLocation();
	CCLog("ccTouchBegan (%f, %f)", local.x, local.y);
	CCPoint nodePosition = convertToNodeSpace( local );
	CCLog("ccTouchBegan (%f, %f)", nodePosition.x, nodePosition.y);
	MouseMove(b2Vec2(nodePosition.x/PTM_RATIO, nodePosition.y/PTM_RATIO));
}

void Box2dLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint local = touch->getLocation();
	CCLog("ccTouchBegan (%f, %f)", local.x, local.y);
	CCPoint nodePosition = convertToNodeSpace( local );
	CCLog("ccTouchBegan (%f, %f)", nodePosition.x, nodePosition.y);
	MouseUp(b2Vec2(nodePosition.x/PTM_RATIO, nodePosition.y/PTM_RATIO));
}

//------------------------------------------------------------
class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(const b2Vec2& point)
	{
		m_point = point;
		m_fixture = NULL;
	}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;

				// We are done, terminate the query.
				return false;
			}
		}

		// Continue the query.
		return true;
	}

	b2Vec2 m_point;
	b2Fixture* m_fixture;
};


bool Box2dLayer::MouseDown(const b2Vec2& p)
{
	m_mouseWorld = p;

	if (m_mouseJoint != NULL)
	{
		return false;
	}

	// Make a small box.
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = p - d;
	aabb.upperBound = p + d;

	// Query the world for overlapping shapes.
	QueryCallback callback(p);
	m_world->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
	{
		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef md;
		md.bodyA = m_groundBody;
		md.bodyB = body;
		md.target = p;
		md.maxForce = 1000.0f * body->GetMass();
		m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&md);
		body->SetAwake(true);
		return true;
	}
	return false;
}

void Box2dLayer::MouseUp(const b2Vec2& p)
{
	if (m_mouseJoint)
	{
		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}
}

void Box2dLayer::MouseMove(const b2Vec2& p)
{
	m_mouseWorld = p;

	if (m_mouseJoint)
	{
		m_mouseJoint->SetTarget(p);
	}
}
