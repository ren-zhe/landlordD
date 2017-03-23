#ifndef __CLOCKCRTL_H__
#define __CLOCKCTRL_H__
#include"cocos2d.h"
class ClockCtrl : public cocos2d::Node
{
public:
	/*Clock(int time){
		sprite = cocos2d::Sprite::create("HLDDZ_MainGame0_2x_202.png");
		label = cocos2d::Label::create();
		label->setString(cocos2d::StringUtils::format("%2d",time));
	}*/
	ClockCtrl(){}
	~ClockCtrl(){}
	virtual bool init(int time);
	static ClockCtrl* create(int time);
	void down(float dt);
	bool isStop()
	{
		if (time == 0)
			return true;
		else
			return false;
	}
	int getTime()
	{
		return time;
	}
private:
	int time;
	cocos2d::Label* label;
	cocos2d::Sprite* sprite;
};
#endif
