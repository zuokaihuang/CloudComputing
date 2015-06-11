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
			
			for (auto bg : background){
				bg->release();
			}
			background.clear();
			for (auto p : projectile){
				p->release();
			}
			projectile.clear();
	}
    
public:
    void onBtnClick(cocos2d::CCObject* pSender);
	

	void update(float dt);
private:
	cocos2d::Sprite* _player;

	void addMonster(float dt);
	void addPipe(int index);
	void addPlayer();
	void initBg();

	std::vector<cocos2d::Sprite*> Monster;
	std::vector<cocos2d::Sprite*> projectile;
	std::vector<cocos2d::Sprite*> background;

	virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	bool onContactBegin(const cocos2d::PhysicsContact& contact);

	
	void JumpedCallBack();

	cocos2d::EventListenerTouchOneByOne* touch_listener;
	int i, j ,t;
	int pipe_count;
	bool firstTouch;
	STATE playerState;
};

#endif // __HELLOWORLD_SCENE_H__
