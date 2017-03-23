#include "card.h"
USING_NS_CC;
bool Card::init(pukeType type, int number)
{
	if (!Sprite::init())
		return false;
	initWithSpriteFrameName("HLDDZ_MainGame0_2x_0card_f.png");//扑克背景图案
	
	// 加入花色
	Size size = getContentSize();
	if (type < 4)
	{
		Sprite* spriteType = Sprite::createWithSpriteFrameName(StringUtils::format("HLDDZ_MainGame0_2x_%02d.png", type));
		spriteType->setAnchorPoint(Vec2(0, 1));
		spriteType->setScale(0.9);//待处理
		
		spriteType->setPosition(Vec2(8, size.height * 3 / 4));//待处理
		addChild(spriteType);
	}
	// 加入数字
	Sprite* spriteNumber = Sprite::createWithSpriteFrameName(StringUtils::format("HLDDZ_MainGame0_2x_0%d_%02d.png", type < 2 || number == 15 ? 1 : 0, number));//type < 2 表示是红色扑克， 15是大王
	spriteNumber->setAnchorPoint(Vec2(0, 1));
	spriteNumber->setScale(0.9);//待处理
	spriteNumber->setPosition(Vec2(3, size.height - 3));//待处理
	addChild(spriteNumber);
	//缩小扑克
	setScale(0.8);//待处理
	setContentSize(getContentSize() * 0.8);//待处理
	return true;
}

Card * Card::create(pukeType type, int number)
{
	Card* card = new (std::nothrow) Card(type, number);
	if (card && card->init(type, number))
	{
		card->autorelease();
		return card;
	}
	else
	{
		delete card;
		card = nullptr;
		return nullptr;
	}
}
