#ifndef __CSceneGame_SCENE_H__
#define __CSceneGame_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class CSceneGame : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void JumpCallback(CCObject* pSender);
	void ShootCallback(CCObject* pSender);

	void MoveLeft();
	void MoveRight();

	void MoveCallback(float dt);
	void GyroCallback(float dt);

	bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	virtual void didAccelerate(CCAcceleration* pAccelerationValue);

    // implement the "static node()" method manually
    LAYER_CREATE_FUNC(CSceneGame);

private:
	int direction;

	CCSprite *Bgi;
	CCSprite *Jump;
	CCSprite *Shoot;
	CCSprite *JumpSel;
	CCSprite *ShootSel;
};

#endif  // __CSceneGame_SCENE_H__