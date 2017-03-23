#include "card.h"
USING_NS_CC;
bool Card::init(pukeType type, int number)
{
	if (!Sprite::init())
		return false;
	initWithSpriteFrameName("HLDDZ_MainGame0_2x_0card_f.png");//�˿˱���ͼ��
	
	// ���뻨ɫ
	Size size = getContentSize();
	if (type < 4)
	{
		Sprite* spriteType = Sprite::createWithSpriteFrameName(StringUtils::format("HLDDZ_MainGame0_2x_%02d.png", type));
		spriteType->setAnchorPoint(Vec2(0, 1));
		spriteType->setScale(0.9);//������
		
		spriteType->setPosition(Vec2(8, size.height * 3 / 4));//������
		addChild(spriteType);
	}
	// ��������
	Sprite* spriteNumber = Sprite::createWithSpriteFrameName(StringUtils::format("HLDDZ_MainGame0_2x_0%d_%02d.png", type < 2 || number == 15 ? 1 : 0, number));//type < 2 ��ʾ�Ǻ�ɫ�˿ˣ� 15�Ǵ���
	spriteNumber->setAnchorPoint(Vec2(0, 1));
	spriteNumber->setScale(0.9);//������
	spriteNumber->setPosition(Vec2(3, size.height - 3));//������
	addChild(spriteNumber);
	//��С�˿�
	setScale(0.8);//������
	setContentSize(getContentSize() * 0.8);//������
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
