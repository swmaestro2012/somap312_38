#include "GameScene.h"

#include "SmartPad.h"

using namespace cocos2d;

SP_Gyro gyro;
float x=0,y=0,z=0;

CCScene* CSceneGame::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CSceneGame *layer = CSceneGame::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CSceneGame::init()
{
	CCLayer::init();
   
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCDirector::sharedDirector()->getAccelerometer()->setDelegate(this);

	CCLabelTTF *label = CCLabelTTF::create("1234","Arial",20);
	label->setColor(ccc3(0,0,0));
	label->setPosition(ccp(100,100));
	

	Bgi = CCSprite::create("bgi.png");
	Jump = CCSprite::create("jump.png");
	Shoot = CCSprite::create("shoot.png");
	JumpSel = CCSprite::create("jump_sel.png");
	ShootSel = CCSprite::create("shoot_sel.png");

	Bgi->setPosition(ccp(240,160));
	Jump->setAnchorPoint(ccp(0,0));
	Jump->setPosition(ccp(40,80));
	Shoot->setAnchorPoint(ccp(0,0));
	Shoot->setPosition(ccp(280,80));
	JumpSel->setAnchorPoint(ccp(0,0));
	JumpSel->setPosition(ccp(40,80));
	ShootSel->setAnchorPoint(ccp(0,0));
	ShootSel->setPosition(ccp(280,80));

	this->addChild(Bgi);
	this->addChild(Jump);
	this->addChild(Shoot);
	this->addChild(JumpSel);
	this->addChild(ShootSel);

	JumpSel->setVisible(false);
	ShootSel->setVisible(false);
	
	
	//this->addChild(label);


	setAccelerometerEnabled(true);
	setTouchEnabled(true);

	direction = 0;

	schedule(schedule_selector(CSceneGame::MoveCallback),0.1f);
	schedule(schedule_selector(CSceneGame::GyroCallback),0.1f);


	SP_Connect("192.168.1.163",9919);
	SP_SendPIN(1041);
	SP_SendVersion(SP_GetVersion());
    return true;
}

void CSceneGame::GyroCallback(float dt){
	SP_SendGyroInput(&gyro);
}
 
void CSceneGame::JumpCallback(CCObject* pSender){
	printf("do jump\n");

	SP_Touch touch;
	touch.x = 1;
	touch.y = 2;
	touch.state = SP_TOUCH_UP;
	SP_SendTouchInput(&touch);
	
}
void CSceneGame::ShootCallback(CCObject* pSender){
	printf("do shoot\n");
}


void CSceneGame::MoveLeft(){
	printf("MoveLeft\n");
}
void CSceneGame::MoveRight(){
	printf("MoveRight");
}

void CSceneGame::MoveCallback(float dt){
	if(direction > 0)
		MoveRight();
	else if(direction < 0)
		MoveLeft();
}

void CSceneGame::didAccelerate(CCAcceleration* pAccelerationValue){
	x = gyro.x = pAccelerationValue->x;
	y = gyro.y = pAccelerationValue->y;
	z = gyro.z = pAccelerationValue->z;
	

	if(pAccelerationValue->x >= 0.5f){
		direction = 1;
	}
	else if(pAccelerationValue->x <= -0.5f){
		direction = 1;
	}
	else direction = 0;
}

bool CSceneGame::ccTouchBegan(CCTouch* pTouches, CCEvent* pEvent) {
    CCTouch* pTouch = pTouches;
    const CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());

	printf("new touch %f %f\n", touchPoint.x,touchPoint.y);

	SP_Touch touch;
	touch.x = (int)touchPoint.x;
	touch.y = (int)touchPoint.y;
	touch.state = SP_TOUCH_DOWN;
	SP_SendTouchInput(&touch);

	//shoot
	if(touchPoint.x >= 240)
	{
		ShootSel->setVisible(true);
	}
	//jump
	else
		JumpSel->setVisible(true);

	return true;
}

void CSceneGame::ccTouchEnded(CCTouch* pTouches, CCEvent* pEvent) {
    CCTouch* pTouch = pTouches;
    const CCPoint touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->locationInView());

	SP_Touch touch;
	touch.x = (int)touchPoint.x;
	touch.y = (int)touchPoint.y;
	touch.state = SP_TOUCH_UP;
	SP_SendTouchInput(&touch);

	//shoot
	if(touchPoint.x >= 240)
	{
		ShootSel->setVisible(false);
	}
	//jump
	else
		JumpSel->setVisible(false);
}

void CSceneGame::ccTouchCancelled(CCTouch* pTouches, CCEvent* pEvent) {
}