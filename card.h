#include"cocos2d.h"
enum pukeType {
	Heart,
	Spade,
	Diamond,
	Club,
	Joker
};
class Card : public cocos2d::Sprite
{
public :
	Card(pukeType type, int num):puketype(type),number(num),select(false),moveState(false){}
	virtual bool init(pukeType type, int number);
	static Card* create(pukeType type, int number);
	static bool cmp(const Card*  lhs, const Card *rhs)
	{
		if (lhs->getNumber() > rhs->getNumber())
			return true;
		else
			return false;
	}
	int getNumber()const {
		return number;
	}
	pukeType getType()const
	{
		return puketype;
	}
	bool isSelect()
	{
		return select;
	}
	void setSelect(bool s)
	{
		select = s;
	}
	bool getMoveState()
	{
		return moveState;
	}
	void setMoveState(bool state)
	{
		moveState = state;
	}
private:
	pukeType puketype; //�˿˻�ɫ
	int number;//�˿�����
	bool select;//�Ƿ�ѡ��
	bool moveState;//�Ƿ�����ָѡ��״̬
};