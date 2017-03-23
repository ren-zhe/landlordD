#include"clock.h"
USING_NS_CC;
bool ClockCtrl::init(int time)
{
	if(!Node::init())
	   return false;
	sprite = Sprite::create("HLDDZ_MainGame0_2x_202.png");
	addChild(sprite);
	this->time = time;
	label = Label::create(cocos2d::StringUtils::format("%02d", time), "arial", 30);
	label->setColor(Color3B(0, 0, 0));
	label->setString(StringUtils::format("%02d", time));
	label->setAnchorPoint(Vec2(0.6, 0.7));//为什么锚点这样变化
	addChild(label);
	setScale(0.7);
	setContentSize(getContentSize() * 0.7);
	schedule(schedule_selector(ClockCtrl::down), 1);
	return true;
}

ClockCtrl * ClockCtrl::create(int time)
{
	ClockCtrl* pRet = new(std::nothrow)ClockCtrl();
	if (pRet && pRet->init(time))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void ClockCtrl::down(float dt)
{
	time--;
	
	label->setString(StringUtils::format("%02d", time));
	if (time == 0)
	{
		//时间到了
		unschedule(schedule_selector(ClockCtrl::down));
	}
}
