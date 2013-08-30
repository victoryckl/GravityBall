#include "Box2dPhysicsSprite.h"
#include "support/CCPointExtension.h"

#include "Box2D/Box2D.h"

Box2dPhysicsSprite::Box2dPhysicsSprite()
: m_bIgnoreBodyRotation(false)
, m_pB2Body(NULL)
, m_fPTMRatio(PTM_RATIO)
{}

Box2dPhysicsSprite* Box2dPhysicsSprite::create()
{
    Box2dPhysicsSprite* pRet = new Box2dPhysicsSprite();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

Box2dPhysicsSprite* Box2dPhysicsSprite::createWithTexture(CCTexture2D *pTexture)
{
    Box2dPhysicsSprite* pRet = new Box2dPhysicsSprite();
    if (pRet && pRet->initWithTexture(pTexture))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

Box2dPhysicsSprite* Box2dPhysicsSprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
    Box2dPhysicsSprite* pRet = new Box2dPhysicsSprite();
    if (pRet && pRet->initWithTexture(pTexture, rect))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

Box2dPhysicsSprite* Box2dPhysicsSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    Box2dPhysicsSprite* pRet = new Box2dPhysicsSprite();
    if (pRet && pRet->initWithSpriteFrame(pSpriteFrame))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

Box2dPhysicsSprite* Box2dPhysicsSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    Box2dPhysicsSprite* pRet = new Box2dPhysicsSprite();
    if (pRet && pRet->initWithSpriteFrameName(pszSpriteFrameName))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

Box2dPhysicsSprite* Box2dPhysicsSprite::create(const char *pszFileName)
{
    Box2dPhysicsSprite* pRet = new Box2dPhysicsSprite();
    if (pRet && pRet->initWithFile(pszFileName))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

Box2dPhysicsSprite* Box2dPhysicsSprite::create(const char *pszFileName, const CCRect& rect)
{
    Box2dPhysicsSprite* pRet = new Box2dPhysicsSprite();
    if (pRet && pRet->initWithFile(pszFileName, rect))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}

// this method will only get called if the sprite is batched.
// return YES if the physic's values (angles, position ) changed.
// If you return NO, then nodeToParentTransform won't be called.
bool Box2dPhysicsSprite::isDirty()
{
    return true;
}

bool Box2dPhysicsSprite::isIgnoreBodyRotation() const
{
    return m_bIgnoreBodyRotation;
}

void Box2dPhysicsSprite::setIgnoreBodyRotation(bool bIgnoreBodyRotation)
{
    m_bIgnoreBodyRotation = bIgnoreBodyRotation;
}

b2Body* Box2dPhysicsSprite::getBody() const
{
    return m_pB2Body;
}

void Box2dPhysicsSprite::setBody(b2Body *pBody)
{
    m_pB2Body = pBody;
}

float Box2dPhysicsSprite::getPTMRatio() const
{
    return m_fPTMRatio;
}

void Box2dPhysicsSprite::setPTMRatio(float fRatio)
{
    m_fPTMRatio = fRatio;
}

// Override the setters and getters to always reflect the body's properties.
const CCPoint& Box2dPhysicsSprite::getPosition()
{
    b2Vec2 pos = m_pB2Body->GetPosition();
    
    float x = pos.x * m_fPTMRatio;
    float y = pos.y * m_fPTMRatio;
    m_obPosition = ccp(x,y);
    return m_obPosition;
}

void Box2dPhysicsSprite::setPosition(const CCPoint &pos)
{
    float angle = m_pB2Body->GetAngle();
    m_pB2Body->SetTransform(b2Vec2(pos.x / m_fPTMRatio, pos.y / m_fPTMRatio), angle);
}

float Box2dPhysicsSprite::getRotation()
{
    return (m_bIgnoreBodyRotation ? CCSprite::getRotation() :
            CC_RADIANS_TO_DEGREES(m_pB2Body->GetAngle()));
}

void Box2dPhysicsSprite::setRotation(float fRotation)
{
    if (m_bIgnoreBodyRotation)
    {
        CCSprite::setRotation(fRotation);
    }
    else
    {
        b2Vec2 p = m_pB2Body->GetPosition();
        float radians = CC_DEGREES_TO_RADIANS(fRotation);
        m_pB2Body->SetTransform(p, radians);
    }
}

// returns the transform matrix according the Box2D Body values
CCAffineTransform Box2dPhysicsSprite::nodeToParentTransform()
{
    b2Vec2 pos  = m_pB2Body->GetPosition();
	
	float x = pos.x * m_fPTMRatio;
	float y = pos.y * m_fPTMRatio;
	
	if (m_bIgnoreAnchorPointForPosition)
    {
		x += m_obAnchorPointInPoints.x;
		y += m_obAnchorPointInPoints.y;
	}
	
	// Make matrix
	float radians = m_pB2Body->GetAngle();
	float c = cosf(radians);
	float s = sinf(radians);
	
	// Although scale is not used by physics engines, it is calculated just in case
	// the sprite is animated (scaled up/down) using actions.
	// For more info see: http://www.cocos2d-iphone.org/forum/topic/68990
	if (!m_obAnchorPointInPoints.equals(CCPointZero))
    {
		x += ((c * -m_obAnchorPointInPoints.x * m_fScaleX) + (-s * -m_obAnchorPointInPoints.y * m_fScaleY));
		y += ((s * -m_obAnchorPointInPoints.x * m_fScaleX) + (c * -m_obAnchorPointInPoints.y * m_fScaleY));
	}
    
	// Rot, Translate Matrix
	m_sTransform = CCAffineTransformMake( c * m_fScaleX,	s * m_fScaleX,
									     -s * m_fScaleY,	c * m_fScaleY,
									     x,	y );
	
	return m_sTransform;
}
