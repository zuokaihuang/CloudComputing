#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#include <stdio.h>
#include <stdlib.h>//srand()��rand()
#include <time.h>//time();
//#include <conio.h>

//asdfasd
enum class STATE
{
	READY,
	JUMPING
};

static float avgW = 270.0f;
static float avgH = 200.0f;
static float jumpLenght = 200.0f;
static float fontSize = 50;
static const char* ok = "ok";
static const std::string answer_name = "lastanswer";

static float scarelXbase = 1080;
static float scarelYbase = 1920;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#define MUSIC_FILE  "res/jumping.ogg"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#define MUSIC_FILE  "res/jumping.wav"
#endif
 
class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	// a selector input callback
	void menuInputCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	~HelloWorld(){ _player->release();
				m_inputImage->release();
				m_scoreLable->release();
				m_notifyLable->release();

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


	cocos2d::Sprite* m_inputImage;

	void addMonster(float dt);
	void addPipe(int index);
	void addPlayer();
	void initBg();
	void addInputImage();

	inline void playerJumping();
	inline void movingBg();
	inline void movingPipe();
	inline void drawQuestion(int index, std::string question);
	inline void drawAnwser(std::string answer);

	inline int getRandomNumber(int start,int end);


	std::string getKey(cocos2d::Point point);


	std::vector<cocos2d::Sprite*> Monster;
	std::vector<cocos2d::Sprite*> projectile;
	std::vector<cocos2d::Sprite*> background;

	bool onTouchBeganKey(cocos2d::Touch* pTouch, cocos2d::Event* pEvent,std::string key);
	virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	bool onContactBegin(const cocos2d::PhysicsContact& contact);

	
	void JumpedCallBack();

	cocos2d::EventListenerTouchOneByOne* touch_listener;
	int i, j ,t;
	int pipe_count;
	bool firstTouch;
	STATE playerState;
	float inputimageX, inputimageY;

	int					m_score;
	cocos2d::Label*		m_scoreLable;
	cocos2d::Label*		m_notifyLable;

	void					drawScore(int score);
	void					marking(long time);
	void					drawNiceOrBadNotify(std::string notify,int score);
	void					removeNotifyCallback(Node* pSender);

	//�Զ��������
	//void					updateCustom(float dt);

	long					m_LastTime;
	long					m_CurrentTime;
	long					getCurrentTime();
	long					getTimeOfCalculating();


	int first_plus, second_plus, m_answer;
	bool checkAnswer(int answer);

	static std::map<std::string,cocos2d::Rect> CreateInputMap()
	{
		std::map<std::string, cocos2d::Rect> map;
		map[std::string("delete")] = cocos2d::Rect(avgW*0,avgH*0, avgW, avgH);
		map[std::string("0")] = cocos2d::Rect(avgW * 1, avgH * 0, avgW, avgH);
		map[std::string("ok")] = cocos2d::Rect(avgW * 2, avgH * 0, avgW, avgH);
		map[std::string("1")] = cocos2d::Rect(avgW * 0, avgH * 1, avgW, avgH);
		map[std::string("2")] = cocos2d::Rect(avgW * 1, avgH * 1, avgW, avgH);
		map[std::string("3")] = cocos2d::Rect(avgW * 2, avgH * 1, avgW, avgH);
		map[std::string("4")] = cocos2d::Rect(avgW * 0, avgH * 2, avgW, avgH);
		map[std::string("5")] = cocos2d::Rect(avgW * 1, avgH * 2, avgW, avgH);
		map[std::string("6")] = cocos2d::Rect(avgW * 2, avgH * 2, avgW, avgH);
		map[std::string("7")] = cocos2d::Rect(avgW * 0, avgH * 3, avgW, avgH);
		map[std::string("8")] = cocos2d::Rect(avgW * 1, avgH * 3, avgW, avgH);
		map[std::string("9")] = cocos2d::Rect(avgW * 2, avgH * 3, avgW, avgH);
		return map;
	}
	static const std::map<std::string, cocos2d::Rect> InputMap;
	
	

};



#endif // __HELLOWORLD_SCENE_H__
