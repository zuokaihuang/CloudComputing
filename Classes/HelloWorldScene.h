#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
enum class STATE
{
	READY,
	JUMPING
};
class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	~HelloWorld(){ _player->release();
	_p->release();
	}
    
public:
    void onBtnClick(cocos2d::CCObject* pSender);
	

	void update(float dt);
private:
	cocos2d::Sprite* _player;
	cocos2d::Sprite* _p;
	void addMonster(float dt);
	void addPipe(int index);

	std::vector<cocos2d::Sprite*> Monster;
	std::vector<cocos2d::Sprite*> projectile;
	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	bool onContactBegin(const cocos2d::PhysicsContact& contact);

	
	void JumpedCallBack();

	cocos2d::EventListenerTouchOneByOne* touch_listener;
	int i, j ,t;
	int pipe_count;
};

#endif // __HELLOWORLD_SCENE_H__
