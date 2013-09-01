#ifndef __BOX2D_PHYSICS_SPRITE_H__
#define __BOX2D_PHYSICS_SPRITE_H__

#include "cocos2d.h"
#include "GravityBallConfig.h"

USING_NS_CC;

class b2Body;

class Box2dPhysicsSprite : public CCSprite
{
protected:
    bool    m_bIgnoreBodyRotation;

    b2Body  *m_pB2Body;
    // Pixels to Meters ratio
    float   m_fPTMRatio;

public:
    Box2dPhysicsSprite();

    static Box2dPhysicsSprite* create();
    /** Creates an sprite with a texture.
     The rect used will be the size of the texture.
     The offset will be (0,0).
     */
    static Box2dPhysicsSprite* createWithTexture(CCTexture2D *pTexture);

    /** Creates an sprite with a texture and a rect.
     The offset will be (0,0).
     */
    static Box2dPhysicsSprite* createWithTexture(CCTexture2D *pTexture, const CCRect& rect);

    /** Creates an sprite with an sprite frame. */
    static Box2dPhysicsSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);

    /** Creates an sprite with an sprite frame name.
     An CCSpriteFrame will be fetched from the CCSpriteFrameCache by name.
     If the CCSpriteFrame doesn't exist it will raise an exception.
     @since v0.9
     */
    static Box2dPhysicsSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);

    /** Creates an sprite with an image filename.
     The rect used will be the size of the image.
     The offset will be (0,0).
     */
    static Box2dPhysicsSprite* create(const char *pszFileName);

    /** Creates an sprite with an image filename and a rect.
     The offset will be (0,0).
     */
    static Box2dPhysicsSprite* create(const char *pszFileName, const CCRect& rect);

    virtual bool isDirty();
    
    /** Keep the sprite's rotation separate from the body. */
    bool isIgnoreBodyRotation() const;
    void setIgnoreBodyRotation(bool bIgnoreBodyRotation);
    
    virtual const CCPoint& getPosition();
    virtual void setPosition(const CCPoint &position);
    virtual float getRotation();
    virtual void setRotation(float fRotation);
    virtual CCAffineTransform nodeToParentTransform();
    
    /** Body accessor when using box2d */
    b2Body* getBody() const;
    void setBody(b2Body *pBody);
    
    float getPTMRatio() const;
    void setPTMRatio(float fPTMRatio);    
};

#endif // __BOX2D_PHYSICS_SPRITE_H__
