#include "HelloWorldScene.h"
#include <algorithm> 

#include "SimpleAudioEngine.h"
//#include "CocosDenshionTest.h"
using namespace CocosDenshion;


USING_NS_CC;

const std::map<std::string, cocos2d::Rect> HelloWorld::InputMap = HelloWorld::CreateInputMap();

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
	//auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setGravity(Vect(0,-900));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	i = j = 0;
	t = 0;
	first_plus = second_plus = m_answer = 0;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	pipe_count = 0;
	firstTouch = true;
	playerState = STATE::READY;
	m_score = 0;
	m_LastTime = m_CurrentTime = 0;

	inputimageX = visibleSize.width / 2;
	inputimageY = visibleSize.height / 4;

	srand((unsigned)time(nullptr));

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
										   CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,3);

    /////////////////////////////
    // 3. add your codes below...
	
	initBg();
    
	//add the score label;
	auto stringScoreLable = Label::createWithTTF("SCORE", "fonts/Marker Felt.ttf", fontSize);
	stringScoreLable->setPosition(visibleSize.width * 2 / 5 , visibleSize.height - 30);
	this->addChild(stringScoreLable, 2);

	auto stringLayerLable = Label::createWithTTF("L", "fonts/Marker Felt.ttf", fontSize);
	stringLayerLable->setPosition(stringScoreLable->getPositionX() - stringScoreLable->getContentSize().width,
		visibleSize.height - 30);
	this->addChild(stringLayerLable, 2);


	m_scoreLable = Label::createWithTTF("0", "fonts/Marker Felt.ttf", fontSize);
	m_scoreLable->setPosition(visibleSize.width * 2 / 5, visibleSize.height - 100);
	m_scoreLable->retain();
	this->addChild(m_scoreLable,2);

	//It will call updateCustom(dt) one per second
	//You can cancel this schedule with [unschedule(SEL_SCHEDULE selector, float delay)]
	//schedule(schedule_selector(HelloWorld::updateCustom), 1.0f, kRepeatForever, 0);

	addPlayer();
	//add the notify lable
	m_notifyLable = Label::createWithTTF("", "fonts/Marker Felt.ttf", fontSize);
	m_notifyLable->setPosition(_player->getPositionX() , _player->getPositionY()+100);
	m_notifyLable->retain();
	this->addChild(m_notifyLable, 10);



	int i = 0;
	while (i<=6)
		addPipe(i++);


	addInputImage();

	//this->schedule(schedule_selector(HelloWorld::addMonster), 1);


	touch_listener = EventListenerTouchOneByOne::create();

	touch_listener->setSwallowTouches(true);

	touch_listener->onTouchBegan = [this](Touch* touch, Event* event){
		
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		//AllocConsole();
		//freopen("CONIN$", "r", stdin);
		//freopen("CONOUT$", "w", stdout);
		//freopen("CONOUT$", "w", stderr);

		std::string key = getKey(locationInNode);
		//CCLOG("-[visibleSize w=>%f h=>%f]---x=>%f---y=>%f-----%s--------",visibleSize.width,visibleSize, locationInNode.x, locationInNode.y,key.c_str());
		


		onTouchBeganKey(touch, event, key);
		

		return true;
	
	};

	touch_listener->onTouchMoved = [](Touch* touch, Event* event){};
	touch_listener->onTouchEnded = [](Touch* touch, Event* event){};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, m_inputImage);

	//EventListenerPhysicsContact *contact_listener = EventListenerPhysicsContact::create();
	//contact_listener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(contact_listener, this);

	scheduleUpdate();

	b2BodyDef bodydef;
	bodydef.type = b2_dynamicBody;
	bodydef.position.Set(100,200);
	bodydef.angle = 0;


    return true;
}
void HelloWorld::update(float dt){

	//auto v = _player->getPhysicsBody()->getVelocity();

	//_player->setRotation(std::min(std::max(-90.0f, v.y * 0.2f + 60.0f), 30.0f));

}
bool HelloWorld::checkAnswer(int answer){

	bool ret = false;
	
	if (answer == (first_plus + second_plus))
	{
		m_CurrentTime = getCurrentTime();
		long time = getTimeOfCalculating();
		//AllocConsole();
		//freopen("CONIN$", "r", stdin);
		//freopen("CONOUT$", "w", stdout);
		//freopen("CONOUT$", "w", stderr);
		CCLOG("----CCCCCCCCCC==> %ld--LastScore=>%d------", time, m_score);
		marking(time);

		drawScore(m_score);
		return true;
	}

	return ret;
}
void HelloWorld::drawQuestion(int index, std::string question)
{
	auto p = projectile[index];
	p->removeChildByName(answer_name);
	auto label = Label::createWithTTF(question, "fonts/Marker Felt.ttf", fontSize);
	//label->setRotation(90);
	label->setColor(Color3B(255, 0, 0));

	label->setAnchorPoint(Vec2(0.5f, 0.5f));
	label->setPosition(Vec2(p->getContentSize().width / 2, p->getContentSize().height / 2));
	p->addChild(label, 1, answer_name);

}

void HelloWorld::drawAnwser(std::string answer){

	_player->removeChildByName(answer_name);
	auto label = Label::createWithTTF(answer, "fonts/Marker Felt.ttf", fontSize);
	label->setColor(Color3B(255, 255, 0));
	label->setAnchorPoint(Vec2(0.5f, 0.5f));
	label->setPosition(Vec2(_player->getContentSize().width / 2, _player->getContentSize().height / 2));
	_player->addChild(label, 1, answer_name);

}


bool HelloWorld::onTouchBeganKey(Touch* pTouch, Event* pEvent, std::string key)
{


	if (playerState == STATE::JUMPING)
	{
		firstTouch = false;
		return true;

	}else if (playerState == STATE::READY)
	{
		if (key != "ok" && key != "null"){

			if (key == "delete"){
				m_answer /= 10;
			}
			else{
				m_answer *= 10;
				m_answer += atoi(key.c_str());
			}
			
			char tempbuf[128]={0};
			sprintf(tempbuf,"%d",m_answer);
			drawAnwser(tempbuf);

		}
	}
	//
	//play the sound
	SimpleAudioEngine::getInstance()->setEffectsVolume(SimpleAudioEngine::getInstance()->getEffectsVolume() + 30.1f);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE,false);
	

	if (key != "ok"){

		if (checkAnswer(m_answer)){ // if answer is right , continue
			char tempbuf[128]={0};
			sprintf(tempbuf, "%d + %d = %d", first_plus, second_plus,m_answer);
			drawQuestion(1,tempbuf);
		}
		else
		{
			firstTouch = false;
			return false;
		}
	}


	//movingBg();

	playerJumping();

	if (firstTouch){

		movingPipe();
		firstTouch = false;
		
	}
	else
	{
		
		auto back = projectile.back();
		auto back_x = back->getPositionX();
		auto back_y = back->getPositionY();

		movingPipe();

		auto front = projectile.front();
		front->stopAllActions();
		front->setPosition(back_x, back_y);
		projectile.push_back(projectile.front());
		projectile.erase(projectile.begin());
	}


	return true;
}



void HelloWorld::onTouchMoved(Touch* pTouch, Event* pEvent)
{
}
void HelloWorld::onTouchEnded(Touch* pTouch, Event* pEvent)
{
}


void HelloWorld::addMonster(float dt){

	Size visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* MonsterExample = Sprite::create("monster-hd.png");
	j += 1;

	auto OneBody = PhysicsBody::createBox(MonsterExample->getContentSize());
	OneBody->setContactTestBitmask(0x08);

	MonsterExample->setPhysicsBody(OneBody);

	MonsterExample->setPosition(
		visibleSize.width + MonsterExample->getContentSize().width,
		MonsterExample->getContentSize().height / 2 + CCRANDOM_0_1()*(visibleSize.height - MonsterExample->getContentSize().height / 2));

	Monster.push_back(MonsterExample);
	this->addChild(Monster.back(),j);

	MoveTo* Move = MoveTo::create(8, Vec2(-Monster.back()->getContentSize().width / 2, Monster.back()->getPosition().y));

	Monster.back()->runAction(Move);

}


bool HelloWorld::onContactBegin(const PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	return true;
}

void HelloWorld::addPipe(int index)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto p = Sprite::create("Clound.png");
	//p->setScale(0.7);
	if (index<1){
		char buf[10] = { 0 };
		sprintf(buf, "%d + %d = ", first_plus, second_plus);
		auto label = Label::createWithTTF(buf, "fonts/Marker Felt.ttf", fontSize);
		//label->setRotation(90);
		label->setAnchorPoint(Vec2(0.5f, 0.5f));
		label->setPosition(Vec2(p->getContentSize().width / 2, p->getContentSize().height / 2));
		p->addChild(label,1,answer_name);
	}
	//auto OneBody = PhysicsBody::createBox(_p->getContentSize());
	//OneBody->setContactTestBitmask(0x04);
	//OneBody->setGravityEnable(false);
	//OneBody->setDynamic(false);
	auto w = p->getContentSize().width;
	auto h = p->getContentSize().height;
	p->setPosition(visibleSize.width * 2 / 5 + index*jumpLenght, visibleSize.height * 3 / 5 + index* jumpLenght);
	//_p->setPhysicsBody(OneBody);
	
	p->setScale(visibleSize.width / scarelXbase, visibleSize.height / scarelYbase);
	projectile.push_back(p);
	p->retain();

	this->addChild(projectile.back(), 1);
	pipe_count = projectile.size();
	
}

void HelloWorld::addPlayer()
{
	//hzk add 
	Size visibleSize = Director::getInstance()->getVisibleSize();

	_player = Sprite::create("player-hd.png");
	_player->setPosition(visibleSize.width * 2 / 5, visibleSize.height * 3 / 5 + _player->getContentSize().height / 2 + 50);

	//auto OneBody = PhysicsBody::createBox(_player->getContentSize());
	//OneBody->setContactTestBitmask(0x04);

	//_player->setPhysicsBody(OneBody);
	this->addChild(_player,3);
	_player->retain();
}


void HelloWorld::initBg()
{
	// add "HelloWorld" splash screen"
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg_first = Sprite::create("ZX.jpg");
	bg_first->retain();
	bg_first->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 /*4*/ + origin.y));
	if (visibleSize.width >= bg_first->getContentSize().width)
	{
		bg_first->setScaleX(visibleSize.width / bg_first->getContentSize().width);
	}
	if (visibleSize.height >= bg_first->getContentSize().height)
	{
		bg_first->setScaleY(visibleSize.height / bg_first->getContentSize().height);
	}

	background.push_back(bg_first);
	
	this->addChild(background.back(), -1);
	/*
	auto bg_second = Sprite::create("ZX.jpg");
	bg_second->retain();
	//bg_second->setPosition(Vec2(bg_first->getPositionX() + bg_first->getContentSize().width / 2 + bg_second->getContentSize().width/2,
	//	visibleSize.height / 2 + origin.y));
	bg_second->setPosition(Vec2(visibleSize.width / 2 + origin.x,
		bg_first->getPositionY() + bg_first->getContentSize().height / 2 + bg_second->getContentSize().height / 2));
	background.push_back(bg_second);
	this->addChild(background.back(), -1);

	auto bg_third = Sprite::create("ZX.jpg");
	bg_third->retain();
	//bg_third->setPosition(Vec2(bg_second->getPositionX() + bg_second->getContentSize().width / 2 + bg_third->getContentSize().width / 2,
	//	visibleSize.height / 2 + origin.y));
	bg_third->setPosition(Vec2(visibleSize.width / 2 + origin.x,
		bg_second->getPositionY() + bg_second->getContentSize().height / 2 + bg_third->getContentSize().height / 2));
	background.push_back(bg_third);
	this->addChild(background.back(), -1);
	*/
}

void HelloWorld::addInputImage(){

	m_inputImage = Sprite::create("Input-md.png");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
		//m_inputImage->setScale(0.5f);
#endif

	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_inputImage->setScale(visibleSize.width / scarelXbase, visibleSize.height / scarelYbase);
	// add a "close" icon to exit the progress. it's an autorelease object
	//auto inputImage = MenuItemImage::create(
	//	"Input.png",
	//	"Input.png",
	//	CC_CALLBACK_1(HelloWorld::menuInputCallback, this));

	m_inputImage->setPosition(inputimageX, inputimageY);

	// create menu, it's an autorelease object
	//m_inputImage = Menu::create(inputImage, NULL);
	m_inputImage->retain();
	this->addChild(m_inputImage,2);

}

void HelloWorld::playerJumping()
{
	playerState = STATE::JUMPING;

	auto x = _player->getPositionX();
	auto y = _player->getPositionY();
	auto moveUp = MoveTo::create(0.2f, Vec2(x, y + 60));
	auto moveDown = MoveTo::create(0.5f, Vec2(x, y));

	auto callback = CallFunc::create([this](){
			this->JumpedCallBack();
	});

	auto action = Sequence::create(moveUp->clone(), moveDown->clone(), callback, NULL);

	_player->runAction(action->clone());
	
}

void HelloWorld::movingBg()
{
	auto bg = background.front();
	//if ((bg->getPositionX()< -0.0f) && (bg->getPositionX() + bg->getContentSize().width / 2)<0.0f){
	if ((bg->getPositionY()< -0.0f) && (bg->getPositionY() + bg->getContentSize().height / 2)<0.0f){
		auto front = background.front();
		auto back = background.back();
		//front->setPositionX(back->getPositionX() + back->getContentSize().width / 2 + front->getContentSize().width / 2);
		front->setPositionY(back->getPositionY() + back->getContentSize().height / 2 + front->getContentSize().height / 2);
		background.push_back(front);
		background.erase(background.begin());
	}

	for (auto bg : background){
		auto move = MoveTo::create(0.5f, Vec2(bg->getPositionX(), bg->getPositionY() - (jumpLenght-50)));
		bg->runAction(move);
	}
}

void HelloWorld::movingPipe()
{
	projectile.front()->removeChildByName(answer_name);

	for (auto p : projectile){
		auto x = p->getPositionX();
		auto y = p->getPositionY();
		auto move = MoveTo::create(0.7f, Vec2(x - (jumpLenght), y - (jumpLenght)));
		p->runAction(move);
	}
	
}








void HelloWorld::menuInputCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)

#endif
	//add the logic code
	


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


void HelloWorld::JumpedCallBack()
{
	
	playerState = STATE::READY;

	_player->removeChildByName(answer_name);
	char buf[128]={0};
	sprintf(buf, "%d + %d = %d", first_plus, second_plus,m_answer);
	drawQuestion(0, buf);

	m_answer = 0;

	first_plus = getRandomNumber(1,100);
	second_plus = getRandomNumber(1, 100);

	char buf1[128]={0};
	sprintf(buf1,"%d + %d =",first_plus,second_plus);
	drawQuestion(1, buf1);

	m_LastTime = getCurrentTime();

}
int HelloWorld::getRandomNumber(int start, int end)
{
	auto ret = 0;

	ret = CCRANDOM_0_1()*(end - start + 1) + start;

	return ret;
}
std::string HelloWorld::getKey(Point point)
{
	std::string ret("null");
	for (auto map : InputMap){

		if (map.second.containsPoint(point)){

			return map.first;
		}
	}

	return ret;
}

void HelloWorld::drawScore(int score)
{
	char buf[128]={0};
	sprintf(buf,"%d",score);
	m_scoreLable->setString(buf);
}

void HelloWorld::marking(long time)
{
	if (time > 1000)
	{
		return;
	}
	int scoreBase = 24;
	if (time > 0 && time <=10)
	{
		scoreBase = (scoreBase / time);
		drawNiceOrBadNotify("Nice",scoreBase);

	}
	else if ( time > 10 && time <= 20)
	{
		scoreBase = 1;
		drawNiceOrBadNotify("Come on", scoreBase);
	}
	else if ( time > 20 && time <= 30)
	{
		scoreBase = 0;
		drawNiceOrBadNotify("Oh No~~!", scoreBase);
	}else
	{
		scoreBase = -5;
		drawNiceOrBadNotify("Oh No~~   -5 !", scoreBase);
	}

	m_score += scoreBase;
}

void HelloWorld::removeNotifyCallback(Node* pSender)
{
	//this->removeChild(pSender);
}

void HelloWorld::drawNiceOrBadNotify(std::string notify,int score)
{
	char buf[32] = { 0 };
	if (score >= 0)
		sprintf(buf,"%s   +%d",notify.c_str(),score);
	else
		sprintf(buf, "%s   %d", notify.c_str(), score);

	m_notifyLable->setString(std::string(buf));
	auto show = Show::create();
	auto fade_out = FadeOut::create(1.0f);
	auto fade_in = FadeIn::create(0.1f);
	auto hide = Hide::create();

	auto seq = Sequence::create(show, fade_out, hide, fade_in, nullptr);
	auto fade_one = Repeat::create(seq, 1);
	//FiniteTimeAction * callFuncN = CallFuncN::create(notifyLable,callfuncN_selector(HelloWorld::removeNotifyCallback));

	m_notifyLable->runAction(fade_one);
}


long HelloWorld::getCurrentTime()
{
	long time = 0;
	struct  timeval tv;
	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return time;
}
long HelloWorld::getTimeOfCalculating()
{
	long time;
	time = (m_CurrentTime - m_LastTime)/1000;
	return time;
}


