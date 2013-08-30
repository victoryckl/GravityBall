#include "Box2dLayer.h"

USING_NS_CC;

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

	initWorld();
	initBody();

	schedule( schedule_selector(Box2dLayer::tick) );
    return true;
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
	shape.Set(b2Vec2(-10.0f, 0.0f), b2Vec2(10.0f, 0.0f));
	m_groundBody->CreateFixture(&shape, 0.0f);

	// Left wall
	shape.Set(b2Vec2(-10.0f, 0.0f), b2Vec2(-10.0f, 20.0f));
	m_groundBody->CreateFixture(&shape, 0.0f);

	// Right wall
	shape.Set(b2Vec2(10.0f, 0.0f), b2Vec2(10.0f, 20.0f));
	m_groundBody->CreateFixture(&shape, 0.0f);

	// Roof
	shape.Set(b2Vec2(-10.0f, 20.0f), b2Vec2(10.0f, 20.0f));
	m_groundBody->CreateFixture(&shape, 0.0f);
}

void Box2dLayer::initBody()
{
	{
		b2BodyDef bd;
		b2Body* ground = m_world->CreateBody(&bd);

		b2EdgeShape shape;
		shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
		ground->CreateFixture(&shape, 0.0f);
	}

	{
		b2CircleShape shape;
		shape.m_radius = 1.0f;

		for (int32 i = 0; i < e_count; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0, 4.0f + 3.0f * i);

			m_bodies[i] = m_world->CreateBody(&bd);

			m_bodies[i]->CreateFixture(&shape, 1.0f);

			m_bodies[i]->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
		}
	}
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

void Box2dLayer::tick(float dt)
{
	Step(&settings);
}

void Box2dLayer::draw()
{
	BaseLayer::draw();

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

	kmGLPushMatrix();

	m_world->DrawDebugData();

	kmGLPopMatrix();

	CHECK_GL_ERROR_DEBUG();
}