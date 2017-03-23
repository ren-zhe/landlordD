#include "gameScene.h"
USING_NS_CC;

bool GameScene::init()
{
	srand((unsigned int)time(NULL));
	if (!Layer::init())
		return false;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("puke.plist", "puke.png");
	Size size = Director::getInstance()->getVisibleSize();
	Sprite* sprite = Sprite::create("Common_BG2_2x.png");
	sprite->setPosition(size.width / 2, size.height / 2);
	addChild(sprite);
	//初始化数据
	startx = 40;
	circleNum = 0;
	callLandlord = none;
	grabLandlord = none;
	doneLandlord = false;
	//先指定
	oriLandlord  = static_cast<Landlord>(rand()%3);
	createCards();
	shuffleCards();
	discardCards();
	sortCards();
	displayCards();
	clock = nullptr;
	//displayLandlordCards();
	//显示其他玩家牌数
	Sprite* cardsNum = Sprite::create("HLDDZ_MainGame0_2x_0card_b.png");
	cardsNum->setPosition(Director::getInstance()->getVisibleSize().width * 6 / 7, Director::getInstance()->getVisibleSize().height * 2 / 3);
	addChild(cardsNum);
	label1 = Label::createWithSystemFont(StringUtils::format("%d", player2Cards.size()), "arial", 50);
	label1->setPosition(cardsNum->getContentSize().width / 2, cardsNum->getContentSize().height / 2);
	label1->setColor(Color3B(0, 0, 0));
	cardsNum->addChild(label1);
	cardsNum->setScale(0.4);
	cardsNum->setContentSize(cardsNum->getContentSize() * 0.4);

	Sprite* cardsNum1 = Sprite::create("HLDDZ_MainGame0_2x_0card_b.png");
	cardsNum1->setPosition(Director::getInstance()->getVisibleSize().width / 7, Director::getInstance()->getVisibleSize().height * 2 / 3);
	addChild(cardsNum1);
	label2 = Label::createWithSystemFont(StringUtils::format("%d", player3Cards.size()), "arial", 50);
	label2->setPosition(cardsNum1->getContentSize().width / 2, cardsNum1->getContentSize().height / 2);
	label2->setColor(Color3B(0, 0, 0));
	cardsNum1->setScale(0.4);
	cardsNum1->setContentSize(cardsNum1->getContentSize() * 0.4);
	cardsNum1->addChild(label2);

	//显示地主牌
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = std::bind(&GameScene::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchMoved = std::bind(&GameScene::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	listener->onTouchEnded = std::bind(&GameScene::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	turn = static_cast<Turn>(oriLandlord);
	schedule(schedule_selector(GameScene::AIplayer2), 1.0, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::AIplayer3), 1.0, kRepeatForever, 1);
	schedule(schedule_selector(GameScene::myTurn), 0.5, kRepeatForever, 0);
	//addChild(menu);
	return true;
}

Scene * GameScene::createScene()
{
	Scene* scene = Scene::create();
	GameScene* layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}
void GameScene::createCards()
{
	int type = Heart;//创建扑克，按照红桃，方片，黑桃，梅花，小王，大王，顺序创建
	while (type < 4)
	{
		for (int i = 1; i < 14; i++)//从3至2
		{
			allCards.pushBack(Card::create(static_cast<pukeType>(type), i));//用allCards存储所有扑克
			//addChild(allCards.at(type * 13 + i - 1));
		}
		type = type + 1;
	}
	allCards.pushBack(Card::create(Joker, 14));
	allCards.pushBack(Card::create(Joker, 15));
	//	addChild(allCards.at(52));
		//addChild(allCards.at(53));
		//for (int i = 0; i < allCards.size(); ++i)
		//{
		//	//allCards.at(i)->ignoreAnchorPointForPosition(true);
		//}
}

void GameScene::shuffleCards() //洗牌
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < allCards.size(); i++)
	{

		int t1 = rand() % allCards.size();
		int t2 = rand() % allCards.size();
		allCards.swap(allCards.at(t1), allCards.at(t2));
	}
	for (int i = 0; i < allCards.size(); i++)
	{

		int t1 = rand() % allCards.size();
		int t2 = rand() % allCards.size();
		allCards.swap(allCards.at(t1), allCards.at(t2));
	}
}

void GameScene::discardCards()//分牌
{
	for (int i = 0; i < 17; i++)
	{
		player1Cards.pushBack(allCards.at(i));
		//removeChild(allCards.at(i));
		//addChild(allCards.at(i));
	}
	for (int i = 17; i < 34; i++)
	{
		player2Cards.pushBack(allCards.at(i));
		//removeChild(allCards.at(i));
		//addChild(allCards.at(i));
	}
	for (int i = 34; i < 51; i++)
	{
		player3Cards.pushBack(allCards.at(i));
		//removeChild(allCards.at(i));
		//addChild(allCards.at(i));
	}
	for (int i = 51; i < 54; i++)
	{
		landlordCards.pushBack(allCards.at(i));
		//removeChild(allCards.at(i));
		//addChild(allCards.at(i));
	}
}

void GameScene::sortCards()
{
	std::sort(player1Cards.begin(), player1Cards.end(), Card::cmp);
	std::sort(player2Cards.begin(), player2Cards.end(), Card::cmp);
	std::sort(player3Cards.begin(), player3Cards.end(), Card::cmp);
}

void GameScene::displayCards()
{
	//Size size = Director::getInstance()->getVisibleSize();
	//Point origin = Director::getInstance()->getVisibleOrigin();
	for (int i = 0; i < player1Cards.size(); i++)
	{
		player1Cards.at(i)->setAnchorPoint(Vec2(0, 0));
		player1Cards.at(i)->setPosition(startx + INTERVAL * i, STARTY);
		addChild(player1Cards.at(i));
	}
	for (int i = 0; i < player2Cards.size(); i++)//也许不应该将所有牌全部显示出来
	{
		player2Cards.at(i)->setScale(0.5);//进行缩放到适合大小
		player2Cards.at(i)->setAnchorPoint(Vec2(0, 0));
		player2Cards.at(i)->setContentSize(player2Cards.at(i)->getContentSize() * 0.5);
		//player2Cards.at(i)->setPosition(-400, 0);//移动到不可见区域
		//addChild(player2Cards.at(i));	
	}
	for (int i = 0; i < player3Cards.size(); i++)
	{
		player3Cards.at(i)->setScale(0.5);
		player3Cards.at(i)->setAnchorPoint(Vec2(0, 0));
		player3Cards.at(i)->setContentSize(player3Cards.at(i)->getContentSize() * 0.5);
		//addChild(player3Cards.at(i));
		//player3Cards.at(i)->setPosition(-400, 0);
	}
	for (int i = 0; i < landlordCards.size(); i++)
	{
		landlordCards.at(i)->setAnchorPoint(Vec2(0, 0));
	}
}

bool GameScene::onTouchBegan(Touch * touch, Event * event)
{
	Vec2 pot = touch->getLocation();
	Size size = player1Cards.at(0)->getContentSize();
	Rect range = { startx ,STARTY,INTERVAL * player1Cards.size() + size.width - INTERVAL, size.height + MOVEHEIGHT };
	if (range.containsPoint(pot))
	{
		int num;
		if (pot.x < startx + INTERVAL * player1Cards.size()) //判断点击所属扑克大致位置
		{
			num = (pot.x - startx) / INTERVAL;
		}
		else
		{
			num = player1Cards.size() - 1;
		}
		if (pot.y > STARTY + size.height && pot.y < STARTY + size.height + MOVEHEIGHT)//点中顶部区域
		{
			int i;
			for (i = num; i >= 0; i--)//向左找出第一个被选中的扑克
			{
				if (player1Cards.at(i)->isSelect())
				{
					num = i;
					break;
				}
			}
			if (i >= 0)  //找到
			{
				Rect smallRange = { (float)i * INTERVAL + startx, STARTY + size.height, size.width, MOVEHEIGHT };
				if (smallRange.containsPoint(pot))
				{
					startTouchNum = num;
					player1Cards.at(startTouchNum)->setMoveState(true);
					/*	player1Cards[startTouchNum]->setColor(Color3B(200, 200, 200));*/
					return true;
				}
			}
		}
		else if (pot.y > STARTY && pot.y < STARTY + MOVEHEIGHT) // 点中底部
		{
			int i;
			for (i = num; i >= 0; i--)//让num标识向左第一个未被选中的扑克
			{
				if (!player1Cards.at(i)->isSelect())
				{
					num = i;
					break;
				}
			}
			if (i >= 0)//找到
			{
				Rect bottomRange = { (float)i * INTERVAL + startx, STARTY, size.width, size.height };
				if (bottomRange.containsPoint(pot))
				{
					startTouchNum = num;
					player1Cards.at(startTouchNum)->setMoveState(true);
					/*player1Cards[startTouchNum]->setColor(Color3B(200, 200, 200));*/
					return true;
				}
			}
		}
		else//点击中间部分
		{
			startTouchNum = num;
			player1Cards.at(startTouchNum)->setMoveState(true);
			/*	player1Cards[startTouchNum]->setColor(Color3B(200, 200, 200));*/
			return true;
		}
	}
	return false;
}

void GameScene::onTouchMoved(Touch * touch, Event * event)
{
	Vec2 point = touch->getLocation();
	int num;
	if (point.x < startx + INTERVAL * player1Cards.size())
	{
		num = (point.x - startx) / INTERVAL;
	}
	else
	{
		num = player1Cards.size() - 1;
	}
	Size size = player1Cards.at(0)->getContentSize();
	if (point.y > STARTY && point.y < STARTY + MOVEHEIGHT)//点中底部区域
	{
		int i;//向左找到第一个未被选中的扑克
		for (i = num; i >= 0; i--)
		{
			if (!player1Cards.at(i)->isSelect())
			{
				num = i;
				break;
			}
		}
		if (i >= 0)//找到
		{
			Rect bottomRange = { (float)i * INTERVAL + startx, STARTY, size.width, MOVEHEIGHT };
			if (!bottomRange.containsPoint(point))//鼠标点击的位置有扑克
			{
				return;
			}
		}
		else
			return;
	}
	else if (point.y > STARTY + size.height && point.y < STARTY + size.height + MOVEHEIGHT)//点中顶部区域
	{
		int i;
		for (i = num; i >= 0; i--)
		{
			if (player1Cards.at(i)->isSelect())
			{
				num = i;
				break;
			}
		}
		if (i >= 0) //找到
		{
			Rect topRange = { (float)i * INTERVAL + startx, STARTY + size.height, size.width, MOVEHEIGHT };
			if (!topRange.containsPoint(point))// 鼠标点击的位置有扑克
			{
				return;
			}
		}
		else
			return;
	}
	else if (point.y < STARTY || point.y > STARTY + MOVEHEIGHT + size.height)
		return;
	if (num < startTouchNum)
	{
		for (int i = 0; i < player1Cards.size(); i++)
		{
			if (i >= num && i <= startTouchNum)
			{
				player1Cards.at(i)->setMoveState(true);
				player1Cards.at(i)->setColor(Color3B(200, 200, 200));
			}
			else
			{
				player1Cards.at(i)->setMoveState(false);
				player1Cards.at(i)->setColor(Color3B(255, 255, 255));
			}
		}
	}
	else
	{
		for (int i = 0; i < player1Cards.size(); i++)
		{
			if (i >= startTouchNum && i <= num)
			{
				player1Cards.at(i)->setMoveState(true);
				player1Cards.at(i)->setColor(Color3B(200, 200, 200));
			}
			else
			{
				player1Cards.at(i)->setMoveState(false);
				player1Cards.at(i)->setColor(Color3B(255, 255, 255));
			}
		}
	}
	//static int lastNum = -1;
	//Vec2 pot = touch->getLocation();
	//Size size = player1Cards[0]->getContentSize();
	//Rect range = { 40 + (17 - (float)player1Cards.size()) * MOVEHEIGHT / 2 ,40,MOVEHEIGHT * player1Cards.size() + size.width - MOVEHEIGHT, size.height + 30};
	//if (range.containsPoint(pot))
	//{
	//	int num; //计算点击所属扑克位置
	//	if (pot.x < 40 + MOVEHEIGHT * 17)
	//	{
	//		num = (pot.x - 40) / MOVEHEIGHT;
	//	}
	//	else
	//	{
	//		num = 16;
	//	}
	//	if (pot.y > 40 && pot.y < 70)//点中底部区域
	//	{
	//		int i;//向左找到第一个未被选中的扑克
	//		for (i = num; i >= 0; i--)
	//		{
	//			if (!player1Cards.at(i)->isSelect())
	//			{
	//				num = i;
	//				break;
	//			}
	//		}
	//		if (i >= 0)//找到
	//		{
	//			Rect bottomRange = { (float)i * MOVEHEIGHT + 40, 40, size.width, 30 };
	//			if (bottomRange.containsPoint(pot))//鼠标点击的位置有扑克
	//			{
	//				if (num == lastNum)
	//					return;
	//				if (!player1Cards[num]->getMoveState())
	//				{
	//					player1Cards[num]->setMoveState(true);
	//					player1Cards[num]->setColor(Color3B(255, 0, 0));
	//				}
	//				else
	//				{
	//					if (player1Cards[lastNum]->getMoveState())
	//					{
	//						player1Cards[lastNum]->setMoveState(false);
	//						player1Cards[lastNum]->setColor(Color3B(255, 255, 255));
	//					}
	//					/*player1Cards[num]->setMoveState(false);
	//					player1Cards[num]->setColor(Color3B(0, 0, 0));*/
	//				}
	//			}
	//		}
	//	}
	//	else if (pot.y > 40 + size.height && pot.y < 40 + size.height + 30)//点中顶部区域
	//	{
	//		int i;
	//		for ( i = num; i >= 0; i--)
	//		{
	//			if (player1Cards.at(i)->isSelect())
	//			{
	//				num = i;
	//				break;
	//			}
	//		}
	//		if (i >= 0) //找到
	//		{
	//			Rect topRange = { (float)i * MOVEHEIGHT + 40, 40 + size.height, size.width, 30 };
	//			if (topRange.containsPoint(pot))// 鼠标点击的位置有扑克
	//			{
	//				if (num == lastNum)
	//					return;
	//				if (!player1Cards[num]->getMoveState())
	//				{
	//					player1Cards[num]->setMoveState(true);
	//					player1Cards[num]->setColor(Color3B(255, 0, 0));
	//				}
	//				else
	//				{
	//					if (player1Cards[lastNum]->getMoveState())
	//					{
	//						player1Cards[lastNum]->setMoveState(false);
	//						player1Cards[lastNum]->setColor(Color3B(255, 255, 255));
	//					}
	//					/*player1Cards[num]->setMoveState(false);
	//					player1Cards[num]->setColor(Color3B(0, 0, 0));*/
	//				}
	//			}
	//		}
	//	}
	//	else//判断中间区域
	//	{
	//		/*if (bottomRange.containsPoint(touch->getPreviousLocation()))
	//			return;*/
	//		if (num == lastNum)
	//			return;
	//		if (!player1Cards[num]->getMoveState())
	//		{
	//			player1Cards[num]->setMoveState(true);
	//			player1Cards[num]->setColor(Color3B(255, 0, 0));
	//		}
	//		else
	//		{
	//			if (player1Cards[lastNum]->getMoveState())
	//			{
	//				player1Cards[lastNum]->setMoveState(false);
	//				player1Cards[lastNum]->setColor(Color3B(255, 255, 255));
	//			}
	//		/*	player1Cards[num]->setMoveState(false);
	//			player1Cards[num]->setColor(Color3B(255, 255, 255));*/
	//		}
	//	}
	//	lastNum = num;
	//}
}

void GameScene::onTouchEnded(Touch * touch, Event * event)
{
	Vec2 point = touch->getLocation();
	for (int i = 0; i < player1Cards.size(); i++)
	{
		if (player1Cards.at(i)->getMoveState())
		{
			Action* move;
			if (player1Cards.at(i)->isSelect())
			{
				move = MoveBy::create(0.1, Vec2(0, -MOVEHEIGHT));
				player1Cards.at(i)->setSelect(false);

			}
			else
			{
				move = MoveBy::create(0.1, Vec2(0, MOVEHEIGHT));
				player1Cards.at(i)->setSelect(true);

			}
			player1Cards.at(i)->setMoveState(false);
			player1Cards.at(i)->setColor(Color3B(255, 255, 255));
			player1Cards.at(i)->runAction(move);
		}
	}
	//Vec2 pot = touch->getLocation();
	//Size size = player1Cards[0]->getContentSize();
	//Rect range = { 40 + (17 - (float)player1Cards.size()) * MOVEHEIGHT / 2 ,40,MOVEHEIGHT * player1Cards.size() + size.width - MOVEHEIGHT, size.height + 30};
	//if (range.containsPoint(pot))
	//{
	//	int num;
	//	if (pot.x < 40 + MOVEHEIGHT * 17)
	//	{
	//		num = (pot.x - 40) / MOVEHEIGHT;
	//	}
	//	else
	//	{
	//		num = 16;
	//	}
	//	if (pot.y > 40 + size.height && pot.y < 40 + size.height + 30)//点中顶部区域
	//	{
	//		int i;
	//		for (i = num; i >= 0; i--)//向左找出第一个被选中的扑克
	//		{
	//			if (player1Cards.at(i)->isSelect())
	//			{
	//				num = i;
	//				break;
	//			}
	//		}
	//		if (i >= 0)  //找到
	//		{
	//			Rect smallRange = { (float)i * MOVEHEIGHT + 40, 40 + size.height, size.width, 30 };
	//			if (smallRange.containsPoint(pot))
	//			{
	//				Action* move = MoveBy::create(0.1, Vec3(0, -30, 0));
	//				player1Cards[num]->setSelect(false);
	//				player1Cards[num]->runAction(move);
	//			}
	//		}
	//	}
	//	else if (pot.y > 40 && pot.y < 40 + 30) // 点中底部
	//	{
	//		int i;
	//		for (i = num; i >= 0; i--)//让num标识向左第一个未被选中的扑克
	//		{
	//			if (!player1Cards.at(i)->isSelect())
	//			{
	//				num = i;
	//				break;
	//			}
	//		}
	//		if (i >= 0)//找到
	//		{
	//			Rect bottomRange = { (float)i * MOVEHEIGHT + 40, 40, size.width, size.height };
	//			if (bottomRange.containsPoint(pot))
	//			{
	//				Action* move = MoveBy::create(0.1, Vec3(0, 30, 0));
	//				player1Cards[num]->setSelect(true);
	//				player1Cards[num]->runAction(move);
	//			}
	//		}
	//	}
	//	else//点击中间部分
	//	{
	//		if (!player1Cards[num]->isSelect())
	//		{
	//			//selectCards.pushBack(player1Cards[num]);
	//			Action* move = MoveBy::create(0.1, Vec3(0, 30, 0));
	//			player1Cards[num]->runAction(move);
	//			player1Cards[num]->setSelect(true);
	//		}
	//		else
	//		{
	//			//selectCards.erase(player1Cards.begin() + num);
	//			Action* move = MoveBy::create(0.1, Vec3(0, -30, 0));
	//			player1Cards[num]->setSelect(false);
	//			player1Cards[num]->runAction(move);
	//		}	
	//	}
	//}
		/*player1Cards.erase(player1Cards.begin() + num);*/
}

bool GameScene::isSingleCard(cocos2d::Vector<Card*> &cards)
{
	return isSingleCard(cards.begin(), cards.end());
}

bool GameScene::isSingleCard(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	if (end - begin == 1)
		return true;
	else
		return false;
}

bool GameScene::isPair(cocos2d::Vector<Card*> &cards)
{
	return isPair(cards.begin(), cards.end());
}

bool GameScene::isPair(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	if (end - begin == 2 && (*begin)->getNumber() == (*(begin + 1))->getNumber())
		return true;
	else
		return false;
}

bool GameScene::isTriplet(Vector<Card*> &cards)
{
	return isTriplet(cards.begin(), cards.end());
}

bool GameScene::isTriplet(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	if (end - begin == 3 && (*begin)->getNumber() == (*(begin + 1))->getNumber() && (*begin)->getNumber() == (*(begin + 2))->getNumber())
		return true;
	else
		return false;
}

bool GameScene::isTripletWithAnAttachedCard(Vector<Card*> &cards)
{
	return isTripletWithAnAttachedCard(cards.begin(), cards.end());
}

bool GameScene::isTripletWithAnAttachedCard(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	if (end - begin == 4 && (isTriplet(begin, begin + 3) == true && (*begin)->getNumber() != (*(begin + 3))->getNumber()
		|| isTriplet(begin + 1, end) == true && (*begin)->getNumber() != (*(begin + 1))->getNumber()))
		return true;
	else
		return false;
}

bool GameScene::isTripletWithAnAttachedPair(Vector<Card*> &cards)
{
	return isTripletWithAnAttachedPair(cards.begin(), cards.end());
}

bool GameScene::isTripletWithAnAttachedPair(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	if (end - begin == 5 && (isTriplet(begin, begin + 3) == true && isPair(begin + 3, end)
		|| isPair(begin, begin + 2) == true && isTriplet(begin + 2, end)))
		return true;
	else
		return false;

}

bool GameScene::isSequence(Vector<Card*> &cards)
{
	return isSequence(cards.begin(), cards.end());
}

bool GameScene::isSequence(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	int num = end - begin;
	if (num >= 5 && num <= 12)//必须满足数量条件
	{
		if ((*begin)->getNumber() >= 13)//最大扑克是否越界
			return false;
		for (int i = 0; i < num - 1; i++)//循环至倒数第二张扑克
		{
			if ((*(begin + i))->getNumber() != (*(begin + i + 1))->getNumber() + 1)//不满足依此递减关系
				return false;
		}
		return true;
	}
	else
		return false;
}

bool GameScene::isSequenceOfPairs(Vector<Card*> &cards)
{
	return isSequenceOfPairs(cards.begin(), cards.end());
}

bool GameScene::isSequenceOfPairs(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	int num = end - begin;
	if (num % 2 == 0 && num >= 6)//满足数量条件
	{
		if ((*begin)->getNumber() >= 13)//最大扑克是否越界
			return false;
		for (int i = 0; i < num - 1; i += 2)//循环至倒数第二张牌，比较是否构成对子
		{
			if ((*(begin + i))->getNumber() == (*(begin + i + 1))->getNumber())
			{
				if (i != num - 2)//循环至倒数第4张牌，比较是否依次递减
				{
					if ((*(begin + i))->getNumber() != (*(begin + i + 2))->getNumber() + 1)
						return false;
				}
			}
			else
				return false;
		}
	}
	else
		return false;
	return true;
}

bool GameScene::isSequenceOfTriplets(Vector<Card*> &cards)
{
	return isSequenceOfTriplets(cards.begin(), cards.end());
}

bool GameScene::isSequenceOfTriplets(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	int num = end - begin;
	if (num % 3 == 0 && num >= 6)//满足数量条件
	{
		if ((*begin)->getNumber() >= 13)//最大扑克是否越界
			return false;
		for (int i = 0; i < num - 2; i += 3)//循环至倒数第三张牌，比较是否构成三张相同的
		{
			if (isTriplet(begin + i, begin + i + 3) == false)
				return false;
			if (i != num - 3)
			{
				if ((*(begin + i))->getNumber() != (*(begin + i + 3))->getNumber() + 1)
					return false;
			}
		}
	}
	else
		return false;
	return true;
}

bool GameScene::isSequenceOfTripletsWithAttachedCards(Vector<Card*> &cards)
{
	return isSequenceOfTripletsWithAttachedCards(cards.begin(), cards.end());
}

bool GameScene::isSequenceOfTripletsWithAttachedCards(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{//99987776
	int num = end - begin;//牌的数量
	int triNum = 0;//三张的数量
	int startTri = -1;//第一个三张位置
	int endTri = -1;//最后一个三张位置
	for (int i = 0; i < num - 2; i++)
	{
		if (isTriplet(begin + i, begin + i + 3) == true)
		{
			triNum++;
			if (startTri == -1)
			{
				startTri = i;
				endTri = i;
			}
			else
			{
				endTri = i;
			}
		}
	}
	if (triNum < 2 || (endTri - startTri) % 3 != 0 || (endTri - startTri) / 3 != triNum - 1)//检验每个三张是否是连着的
		return false;
	if (triNum * 4 != end - begin)
		return false;
	if ((*(begin + startTri))->getNumber() == 13)
		return false;
	for (int i = startTri; i < endTri; i += 3)
	{
		if ((*(begin + i))->getNumber() != (*(begin + i + 3))->getNumber() + 1)
			return false;
	}
	std::vector<int> singles;
	for (int i = 0; i < startTri; i++)
	{
		singles.push_back((*(begin + i))->getNumber());
	}
	for (int i = endTri + 3; i < num; i++)
	{
		singles.push_back((*(begin + i))->getNumber());
	}
	int jokerNum = 0;//统计joker数量，判断是否有相同的扑克
	for (int i = 0; i < singles.size() - 1; i++)
	{
		if (singles[i] == 14 || singles[i] == 15)
			jokerNum++;
		for (int j = i + 1; j < singles.size(); j++)
		{
			if (singles[i] == singles[j])
				return false;
			else //跟这个不等，跟之后的也不可能相等，所以跳出。
				break;
		}
	}
	if (jokerNum == 2)
		return false;
	return true;
}

bool GameScene::isSequenceOfTripletsWithAttachedPairs(Vector<Card*> &cards)
{
	return isSequenceOfTripletsWithAttachedPairs(cards.begin(), cards.end());
}

bool GameScene::isSequenceOfTripletsWithAttachedPairs(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	int num = end - begin;
	int triNum = 0;
	int startTri = -1;
	int endTri = -1;
	for (int i = 0; i < num - 2; i++)
	{
		if (isTriplet(begin + i, begin + i + 3) == true)
		{
			triNum++;
			if (startTri == -1)
			{
				startTri = i;
				endTri = i;
			}
			else
			{
				endTri = i;
			}
		}
	}
	//9998884433/
	if (triNum < 2 || triNum * 5 != end - begin || (endTri - startTri) % 3 != 0 || (endTri - startTri) / 3 != triNum - 1)//检验每个三张是否是连着的)
		return false;
	if ((*(begin + startTri))->getNumber() == 13)
		return false;
	for (int i = startTri; i < endTri; i += 3)
	{
		if ((*(begin + i))->getNumber() != (*(begin + i + 3))->getNumber() + 1)
			return false;
	}
	std::vector<int> doubles;
	for (int i = 0; i < startTri - 1; i += 2)
	{
		if ((*(begin + i))->getNumber() == (*(begin + i + 1))->getNumber())
		{
			doubles.push_back((*(begin + i))->getNumber());
		}
		else
			return false;
	}
	for (int i = endTri + 3; i < num - 1; i += 2)//以修正由i++ -> i += 2
	{
		if ((*(begin + i))->getNumber() == (*(begin + i + 1))->getNumber())
		{
			doubles.push_back((*(begin + i))->getNumber());
		}
		else
			return false;
	}
	for (int i = 0; i < doubles.size() - 1; i++)
	{
		for (int j = i + 1; j < doubles.size(); j++)
		{
			if (doubles[i] == doubles[j])
				return false;
			else
				break;
		}
	}
	return true;
}

bool GameScene::isBomb(cocos2d::Vector<Card*> &cards)
{
	return isBomb(cards.begin(), cards.end());
}

bool GameScene::isBomb(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	if ((end - begin) == 4 && isTriplet(begin, begin + 3) == true && (*(begin + 3))->getNumber() == (*begin)->getNumber())
		return true;
	else
		return false;
}

bool GameScene::isRocket(cocos2d::Vector<Card*> &cards)
{
	return isRocket(cards.begin(), cards.end());
}

bool GameScene::isRocket(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	if ((end - begin) == 2 && (*begin)->getNumber() == 15 && (*(begin + 1))->getNumber() == 14)
		return true;
	else
		return false;
}

bool GameScene::isQuadplexSet(cocos2d::Vector<Card*> &cards)
{
	return isQuadplexSet(cards.begin(), cards.end());
}
bool GameScene::isQuadplexSet(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	if (end - begin == 6)
	{
		//int bombNum = 0;
		int startBomb = -1;
		//int endBomb = -1;
		for (int i = 0; i < end - begin - 3; i++)//找到4张位置
		{
			if (isBomb(begin + i, begin + i + 4) == true)
			{
				//bombNum++;
				/*if (startBomb == -1)
				{
					startBomb = i;
					endBomb = i;
				}
				else
				{
					endBomb = i;
				}*/
				startBomb = i;
				break;
			}
		}
		if (startBomb != -1)//炸弹个数符合，单牌不相等，没有王炸时成功
		{
			std::vector<int> singles;
			//singles.push_back((*(begin + startBomb))->getNumber());
			for (int i = 0; i < startBomb; i++)
			{
				singles.push_back((*(begin + i))->getNumber());
			}
			for (int i = startBomb + 4; i < end - begin; i++)
			{
				singles.push_back((*(begin + i))->getNumber());
			}
			int jokerNum = 0;
			for (int i = 0; i < singles.size() - 1; i++)
			{
				if (singles[i] == 14 || singles[i] == 15)
					jokerNum++;
				for (int j = i + 1; j < singles.size(); j++)
				{
					if (singles[i] == singles[j])
						return false;
				}
			}
			if (jokerNum == 2)
				return false;
			return true;
		}
	}
	return false;
}
bool GameScene::isQuadPlexSets(cocos2d::Vector<Card*> &cards)
{
	return isQuadPlexSets(cards.begin(), cards.end());
}
bool GameScene::isQuadPlexSets(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end)
{
	if (end - begin == 8)
	{
		//int bombNum = 0;
		int startBomb = -1;
		//int endBomb = -1;
		for (int i = 0; i < end - begin - 3; i++)//找到4张位置
		{
			if (isBomb(begin + i, begin + i + 4) == true)
			{
				//bombNum++;
				/*i += 3;*/
				/*if (startBomb == -1)
				{
					startBomb = i;
					endBomb = i;
				}
				else
				{
					endBomb = i;
				}*/
				startBomb = i;
				break;
			}
		}
		if (startBomb != -1 && startBomb % 2 == 0)//炸弹数量及位置合法
		{
			std::vector<int> doubles;
			//doubles.push_back((*(begin + startBomb))->getNumber());
			for (int i = 0; i < startBomb - 1; i += 2)
			{
				if ((*(begin + i))->getNumber() == (*(begin + i + 1))->getNumber())
				{
					doubles.push_back((*(begin + i))->getNumber());
				}
				else
					return false;
			}
			for (int i = startBomb + 4; i < end - begin - 1; i += 2)
			{
				if ((*(begin + i))->getNumber() == (*(begin + i + 1))->getNumber())
				{
					doubles.push_back((*(begin + i))->getNumber());
				}
				else
					return false;
			}
			//if (doubles.size() != 3)
			//	return false;
			for (int i = 0; i < doubles.size() - 1; i++)
			{
				for (int j = i + 1; j < doubles.size(); j++)
				{
					if (doubles[i] == doubles[j])
						return false;
				}
			}
			return true;
		}
	}
	return false;
}
Sprite* GameScene::makeMenuItem(std::string bg, std::string word1, std::string word2)//使用背景与文字组合成新的精灵
{
	Sprite* discardWords1 = Sprite::create(word1.c_str());//创建两个文字
	Sprite* discardWords2 = Sprite::create(word2.c_str());

	Sprite* discardNormalSprite = Sprite::create(bg.c_str());//创建左半部分图片
	discardNormalSprite->addChild(discardWords1);
	discardWords1->setPosition(Vec2(discardNormalSprite->getContentSize().width / 2 + 10, discardNormalSprite->getContentSize().height / 2));
	Sprite* flipSpriteNormal = Sprite::create(bg.c_str());//创建右半部分图片
	flipSpriteNormal->runAction(FlipX::create(true));
	flipSpriteNormal->addChild(discardWords2);
	discardWords2->setPosition(Vec2(discardNormalSprite->getContentSize().width / 2 - 10, discardNormalSprite->getContentSize().height / 2));
	Sprite* bgNormal = Sprite::create();//创建图片合成品
	bgNormal->setContentSize(Size(2 * discardNormalSprite->getContentSize().width, discardNormalSprite->getContentSize().height));
	bgNormal->addChild(discardNormalSprite);//添加左半部分
	discardNormalSprite->setPosition(discardNormalSprite->getContentSize().width / 2, discardNormalSprite->getContentSize().height / 2);
	bgNormal->addChild(flipSpriteNormal);//添加右半部分
	flipSpriteNormal->setPosition(discardNormalSprite->getContentSize().width * 3 / 2, discardNormalSprite->getContentSize().height / 2);
	bgNormal->setScale(0.5);
	bgNormal->setContentSize(bgNormal->getContentSize() * 0.5);
	return bgNormal;
}
cocos2d::Sprite * GameScene::makeMenuItem(std::string bg, std::vector<std::string> words)
{
	//创建背景
	Sprite* normalSprite = Sprite::create(bg.c_str());//创建左半部分图片
	//discardWords1->setPosition(Vec2(discardNormalSprite->getContentSize().width / 2 + 10, discardNormalSprite->getContentSize().height / 2));
	Sprite* flipNormalSprite = Sprite::create(bg.c_str());//创建右半部分图片
	flipNormalSprite->runAction(FlipX::create(true));
	//flipSpriteNormal->addChild(discardWords2);
	//discardWords2->setPosition(Vec2(discardNormalSprite->getContentSize().width / 2 - 10, discardNormalSprite->getContentSize().height / 2));
	Sprite* bgNormal = Sprite::create();//创建图片合成品
	bgNormal->setContentSize(Size(2 * normalSprite->getContentSize().width, normalSprite->getContentSize().height));//设置大小
	bgNormal->addChild(normalSprite);//添加左半部分
	normalSprite->setPosition(normalSprite->getContentSize().width / 2, normalSprite->getContentSize().height / 2);
	bgNormal->addChild(flipNormalSprite);//添加右半部分
	flipNormalSprite->setPosition(flipNormalSprite->getContentSize().width * 3 / 2, flipNormalSprite->getContentSize().height / 2);
	//创建文字
	Sprite* bgWords = Sprite::create();
	for (int i = 0; i < words.size(); i++)
	{
		Sprite* temp = Sprite::create(words[i]);
		temp->setPosition(temp->getContentSize().width / 2 + i * temp->getContentSize().width, temp->getContentSize().height);
		bgWords->addChild(temp);
	}
	log("%f, %f", bgWords->getContentSize().width, bgWords->getContentSize().height);
	bgNormal->setScale(0.5);
	bgNormal->setContentSize(bgNormal->getContentSize() * 0.5);
	return bgNormal;
}
cocos2d::Sprite * GameScene::makeMenuItem(std::string bg, std::string word1, std::string word2, std::string word3)
{
	//创建背景
	Sprite* normalSprite = Sprite::create(bg);//创建左半部分图片
													  //discardWords1->setPosition(Vec2(discardNormalSprite->getContentSize().width / 2 + 10, discardNormalSprite->getContentSize().height / 2));
	Sprite* flipNormalSprite = Sprite::create(bg);//创建右半部分图片
	flipNormalSprite->runAction(FlipX::create(true));
	//flipSpriteNormal->addChild(discardWords2);
	//discardWords2->setPosition(Vec2(discardNormalSprite->getContentSize().width / 2 - 10, discardNormalSprite->getContentSize().height / 2));
	Sprite* bgNormal = Sprite::create();//创建图片合成品
	bgNormal->setContentSize(Size(2 * normalSprite->getContentSize().width, normalSprite->getContentSize().height));//设置大小
	bgNormal->addChild(normalSprite);//添加左半部分
	normalSprite->setPosition(normalSprite->getContentSize().width / 2, normalSprite->getContentSize().height / 2);
	bgNormal->addChild(flipNormalSprite);//添加右半部分
	flipNormalSprite->setPosition(flipNormalSprite->getContentSize().width * 3 / 2, flipNormalSprite->getContentSize().height / 2);
	//添加文字
	Sprite* discardWords1 = Sprite::create(word1);//创建三个文字
	Sprite* discardWords2 = Sprite::create(word2);
	Sprite* discardWords3 = Sprite::create(word3);
	discardWords1->setPosition(discardWords1->getContentSize().width / 2 + bgNormal->getContentSize().width / 10, discardWords1->getContentSize().height);
	discardWords2->setPosition(discardWords1->getContentSize().width * 3 / 2 + bgNormal->getContentSize().width / 10, discardWords1->getContentSize().height);
	discardWords3->setPosition(discardWords1->getContentSize().width * 5 / 2 + bgNormal->getContentSize().width / 10, discardWords1->getContentSize().height);
	Sprite* bj = Sprite::create();
	bj->addChild(discardWords1);
	bj->addChild(discardWords2);
	bj->addChild(discardWords3);
	bj->setScale(0.9);
	bj->setContentSize(bj->getContentSize() * 0.9);
	/*bgNormal->addChild(discardWords1);
	bgNormal->addChild(discardWords2);
	bgNormal->addChild(discardWords3);*/
	bgNormal->addChild(bj);
	bgNormal->setScale(0.5);
	bgNormal->setContentSize(bgNormal->getContentSize() * 0.5);
	return bgNormal;
}

void GameScene::delayRemove()
{
	displayLandlordCards();
	auto delay = DelayTime::create(1.0f);//延时删除，叫地主信息
	auto func = CallFunc::create([&]() {
		circleNum++;
		removeChildByTag(PLAYER1TAG);
		removeChildByTag(PLAYER2TAG);
		removeChildByTag(PLAYER3TAG);
		
	});
	auto seq = Sequence::create(delay, func, NULL);
	runAction(seq);
}

void GameScene::refresh()
{
	for (int i = 0; i < player1Cards.size(); i++)
	{
		/*player1Cards.at(i)->setAnchorPoint(Vec2(0, 0));*/
		player1Cards.at(i)->setPosition(startx + INTERVAL * i, STARTY);
	}
}
bool GameScene::hintCards(cocos2d::Vector<Card*>& cards)
{
	CardsType type = getCardsType(cards);
	Vector<Card*> hints;
	bool find = false;

	switch (type)
	{
	case SingleCard:
		for (int i = player1Cards.size() - 1; i >= 0; --i)
		{
			if (player1Cards.at(i)->getNumber() > cards.at(0)->getNumber())
			{
				hints.pushBack(player1Cards.at(i));
				find = true;
				break;
			}
		}
		break;
	case Pair:
		for (int i = player1Cards.size() - 2; i >= 0; i--)
		{
			if (player1Cards.at(i)->getNumber() == player1Cards.at(i + 1)->getNumber() && player1Cards.at(i)->getNumber() > cards.at(0)->getNumber())
			{
				hints.pushBack(player1Cards.at(i));
				hints.pushBack(player1Cards.at(i + 1));
				find = true;
				break;
			}
		}
		break;
	case Triplet:
		for (int i = player1Cards.size() - 3; i >= 0; i--)
		{
			if (isTriplet(player1Cards.begin() + i, player1Cards.begin() + i + 3) == true && player1Cards.at(i)->getNumber() > cards.at(0)->getNumber())
			{
				hints.pushBack(player1Cards.at(i));
				hints.pushBack(player1Cards.at(i + 1));
				hints.pushBack(player1Cards.at(i + 2));
				find = true;
				break;
			}
		}
		break;
	case TripletWithAnAttachedCard:
	{
		bool can = false;
		int startCards = -1;
		for (int i = 0; i < cards.size(); i++)
		{
			if (isTriplet(cards.begin() + i, cards.begin() + i + 3) == true)
			{
				startCards = i;
				break;
			}
		}
		int start = -1;
		for (int i = player1Cards.size() - 3; i >= 0; i--)
		{
			if (isTriplet(player1Cards.begin() + i, player1Cards.begin() + i + 3) == true && player1Cards.at(i)->getNumber() > cards.at(startCards)->getNumber())
			{
				hints.pushBack(player1Cards.at(i));
				hints.pushBack(player1Cards.at(i + 1));
				hints.pushBack(player1Cards.at(i + 2));
				can = true;
				start = i;
				break;
			}
		}
		if (can)
		{
			for (int i = player1Cards.size() - 1; i >= 0; i--)
			{
				if (i >= start && i <= start + 2)
					continue;
				else
				{
					hints.pushBack(player1Cards.at(i));
					find = true;
					break;
				}
			}
		}
	}
	break;
	case TripletWithAnAttachedPair:
	{
		bool can = false;
		int startCards = -1;
		for (int i = 0; i < cards.size(); i++)
		{
			if (isTriplet(cards.begin() + i, cards.begin() + i + 3) == true)
			{
				startCards = i;
				break;
			}
		}
		int start = -1;
		for (int i = player1Cards.size() - 3; i >= 0; i--)
		{
			if (isTriplet(player1Cards.begin() + i, player1Cards.begin() + i + 3) == true && player1Cards.at(i)->getNumber() > cards.at(startCards)->getNumber())
			{
				hints.pushBack(player1Cards.at(i));
				hints.pushBack(player1Cards.at(i + 1));
				hints.pushBack(player1Cards.at(i + 2));
				start = i;
				can = true;
				break;
			}
		}
		if (can)
		{
			for (int i = player1Cards.size() - 2; i >= 0; i--)
			{
				if (i >= start - 1 && i <= start + 2)
					continue;
				else
				{
					if (isPair(player1Cards.begin() + i, player1Cards.begin() + 2 + i) == true)
					{
						hints.pushBack(player1Cards.at(i));
						hints.pushBack(player1Cards.at(i + 1));
						find = true;
						break;
					}
				}
			}
		}
	}
	break;
	case CardsType::Sequence:
	{
		int size = cards.size();
		std::vector<bool> temp(12, false);
		for (int i = 0; i < player1Cards.size(); i++)
		{
			if (player1Cards.at(i)->getNumber() < 13)
				temp[player1Cards.at(i)->getNumber() - 1] = true;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--) // i - i - size + 1
			{
				if (temp[j] == false)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > cards.at(0)->getNumber())
			{
				for (int j = 0; j < player1Cards.size(); j++)//挑出那些数
				{
					if (player1Cards.at(j)->getNumber() - 1 <= i  && player1Cards.at(j)->getNumber() - 1 >= i - size + 1 && temp[player1Cards.at(j)->getNumber() - 1] == true)
					{
						hints.pushBack(player1Cards.at(j));
						temp[player1Cards.at(j)->getNumber() - 1] = false;
					}
				}
				find = true;
				break;
			}
		}
	}
	break;
	case SequenceOfPairs:
	{
		int size = cards.size() / 2;
		std::vector<int> temp(12);
		for (int i = 0; i < player1Cards.size(); i++)
		{
			if (player1Cards.at(i)->getNumber() < 13)
				temp[player1Cards.at(i)->getNumber() - 1]++;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--)
			{
				if (temp[j] < 2)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > cards.at(0)->getNumber())
			{
				for (int j = i; j >= i - size + 1; j--)
					temp[j] = 4;
				for (int j = 0; j < player1Cards.size(); j++)//挑出那些数
				{
					if (player1Cards.at(j)->getNumber() - 1 <= i && player1Cards.at(j)->getNumber() - 1 >= i - size + 1 && temp[player1Cards.at(j)->getNumber() - 1] > 2)
					{
						hints.pushBack(player1Cards.at(j));
						temp[player1Cards.at(j)->getNumber() - 1]--;
					}
				}
				find = true;
				break;
			}
		}
	}
	break;
	case SequenceOfTriplets:
	{
		int size = cards.size() / 3;
		std::vector<int> temp(12);
		for (int i = 0; i < player1Cards.size(); i++)
		{
			if (player1Cards.at(i)->getNumber() < 13)
				temp[player1Cards.at(i)->getNumber() - 1]++;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--)
			{
				if (temp[j] < 3)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > cards.at(0)->getNumber())
			{
				for (int j = i; j >= i - size + 1; j--)
					temp[j] = 5;
				for (int j = 0; j < player1Cards.size(); j++)//挑出那些数
				{
					if (player1Cards.at(j)->getNumber() - 1 <= i && player1Cards.at(j)->getNumber() - 1 >= i - size + 1 && temp[player1Cards.at(j)->getNumber() - 1] > 2)
					{
						hints.pushBack(player1Cards.at(j));
						temp[player1Cards.at(j)->getNumber() - 1]--;
					}
				}
				find = true;
				break;
			}
		}
	}
	break;
	case SequenceOfTripletsWithAttachedCards:
	{
		int start = -1;
		for (int i = 0; i < cards.size() - 2; i++)
		{
			if (isTriplet(cards.begin() + i, cards.begin() + i + 3) == true)
			{
				start = i;
				break;
			}
		}
		int size = cards.size() / 4;
		std::vector<int> temp(12);
		for (int i = 0; i < player1Cards.size(); i++)
		{
			if (player1Cards.at(i)->getNumber() < 13)
				temp[player1Cards.at(i)->getNumber() - 1]++;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--)
			{
				if (temp[j] < 3)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > cards.at(start)->getNumber())
			{
				for (int j = i; j >= i - size + 1; j--)
					temp[j] = 5;
				int singles = 0;
				for (int j = player1Cards.size() - 1; j >= 0; j--)//挑出那些数
				{
					if (player1Cards.at(j)->getNumber() - 1 <= i && player1Cards.at(j)->getNumber() - 1 >= i - size + 1)
					{
						if (temp[player1Cards.at(j)->getNumber() - 1] > 2)
						{
							hints.pushBack(player1Cards.at(j));
							temp[player1Cards.at(j)->getNumber() - 1]--;
						}
					}
					else if (temp[player1Cards.at(j)->getNumber() - 1] > 0)
					{
						if (singles == size)
							break;
						hints.pushBack(player1Cards.at(j));
						temp[player1Cards.at(j)->getNumber() - 1] = 0;
						singles++;
					}
				}
				if (singles == size)
				{
					find = true;
					break;
				}
			}
		}
	}
	break;
	case SequenceOfTripletsWithAttachedPairs:
	{
		int start = -1;
		for (int i = 0; i < cards.size() - 2; i++)
		{
			if (isTriplet(cards.begin() + i, cards.begin() + i + 3) == true)
			{
				start = i;
				break;
			}
		}
		int size = cards.size() / 5;
		std::vector<int> temp(12);
		for (int i = 0; i < player1Cards.size(); i++)
		{
			if (player1Cards.at(i)->getNumber() < 13)
				temp[player1Cards.at(i)->getNumber() - 1]++;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--)
			{
				if (temp[j] < 3)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > cards.at(start)->getNumber())
			{
				for (int j = i; j >= i - size + 1; j--)
					temp[j] = 5;
				for (int j = 0; j < i - size + 1; j++)
				{
					if (temp[j] > 2)
						temp[j] = 4;
				}
				for (int j = i + 1; j < 11; j++)
				{
					if (temp[j] > 2)
						temp[j] = 4;
				}
				int doubles = 0;
				for (int j = player1Cards.size() - 1; j > 0; j--)//挑出那些数
				{
					if (player1Cards.at(j)->getNumber() - 1 <= i && player1Cards.at(j)->getNumber() - 1 >= i - size + 1 && temp[player1Cards.at(j)->getNumber() - 1] > 2)
					{
						hints.pushBack(player1Cards.at(j));
						temp[player1Cards.at(j)->getNumber() - 1]--;
					}
					else if (temp[player1Cards.at(j)->getNumber() - 1] > 2)
					{
						hints.pushBack(player1Cards.at(j));
						temp[player1Cards.at(j)->getNumber() - 1]--;
						doubles++;
					}
				}
				//std::vector<int> singles;
				//for (int j = 0; j < player1Cards.size(); j++)//挑出那些数
				//{
				//	if (player1Cards[j]->getNumber() - 1 <= i && player1Cards[j]->getNumber() - 1 >= i - size + 1 && temp[player1Cards[j]->getNumber() - 1] > 2)
				//	{
				//		hints.pushBack(player1Cards[j]);
				//		temp[player1Cards[j]->getNumber() - 1]--;
				//	}
				//	else
				//	{
				//		if (singles.size() < size)
				//		{
				//			bool have = false;
				//			for (int k = 0; k < singles.size(); k++)
				//			{
				//				if (singles[k] == player1Cards[j]->getNumber())
				//				{
				//					have = true;
				//					break;
				//				}
				//			}
				//			if (!have)
				//			{
				//				singles.pushBack(player1Cards[j]->getNumber());
				//				hints.pushBack(player1Cards.at(i));
				//			}
				//		}
				//	}
				//}
				if (doubles == size)
				{
					find = true;
					break;
				}
			}
		}
	}
	break;
	case Bomb:
	{
		for (int i = player1Cards.size() - 4; i >= 0; i--)
		{
			if (isBomb(player1Cards.begin() + i, player1Cards.begin() + i + 4) == true && player1Cards.at(i)->getNumber() > cards.at(0)->getNumber())
			{
				for (int j = i; j < i + 4; j++)
				{
					hints.pushBack(player1Cards.at(j));
				}
				find = true;
				break;
			}
		}
	}
	break;
	case QuadplexSet:
	{
		bool can = false;
		int startCards = -1;
		for (int i = 0; i < cards.size(); i++)
		{
			if (isTriplet(cards.begin() + i, cards.begin() + i + 4) == true)
			{
				startCards = i;
				break;
			}
		}
		int start = -1;
		for (int i = player1Cards.size() - 4; i >= 0; i--)
		{
			if (isTriplet(player1Cards.begin() + i, player1Cards.begin() + i + 4) == true && player1Cards.at(i)->getNumber() > cards.at(startCards)->getNumber())
			{
				hints.pushBack(player1Cards.at(i));
				hints.pushBack(player1Cards.at(i + 1));
				hints.pushBack(player1Cards.at(i + 2));
				hints.pushBack(player1Cards.at(i + 3));
				can = true;
				start = i;
				break;
			}
		}
		if (can)
		{
			int single1;
			for (int i = player1Cards.size() - 1; i >= 0; i--)
			{
				if (i >= start && i <= start + 2)
					continue;
				else
				{
					if (single1 == -1)
					{
						single1 = i;
						hints.pushBack(player1Cards.at(single1));
					}
					else
					{
						hints.pushBack(player1Cards.at(i));
						find = true;
						break;
					}
				}
			}
		}
	}
	break;
	case QuadPlexSets:
	{
		bool can = false;
		int startCards = -1;
		for (int i = 0; i < cards.size(); i++)
		{
			if (isTriplet(cards.begin() + i, cards.begin() + i + 4) == true)
			{
				startCards = i;
				break;
			}
		}
		int start = -1;
		for (int i = player1Cards.size() - 4; i >= 0; i--)
		{
			if (isTriplet(player1Cards.begin() + i, player1Cards.begin() + i + 4) == true && player1Cards.at(i)->getNumber() > cards.at(startCards)->getNumber())
			{
				hints.pushBack(player1Cards.at(i));
				hints.pushBack(player1Cards.at(i + 1));
				hints.pushBack(player1Cards.at(i + 2));
				hints.pushBack(player1Cards.at(i + 3));
				can = true;
				start = i;
				break;
			}
		}
		if (can)
		{
			int double1;
			for (int i = player1Cards.size() - 2; i >= 0; i--)
			{
				if (i >= start - 1 && i <= start + 2)
					continue;
				else
				{
					if (isPair(player1Cards.begin() + i, player1Cards.begin() + i + 2) == true)
					{
						if (double1 == -1)
						{
							double1 = i;
							hints.pushBack(player1Cards.at(double1));
							hints.pushBack(player1Cards.at(double1 + 1));
						}
						else
						{
							hints.pushBack(player1Cards.at(i));
							hints.pushBack(player1Cards.at(i));
							find = true;
							break;
						}
					}
				}
			}
		}
	}
	break;
	default:
		find = false;
		break;
	}
	if (find)
	{
		//先清除扑克状态。。。
		for (int i = 0; i < player1Cards.size(); i++)
		{
			if (player1Cards.at(i)->isSelect())
			{
				player1Cards.at(i)->setSelect(false);
				player1Cards.at(i)->runAction(MoveBy::create(0.1, Vec2(0, -MOVEHEIGHT)));
			}
		}
		for (int i = 0; i < hints.size(); i++)
		{
			hints.at(i)->setSelect(true);
			/*selectCards.pushBack(hints.at(i));*/
			hints.at(i)->runAction(MoveBy::create(0.1, Vec2(0, MOVEHEIGHT)));
		}
		/*	for (int i = 0; i < selectCards.size(); i++)
			{
				selectCards.at(i)->runAction(MoveBy::create(0.1, Vec3(0, MOVEHEIGHT, 0)));
			}*/
		return true;
	}
	else
	{
		log("not find");
		return false;
	}
}
void GameScene::AIplayer2(float dt)
{
	if (turn == 1)
	{
		srand((unsigned int)time(NULL));
		if (circleNum < 2)//叫地主阶段
		{
			if (clock == nullptr)
			{
				clock = ClockCtrl::create(10);
				clock->setTag(CLOCKTAG);
				clock->setPosition(Director::getInstance()->getVisibleSize().width * 3 / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
				addChild(clock);
			}
			if (clock->getTime() < 10)
			{
				removeChildByTag(PLAYER2TAG);
				if (oriLandlord == player1) //我不是初始地主
				{
					if (circleNum == 0)//叫地主环节
					{
						int r = rand() % 2;
						if (r == 0)//叫地主
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//叫地主
							words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							put2.pushBack(words);
							words->setTag(PLAYER2TAG);
							callLandlord = static_cast<Landlord>(turn);
							turn = static_cast<Turn>((turn + 1) % 3);
							circleNum++;
						}
						else//不叫
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER2TAG);
							turn = static_cast<Turn>((turn + 1) % 3);
						}
					}
					else //抢地主环节
					{
						if (callLandlord != turn)//没叫，抢地主
						{
							int r = rand() % 2;
							if (r == 0)
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER2TAG);
								grabLandlord = static_cast<Landlord>(turn);
								turn = static_cast<Turn>((turn + 1) % 3);
							}
							else
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER2TAG);
								turn = static_cast<Turn>((turn + 1) % 3);
							}
						}
						else//叫完后 抢地主
						{
							int r = rand() % 2;
							if (r == 0)//抢地主
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER2TAG);
								grabLandlord = static_cast<Landlord>(turn);
								circleNum = 2;
								delayRemove();
								insert(player2Cards, player2);
							}
							else//不抢
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER2TAG);
								circleNum = 2;
								delayRemove();
								turn = static_cast<Turn>(grabLandlord);
								insert(player3Cards, turn);
							}
						}
					}
				}
				else if (oriLandlord == player2)//我是初始地主
				{
					if (circleNum == 0)//叫地主阶段
					{
						int r = rand() % 2;
						if (r == 0)
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
							words->setTag(PLAYER2TAG);
							addChild(words);
							callLandlord = static_cast<Landlord>(turn);
							turn = static_cast<Turn>((turn + 1) % 3);
							circleNum++;
						}
						else
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER2TAG);
							turn = static_cast<Turn>((turn + 1) % 3);
						}
					}
					else //抢地主环节
					{
						int r = rand() % 2;
						if (r == 0)
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER2TAG);
							insert(player2Cards, player2);
							grabLandlord = p2;
							circleNum++;
							delayRemove();
						}
						else
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER2TAG);
							//yanchishifang
							circleNum++;
							turn = static_cast<Turn>(grabLandlord);
							delayRemove();
							if (turn == player1)
								insert(player1Cards, player1);
							else
								insert(player3Cards, player3);
						}
					}
				}
				else
				{
					if (circleNum == 0)//叫地主环节
					{
						int r = rand() % 2;
						if (r == 0)//叫地主
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//叫地主
							words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							callLandlord = p2;
							words->setTag(PLAYER2TAG);
							circleNum = 2;
							insert(player2Cards, player2);
							delayRemove();
						}
						else//不叫
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER2TAG);
							again();
						}
					}
					else
					{
						if (oriLandlord == callLandlord)//原始地主叫地主
						{
							int r = rand() % 2;
							if (r == 0)
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER2TAG);
								grabLandlord = static_cast<Landlord>(turn);
								turn = static_cast<Turn>((turn + 1) % 3);
							}
							else
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER2TAG);
								turn = static_cast<Turn>((turn + 1) % 3);
								if (grabLandlord == none)
								{
									circleNum = 2;
									delayRemove();
									insert(player3Cards, player3);
								}
							}
						}
						else
						{
							int r = rand() % 2;
							if (r == 0)
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER2TAG);
								grabLandlord = static_cast<Landlord>(turn);
								turn = static_cast<Turn>((turn + 2) % 3);
							}
							else
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER2TAG);
								turn = static_cast<Turn>(callLandlord);
								circleNum = 2;
								delayRemove();
								insert(player1Cards, player1);
							}
						}
					}
				}
				removeChildByTag(CLOCKTAG);
				clock = nullptr;
			}
		}
		else
		{
			log("player2 turn");
			if (clock == nullptr)
			{
				clock = ClockCtrl::create(10);
				clock->setTag(CLOCKTAG);
				clock->setPosition(Director::getInstance()->getVisibleSize().width * 3 / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
				addChild(clock);
			}
			for (int i = 0; i < put2.size(); i++)
				//put2.at(i)->removeFromParent();
				put2.at(i)->runAction(RemoveSelf::create());
			put2.clear();
			if (clock->getTime() < 10)
			{
				if (big == turn || otherCards.empty())//主动出牌状态
				{
					if (player2Cards.size() != 0)
					{
						otherCards.clear();//清空上一个玩家的出牌记录
						otherCards.pushBack(player2Cards.at(player2Cards.size() - 1));
						put2.pushBack(player2Cards.at(player2Cards.size() - 1));
						/* player2Cards.at(player2Cards.size() - 1)->setScale(0.5);
						 setContentSize(getContentSize() * 0.5);*/
						addChild(player2Cards.at(player2Cards.size() - 1));
						player2Cards.at(player2Cards.size() - 1)->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3 + 2, Director::getInstance()->getVisibleSize().height * 2 / 3));
						player2Cards.at(player2Cards.size() - 1)->runAction(MoveTo::create(0.1, Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3)));
						//player2Cards[player2Cards.size() - 1]->setAnchorPoint(Vec2(0.5, 0.5));
						//log("anchorpoint %f, %f", player2Cards.at(player2Cards.size() - 1)->getAnchorPoint().x, player2Cards.at(player2Cards.size() - 1)->getAnchorPoint().y);
						//player2Cards.at(player2Cards.size() - 1)->runAction(MoveTo::create(0.8f, Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height / 2)));
						/*addChild(player2Cards.at(player2Cards.size() - 1));	*/
					//	log("anchorpoint %f, %f", player2Cards.at(player2Cards.size() - 1)->getAnchorPoint().x, player2Cards.at(player2Cards.size() - 1)->getAnchorPoint().y);
					//	log("%f, %f", player2Cards.at(player2Cards.size() - 1)->getPosition().x, player2Cards.at(player2Cards.size() - 1)->getPosition().y);
						player2Cards.erase(player2Cards.begin() + player2Cards.size() - 1);
						label1->setString(StringUtils::format("%d", player2Cards.size()));
						if (player2Cards.size() == 0)
						{
							winner = player2;
							finish();
						}
					}
				}
				else //跟牌状态
				{
					Vector<Card*> temp = hint(player2Cards);//获取提示的扑克序列
					//移除扑克
					if (temp.size() > 0) //可以管上
					{
						for (int i = 0; i < temp.size(); i++)//出牌
						{
							put2.pushBack(temp.at(i));
							/*temp.at(i)->setScale(0.5);
							temp.at(i)->setContentSize(getContentSize() * 0.5);*/
							temp.at(i)->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3 + i * INTERVAL * 0.5 + 2, Director::getInstance()->getVisibleSize().height * 2 / 3));
							temp.at(i)->runAction(MoveTo::create(0.1, Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3 + i * INTERVAL * 0.5, Director::getInstance()->getVisibleSize().height * 2 / 3)));
							//temp.at(i)->setAnchorPoint(Vec2(0.5, 0.5));
						//	temp.at(i)->runAction(MoveTo::create(0.8f, Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3 + i * INTERVAL * 0.5, Director::getInstance()->getVisibleSize().height / 2)));
							addChild(temp.at(i));
						}
						otherCards.clear();//清空上个玩家出的扑克，并加入自己的
						for (int i = 0; i < temp.size(); i++)
						{
							otherCards.pushBack(temp.at(i));
						}
						log(" 2 guanshang");
						big = turn; //标记我最大
						label1->setString(StringUtils::format("%d", player2Cards.size()));
						if (player2Cards.size() == 0)
						{
							winner = player2;
							finish();
						}
					}
					else//要不起
					{
						log("2 yaobuqi");
						Sprite* sp = Sprite::create("HLDDZ_MainGame0_2x_42_01.png");
						sp->setAnchorPoint(Vec2::ZERO);
						sp->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3));
						//sp->runAction(Sequence::create(FadeIn::create(1.0f), FadeOut::create(1.0f), NULL));
						put2.pushBack(sp);
						addChild(sp);
					}
				}
				turn = static_cast<Turn>((turn + 1) % 3);
				removeChildByTag(CLOCKTAG);
				clock = nullptr;
			}
		}
	}
}
void GameScene::AIplayer3(float dt)
{
	if (doneLandlord && circleNum == 2)
	{
		//for (int i = 0; i < put3.size(); i++)
		//	//put2.at(i)->removeFromParent();
		//	put3.at(i)->runAction(RemoveSelf::create());
		//put3.clear();
		//turn = static_cast<Turn>(grabLandlord == none ? callLandlord : grabLandlord);
	}
	if (turn == 2)
	{
		if (circleNum < 2)//叫地主阶段
		{
			if (clock == nullptr)
			{
				clock = ClockCtrl::create(10);
				clock->setTag(CLOCKTAG);
				clock->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
				addChild(clock);
			}
			removeChildByTag(PLAYER3TAG);
			if (clock->getTime() < 10)
			{
				if (oriLandlord == player1)//
				{
					if (circleNum == 1)
					{
						if (callLandlord == oriLandlord)//原始地主叫地主
						{
							int r = rand() % 2;
							if (r == 0)
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
								words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER3TAG);
								grabLandlord = static_cast<Landlord>(turn);
								turn = static_cast<Turn>((turn + 1) % 3);
							}
							else
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER3TAG);
								turn = static_cast<Turn>((turn + 1) % 3);
								if (grabLandlord == none)//没人抢
								{
									circleNum = 2;
									delayRemove();
									insert(player1Cards, player1);
								}
							}
						}
						else//非原始地主叫地主
						{
							int r = rand() % 2;
							if (r == 0)//抢
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
								words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER3TAG);
								grabLandlord = static_cast<Landlord>(turn);
								turn = static_cast<Turn>((turn + 2) % 3);
							}
							else//不抢
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER3TAG);
								circleNum = 2;
								turn = static_cast<Turn>(callLandlord);
								delayRemove();
								insert(player2Cards, player2);
							}
						}
					}
					else//没人叫地主
					{
						int r = rand() % 2;
						if (r == 0)//叫地主
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//叫地主
							words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER3TAG);
							circleNum = 2;
							callLandlord = p3;
							delayRemove();
							insert(player3Cards, player3);
						}
						else//不叫
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width * 2 / 3, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER3TAG);
							again();
						}
					}
				}
				else if (oriLandlord == player2)
				{
					if (circleNum == 0)
					{
						int r = rand() % 2;
						if (r == 0)//叫地主
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//叫地主
							words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER3TAG);
							callLandlord = p3;
							turn = static_cast<Turn>((turn + 1) % 3);
							circleNum++;
						}
						else//不叫
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER3TAG);
							turn = static_cast<Turn>((turn + 1) % 3);
						}
					}
					else
					{
						if (callLandlord == turn)//叫地主后抢地主
						{
							int r = rand() % 2;
							if (r == 0)
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
								words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER3TAG);
								circleNum = 2;
								delayRemove();
								insert(player3Cards, player3);
							}
							else
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER3TAG);
								circleNum = 2;
								delayRemove();
								insert(player1Cards, player1);
								turn = static_cast<Turn>((turn + 1) % 3);
							}
						}
						else//没叫抢地主
						{
							int r = rand() % 2;
							if (r == 0)
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
								words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER3TAG);
								grabLandlord = static_cast<Landlord>(turn);
								turn = static_cast<Turn>((turn + 1) % 3);
							}
							else
							{
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
								addChild(words);
								words->setTag(PLAYER3TAG);
								turn = static_cast<Turn>((turn + 1) % 3);
							}
						}
					}
				}
				else
				{
					if (circleNum == 0)
					{
						int r = rand() % 2;
						if (r == 0)//叫地主
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//叫地主
							words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER3TAG);
							callLandlord = p3;
							turn = static_cast<Turn>((turn + 1) % 3);
							circleNum++;
						}
						else//不叫
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER3TAG);
							turn = static_cast<Turn>((turn + 1) % 3);
						}
					}
					else
					{
						int r = rand() % 2;
						if (r == 0)//抢
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
							words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER3TAG);
							grabLandlord = p3;
							circleNum++;
							delayRemove();
							insert(player3Cards, player3);
						}
						else//不抢
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
							addChild(words);
							words->setTag(PLAYER3TAG);
							circleNum = 2;
							turn = static_cast<Turn>(grabLandlord);
							delayRemove();
							if (turn == player1)
								insert(player1Cards, player1);
							else
								insert(player2Cards, player2);
						}
					}
				}
				removeChildByTag(CLOCKTAG);
				clock = nullptr;
			}
		}
		else
		{
			if (clock == nullptr)
			{
				clock = ClockCtrl::create(10);
				clock->setTag(CLOCKTAG);
				clock->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3);
				addChild(clock);
			}
			for (int i = 0; i < put3.size(); i++)
				//put2.at(i)->removeFromParent();
				put3.at(i)->runAction(RemoveSelf::create());
			put3.clear();
			if (clock->getTime() < 10)
			{
				if (turn == big || otherCards.empty())//我有出牌权
				{
					if (player3Cards.size() != 0)
					{
						otherCards.clear();
						otherCards.pushBack(player3Cards.at(player3Cards.size() - 1));
						put3.pushBack(player3Cards.at(player3Cards.size() - 1));
						//	player3Cards.at(player3Cards.size() - 1)->setScale(0.5);
						//	setContentSize(getContentSize() * 0.5);
						addChild(player3Cards.at(player3Cards.size() - 1));
						player3Cards.at(player3Cards.size() - 1)->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 4 - 2, Director::getInstance()->getVisibleSize().height * 2 / 3));
						//	log("anchorpoint %f, %f", player3Cards.at(player3Cards.size() - 1)->getAnchorPoint().x, player3Cards.at(player3Cards.size() - 1)->getAnchorPoint().y);
						player3Cards.at(player3Cards.size() - 1)->runAction(MoveTo::create(0.1, Vec2(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3)));
						//	addChild(player3Cards.at(player2Cards.size() - 1));
						//	log("anchorpoint %f, %f", player3Cards.at(player3Cards.size() - 1)->getAnchorPoint().x, player3Cards.at(player3Cards.size() - 1)->getAnchorPoint().y);
						//	log("%f, %f", player3Cards.at(player3Cards.size() - 1)->getPosition().x, player3Cards.at(player3Cards.size() - 1)->getPosition().y);
						player3Cards.erase(player3Cards.begin() + player3Cards.size() - 1);
						label2->setString(StringUtils::format("%d", player3Cards.size()));
						if (player3Cards.size() == 0)
						{
							winner = player3;
							finish();
						}
					}
				}
				else
				{
					Vector<Card*> temp = hint(player3Cards);
					if (temp.size() > 0)
					{
						for (int i = 0; i < temp.size(); i++)
						{
							put3.pushBack(temp.at(i));
							//temp.at(i)->setScale(0.5);
						//	temp.at(i)->setContentSize(getContentSize() * 0.5);
							//addChild(temp.at(i));
						   // temp.at(i)->setPosition(-Vec2( Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height / 2));
							temp.at(i)->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 4 + i * INTERVAL * 0.5 - 2, Director::getInstance()->getVisibleSize().height * 2 / 3));
							temp.at(i)->runAction(MoveTo::create(0.8f, Vec2(Director::getInstance()->getVisibleSize().width / 4 + i * INTERVAL * 0.5, Director::getInstance()->getVisibleSize().height * 2 / 3)));
							addChild(temp.at(i));
						}
						otherCards.clear();
						for (int i = 0; i < temp.size(); i++)
						{
							otherCards.pushBack(temp.at(i));
						}
						log("3 guanshang");
						big = turn;
						label2->setString(StringUtils::format("%d", player3Cards.size()));
						if (player3Cards.size() == 0)
						{
							winner = player3;
							finish();
						}
					}
					else//要不起
					{
						log("3 yaobuqi");
						Sprite* sp = Sprite::create("HLDDZ_MainGame0_2x_42_01.png");
						sp->setAnchorPoint(Vec2::ZERO);
						sp->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 2 / 3));
						//sp->runAction(Sequence::create(FadeIn::create(0.8), FadeOut::create(0.8), NULL));
						put3.pushBack(sp);
						addChild(sp);
					}
				}
				turn = static_cast<Turn>((turn + 1) % 3);
				removeChildByTag(CLOCKTAG);
				clock = nullptr;
			}
		}
	}
}
Menu* GameScene::makeMenu()
{
	Sprite* normal = makeMenuItem("HLDDZ_MainGame0_2x_295.png", "HLDDZ_MainGame0_2x_193.png", "HLDDZ_MainGame0_2x_194.png");
	Sprite* selected = makeMenuItem("HLDDZ_MainGame0_2x_296.png", "HLDDZ_MainGame0_2x_193.png", "HLDDZ_MainGame0_2x_194.png");
	MenuItemSprite* discardMenu = MenuItemSprite::create(normal, selected,//出牌菜单
		[&](Ref* ref) {
		if (turn != static_cast<Turn>(0))
			return;
		for (int i = 0; i < player1Cards.size(); i++)
		{
			if (player1Cards.at(i)->isSelect())
			{
				selectCards.pushBack(player1Cards.at(i));
			}
		}
		CardsType type = getCardsType(selectCards);
		if (type == Invalid) //出牌不合法
		{
			Sprite* invalidSprite = Sprite::create("HLDDZ_MainGame0_2x_283.png");
			invalidSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 30));
			invalidSprite->setAnchorPoint(Vec2(0, 0));
			invalidSprite->runAction(Sequence::create(FadeIn::create(1.0f), FadeOut::create(1.0f), RemoveSelf::create(true), NULL));
			addChild(invalidSprite);
		}
		else
		{
			if (otherCards.empty() || big == turn || compare(selectCards, otherCards))// 能够管上
			{
				/*for (int i = 0; i < player1Cards.size(); i++)
				{
					if (player1Cards.at(i)->isSelect())
					{
						removeChild(player1Cards.at(i));
					}
				}*/
				otherCards.clear();
				for (int i = 0; i < selectCards.size(); i++)
				{
					otherCards.pushBack(selectCards.at(i));
					put1.pushBack(selectCards.at(i));
					//addChild(selectCards.at(i));
					//selectCards.at(i)->setTag(PLAYER1TAG);
				}
				log("type:  %d", type);
				adjustCards();
				big = turn;
				turn = static_cast<Turn>((turn + 1) % 3);
				removeChildByTag(MENUTAG);
				stopActionByTag(CLOCKTAG);
				removeChildByTag(CLOCKTAG + 1);
				//animate->stop();
				//animate->autorelease();
				removeChildByTag(CLOCKTAG);
				clock = nullptr;
			}
			else // 管不上
			{
				Sprite* sprite = Sprite::create("HLDDZ_MainGame0_2x_249.png");
				sprite->setAnchorPoint(Vec2(0, 0));
				sprite->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 9);
				sprite->runAction(Sequence::create(FadeIn::create(1), FadeOut::create(1), RemoveSelf::create(true), NULL));
				addChild(sprite);
				log(" 1 guanbushang");
			}
		}
		selectCards.clear();
	});
	Sprite* passNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_167.png", "HLDDZ_MainGame0_2x_168.png");
	Sprite* passSelected = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_167.png", "HLDDZ_MainGame0_2x_168.png");
	MenuItemSprite* passMenu = MenuItemSprite::create(passNormal, passSelected, [&](Ref* ref) {//不出菜单
		Sprite* sprite = Sprite::create("HLDDZ_MainGame0_2x_42_01.png");
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
		//sprite->runAction(Sequence::create(FadeIn::create(1), FadeOut::create(1),RemoveSelf::create(true), NULL));
		put1.pushBack(sprite);
		addChild(sprite);
		turn = static_cast<Turn>((turn + 1) % 3);
		removeChildByTag(MENUTAG);
		removeChildByTag(CLOCKTAG);
		clock = nullptr;
	});

	Sprite* hintNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_191.png", "HLDDZ_MainGame0_2x_192.png");
	Sprite* hintSelected = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_191.png", "HLDDZ_MainGame0_2x_192.png");
	MenuItemSprite* hintMenu = MenuItemSprite::create(hintNormal, hintSelected, [&](Ref* ref) {//提示菜单
																							   /*Card* temp1 = Card::create(static_cast<pukeType>(0), 3);
																							   Card* temp2 = Card::create(static_cast<pukeType>(0), 3);*/
																							   /*	Card* temp3= Card::create(static_cast<pukeType>(0), 3);
																							   Card* temp4 = Card::create(static_cast<pukeType>(0), 2);
																							   Card* temp5 = Card::create(static_cast<pukeType>(0), 2);
																							   Card* temp6 = Card::create(static_cast<pukeType>(0), 2);
																							   Card* temp7 = Card::create(static_cast<pukeType>(0), 1);
																							   Card* temp8 = Card::create(static_cast<pukeType>(0), 4);*/
																							   /*Card* temp1 = Card::create(static_cast<pukeType>(0), 6);
																							   Card* temp2 = Card::create(static_cast<pukeType>(0), 5);
																							   Card* temp3 = Card::create(static_cast<pukeType>(0), 4);
																							   Card* temp4 = Card::create(static_cast<pukeType>(0), 3);
																							   Card* temp5 = Card::create(static_cast<pukeType>(0), 2);
																							   Card* temp6 = Card::create(static_cast<pukeType>(0), 1);*/
																							   /*std::vector<Card*> card;
																							   card.pushBack(temp1);
																							   card.pushBack(temp2);*/
																							   /*card.pushBack(temp3);
																							   card.pushBack(temp4);*/
																							   /*card.pushBack(temp5);
																							   card.pushBack(temp6);
																							   card.pushBack(temp7);
																							   card.pushBack(temp8);*/
		hintCards(otherCards);
	});

	Menu* menu = Menu::create(passMenu, discardMenu, hintMenu, NULL);
	menu->alignItemsHorizontally();
	return menu;
}
cocos2d::Menu * GameScene::makeMenuA()
{
	Sprite* normal = makeMenuItem("HLDDZ_MainGame0_2x_295.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
	Sprite* selected = makeMenuItem("HLDDZ_MainGame0_2x_296.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
	MenuItemSprite* discardMenu = MenuItemSprite::create(normal, selected,//不叫菜单
		[&](Ref* ref) {
	});

	Sprite* passNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
	Sprite* passSelected = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
	MenuItemSprite* passMenu = MenuItemSprite::create(passNormal, passSelected, [&](Ref* ref) {//叫地主菜单

	});

	Sprite* hintNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
	Sprite* hintSelected = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
	MenuItemSprite* hintMenu = MenuItemSprite::create(hintNormal, hintSelected, [&](Ref* ref) {//抢地主菜单

	});

	Menu* menu = Menu::create(passMenu, discardMenu, hintMenu, NULL);
	menu->alignItemsHorizontally();
	return menu;
}
void GameScene::myTurn(float dt)
{
	static bool have = false;
	static bool h = false;
	if (turn == player1)
	{
		if (circleNum < 2)//叫地主环节
		{
			if (h == false)//为了使菜单只绘制一次
			{
				removeChildByTag(PLAYER1TAG);
				clock = ClockCtrl::create(10);
				addChild(clock);
				clock->setTag(CLOCKTAG);
				clock->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
				h = true;
				if (turn == oriLandlord)//我是初始地主
				{
					if (circleNum == 0)//叫地主环节
					{   //叫地主
						Sprite* yNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
						Sprite* ySelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
						MenuItemSprite* menu1 = MenuItemSprite::create(yNormal, ySelect, [&](Ref* ref) {
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							callLandlord = static_cast<Landlord>(turn);
							turn = static_cast<Turn>((turn + 1) % 3);
							circleNum++;
							removeChildByTag(MENUTAG);
							h = false;
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
						});
						//不叫
						Sprite* nNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
						Sprite* nSelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
						MenuItemSprite* menu2 = MenuItemSprite::create(nNormal, nSelect, [&](Ref* ref) {
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							turn = static_cast<Turn>((turn + 1) % 3);
							removeChildByTag(MENUTAG);
							h = false;
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
						});
						Menu* menu = Menu::create(menu1, menu2, NULL);
						menu->setTag(MENUTAG);
						menu->alignItemsHorizontallyWithPadding(Director::getInstance()->getVisibleSize().width / 8);
						menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
						addChild(menu);
					}
					else//抢地主环节
					{
						Sprite* yNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
						Sprite* ySelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
						MenuItemSprite* menu1 = MenuItemSprite::create(yNormal, ySelect, [&](Ref* ref) {
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							removeChildByTag(MENUTAG);
							circleNum++;
							grabLandlord = p1;
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							delayRemove();
							h = false;
							insert(player1Cards, player1);
						});
						Sprite* nNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");//不抢
						Sprite* nSelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
						MenuItemSprite* menu2 = MenuItemSprite::create(nNormal, nSelect, [&](Ref* ref) {
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							removeChildByTag(MENUTAG);
							circleNum++;
							turn = static_cast<Turn>(grabLandlord);
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							delayRemove();
							h = false;
							if (grabLandlord == player2)
								insert(player2Cards, player2);
							else
								insert(player3Cards, player3);
						});
						Menu* menu = Menu::create(menu1, menu2, NULL);
						menu->setTag(MENUTAG);
						menu->alignItemsHorizontallyWithPadding(Director::getInstance()->getVisibleSize().width / 8);
						menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
						addChild(menu);
					}
				}
				else if (oriLandlord == player2)//我不是初始地主
				{
					if (circleNum == 1)//抢地主环节
					{
						if (oriLandlord == callLandlord)//原始玩家叫地主
						{
							Sprite* yNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
							Sprite* ySelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
							MenuItemSprite* menu1 = MenuItemSprite::create(yNormal, ySelect, [&](Ref* ref) {
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
								addChild(words);
								removeChildByTag(MENUTAG);
								words->setTag(PLAYER1TAG);
								removeChildByTag(CLOCKTAG);
								clock = nullptr;
								grabLandlord = static_cast<Landlord>(turn);
								turn = static_cast<Turn>(turn + 1);
								h = false;
							});
							Sprite* nNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");//不抢
							Sprite* nSelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							MenuItemSprite* menu2 = MenuItemSprite::create(nNormal, nSelect, [&](Ref* ref) {
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
								addChild(words);
								words->setTag(PLAYER1TAG);
								removeChildByTag(MENUTAG);
								removeChildByTag(CLOCKTAG);
								clock = nullptr;
								turn = static_cast<Turn>(turn + 1);
								h = false;
								if (grabLandlord == none)//没人抢地主
								{
									circleNum = 2;
									delayRemove();
									insert(player2Cards, player2);
								}
							});
							Menu* menu = Menu::create(menu1, menu2, NULL);
							menu->setTag(MENUTAG);
							menu->alignItemsHorizontallyWithPadding(Director::getInstance()->getVisibleSize().width / 8);
							menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(menu);
						}
						else //其他玩家叫地主
						{
							Sprite* yNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
							Sprite* ySelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
							MenuItemSprite* menu1 = MenuItemSprite::create(yNormal, ySelect, [&](Ref* ref) {
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
								addChild(words);
								removeChildByTag(MENUTAG);
								words->setTag(PLAYER1TAG);
								removeChildByTag(CLOCKTAG);
								clock = nullptr;
								grabLandlord = static_cast<Landlord>(turn);
								turn = static_cast<Turn>((turn + 2) % 3);
								h = false;
							});
							Sprite* nNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");//不抢
							Sprite* nSelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							MenuItemSprite* menu2 = MenuItemSprite::create(nNormal, nSelect, [&](Ref* ref) {
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
								addChild(words);
								words->setTag(PLAYER1TAG);
								removeChildByTag(MENUTAG);
								removeChildByTag(CLOCKTAG);
								clock = nullptr;
								turn = static_cast<Turn>(callLandlord);
								circleNum = 2;
								delayRemove();
								insert(player3Cards, player3);
								h = false;
							});
							Menu* menu = Menu::create(menu1, menu2, NULL);
							menu->setTag(MENUTAG);
							menu->alignItemsHorizontallyWithPadding(Director::getInstance()->getVisibleSize().width / 8);
							menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(menu);
						}
					}
					else//叫地主环节
					{
						Sprite* yNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//叫地主
						Sprite* ySelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
						MenuItemSprite* menu1 = MenuItemSprite::create(yNormal, ySelect, [&](Ref* ref) {
							callLandlord = static_cast<Landlord>(turn);
							circleNum = 2;
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							callLandlord = p1;
							delayRemove();
							removeChildByTag(MENUTAG);
						});
						Sprite* nNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");//不叫
						Sprite* nSelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
						MenuItemSprite* menu2 = MenuItemSprite::create(nNormal, nSelect, [&](Ref* ref) {
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							again();
							removeChildByTag(MENUTAG);
						});
						Menu* menu = Menu::create(menu1, menu2, NULL);
						menu->setTag(MENUTAG);
						menu->alignItemsHorizontallyWithPadding(Director::getInstance()->getVisibleSize().width / 8);
						menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
						addChild(menu);
					}
				}
				else //p3是地主
				{
					if (circleNum == 0)
					{
						Sprite* yNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
						Sprite* ySelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
						MenuItemSprite* menu1 = MenuItemSprite::create(yNormal, ySelect, [&](Ref* ref) {
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_04.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							callLandlord = static_cast<Landlord>(turn);
							turn = static_cast<Turn>((turn + 1) % 3);
							circleNum++;
							removeChildByTag(MENUTAG);
							h = false;
						});
						//不叫
						Sprite* nNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
						Sprite* nSelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
						MenuItemSprite* menu2 = MenuItemSprite::create(nNormal, nSelect, [&](Ref* ref) {
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							turn = static_cast<Turn>((turn + 1) % 3);
							removeChildByTag(MENUTAG);
							h = false;
						});
						Menu* menu = Menu::create(menu1, menu2, NULL);
						menu->setTag(MENUTAG);
						menu->alignItemsHorizontallyWithPadding(Director::getInstance()->getVisibleSize().width / 8);
						menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
						addChild(menu);
					}
					else
					{
						if (callLandlord == turn)//叫地主后的抢地主
						{
							Sprite* yNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
							Sprite* ySelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
							MenuItemSprite* menu1 = MenuItemSprite::create(yNormal, ySelect, [&](Ref* ref) {
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
								addChild(words);
								removeChildByTag(CLOCKTAG);
								clock = nullptr;
								words->setTag(PLAYER1TAG);
								removeChildByTag(MENUTAG);
								grabLandlord = p1;
								circleNum++;
								delayRemove();
								h = false;
								insert(player1Cards, player1);
							});
							Sprite* nNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");//不抢
							Sprite* nSelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							MenuItemSprite* menu2 = MenuItemSprite::create(nNormal, nSelect, [&](Ref* ref) {
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
								addChild(words);
								words->setTag(PLAYER1TAG);
								removeChildByTag(MENUTAG);
								circleNum++;
								turn = static_cast<Turn>(grabLandlord);
								removeChildByTag(CLOCKTAG);
								clock = nullptr;
								delayRemove();
								insert(player2Cards, player2);
								h = false;
							});
							Menu* menu = Menu::create(menu1, menu2, NULL);
							menu->setTag(MENUTAG);
							menu->alignItemsHorizontallyWithPadding(Director::getInstance()->getVisibleSize().width / 8);
							menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(menu);
						}
						else
						{
							Sprite* yNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");//抢地主
							Sprite* ySelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
							MenuItemSprite* menu1 = MenuItemSprite::create(yNormal, ySelect, [&](Ref* ref) {
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_03.png", "HLDDZ_MainGame0_2x_42_07.png", "HLDDZ_MainGame0_2x_42_08.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
								addChild(words);
								words->setTag(PLAYER1TAG);
								removeChildByTag(MENUTAG);
								removeChildByTag(CLOCKTAG);
								clock = nullptr;
								grabLandlord = static_cast<Landlord>(turn);
								turn = static_cast<Turn>((turn + 1) % 3);
								h = false;
							});
							Sprite* nNormal = makeMenuItem("HLDDZ_MainGame0_2x_285.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");//不抢
							Sprite* nSelect = makeMenuItem("HLDDZ_MainGame0_2x_286.png", "HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							MenuItemSprite* menu2 = MenuItemSprite::create(nNormal, nSelect, [&](Ref* ref) {
								Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
								words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
								addChild(words);
								words->setTag(PLAYER1TAG);
								removeChildByTag(MENUTAG);
								removeChildByTag(CLOCKTAG);
								clock = nullptr;
								turn = static_cast<Turn>((turn + 1) % 3);
								h = false;
							});
							Menu* menu = Menu::create(menu1, menu2, NULL);
							menu->setTag(MENUTAG);
							menu->alignItemsHorizontallyWithPadding(Director::getInstance()->getVisibleSize().width / 8);
							menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(menu);
						}
					}
				}
			}
			if (clock && clock->isStop())//为什么放在前面不行
			{
				if (oriLandlord == p1)
				{
					if (circleNum == 0)//叫地主环节
					{
						//不叫
						Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
						words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
						addChild(words);
						words->setTag(PLAYER1TAG);
						turn = static_cast<Turn>((turn + 1) % 3);
						removeChildByTag(MENUTAG);
						h = false;
						removeChildByTag(CLOCKTAG);
						clock = nullptr;
					}
					else//抢地主环节
					{
						Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
						words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
						addChild(words);
						words->setTag(PLAYER1TAG);
						removeChildByTag(MENUTAG);
						circleNum++;
						turn = static_cast<Turn>(grabLandlord);
						removeChildByTag(CLOCKTAG);
						clock = nullptr;
						delayRemove();
						h = false;
						if (grabLandlord == player2)
							insert(player2Cards, player2);
						else
							insert(player3Cards, player3);
					}
				}
				else if (oriLandlord == p2)
				{
					if (circleNum == 1)//抢地主环节
					{
						if (oriLandlord == callLandlord)//原始玩家叫地主
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							removeChildByTag(MENUTAG);
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							turn = static_cast<Turn>(turn + 1);
							h = false;
							if (grabLandlord == none)//没人抢地主
							{
								circleNum = 2;
								delayRemove();
								insert(player2Cards, player2);
							}
						}
						else //其他玩家叫地主
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							removeChildByTag(MENUTAG);
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							turn = static_cast<Turn>(callLandlord);
							circleNum = 2;
							delayRemove();
							insert(player3Cards, player3);
							h = false;
						}
					}
					else//叫地主环节
					{

						removeChildByTag(CLOCKTAG);
						clock = nullptr;
						again();
						removeChildByTag(MENUTAG);
					}
				}
				else
				{
					if (circleNum == 0)
					{
						//不叫
						Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_04.png");
						words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
						addChild(words);
						words->setTag(PLAYER1TAG);
						removeChildByTag(CLOCKTAG);
						clock = nullptr;
						turn = static_cast<Turn>((turn + 1) % 3);
						removeChildByTag(MENUTAG);
						h = false;
					}
					else
					{
						if (callLandlord == turn)//叫地主后的抢地主
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							removeChildByTag(MENUTAG);
							circleNum++;
							turn = static_cast<Turn>(grabLandlord);
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							delayRemove();
							insert(player2Cards, player2);
							h = false;
						}
						else
						{
							Sprite* words = makeSprite("HLDDZ_MainGame0_2x_42_02.png", "HLDDZ_MainGame0_2x_42_03.png");
							words->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
							addChild(words);
							words->setTag(PLAYER1TAG);
							removeChildByTag(MENUTAG);
							removeChildByTag(CLOCKTAG);
							clock = nullptr;
							turn = static_cast<Turn>((turn + 1) % 3);
							h = false;
						}
					}
				}
				clock = nullptr;
			}
		}
		else
		{
			if (have == false)
			{
				clock = ClockCtrl::create(10);
				clock->setTag(CLOCKTAG);
				clock->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height * 4 / 9);
				addChild(clock);
				for (int i = 0; i < put1.size(); i++)
					put1.at(i)->runAction(RemoveSelf::create(true));
				put1.clear();
				have = true;
				Menu* menu = makeMenu();
				menu->setTag(MENUTAG);
				menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height * 4 / 9);
				addChild(menu);
				//removeChildByTag(PLAYER1TAG);
			}
			if (clock && clock->isStop())//为什么放在前面不行
			{
				passFunc();
				clock = nullptr;
			}
		}
	}
	else
	{
		have = false;
		h = false;
	}
}
Vector<Card*> GameScene::hint(Vector<Card*>& playerCards)
{
	CardsType type = getCardsType(otherCards);
	Vector<Card*> hints;
	bool find = false;
	switch (type)
	{
	case SingleCard:
		for (int i = playerCards.size() - 1; i >= 0; --i)
		{
			if (playerCards.at(i)->getNumber() > otherCards.at(0)->getNumber())
			{
				hints.pushBack(playerCards.at(i));
				find = true;
				break;
			}
		}
		break;
	case Pair:
		for (int i = playerCards.size() - 2; i >= 0; i--)
		{
			if (playerCards.at(i)->getNumber() == playerCards.at(i + 1)->getNumber() && playerCards.at(i)->getNumber() > otherCards.at(0)->getNumber())
			{
				hints.pushBack(playerCards.at(i));
				hints.pushBack(playerCards.at(i + 1));
				find = true;
				break;
			}
		}
		break;
	case Triplet:
		for (int i = playerCards.size() - 3; i >= 0; i--)
		{
			if (isTriplet(playerCards.begin() + i, playerCards.begin() + i + 3) == true && playerCards.at(i)->getNumber() > otherCards.at(0)->getNumber())
			{
				hints.pushBack(playerCards.at(i));
				hints.pushBack(playerCards.at(i + 1));
				hints.pushBack(playerCards.at(i + 2));
				find = true;
				break;
			}
		}
		break;
	case TripletWithAnAttachedCard:
	{
		bool can = false;
		int startCards = -1;
		for (int i = 0; i < otherCards.size(); i++)
		{
			if (isTriplet(otherCards.begin() + i, otherCards.begin() + i + 3) == true)
			{
				startCards = i;
				break;
			}
		}
		int start = -1;
		for (int i = playerCards.size() - 3; i >= 0; i--)
		{
			if (isTriplet(playerCards.begin() + i, playerCards.begin() + i + 3) == true && playerCards.at(i)->getNumber() > otherCards.at(startCards)->getNumber())
			{
				hints.pushBack(playerCards.at(i));
				hints.pushBack(playerCards.at(i + 1));
				hints.pushBack(playerCards.at(i + 2));
				can = true;
				start = i;
				break;
			}
		}
		if (can)
		{
			for (int i = playerCards.size() - 1; i >= 0; i--)
			{
				if (i >= start && i <= start + 2)
					continue;
				else
				{
					hints.pushBack(playerCards.at(i));
					find = true;
					break;
				}
			}
		}
	}
	break;
	case TripletWithAnAttachedPair:
	{
		bool can = false;
		int startCards = -1;
		for (int i = 0; i < otherCards.size(); i++)
		{
			if (isTriplet(otherCards.begin() + i, otherCards.begin() + i + 3) == true)
			{
				startCards = i;
				break;
			}
		}
		int start = -1;
		for (int i = playerCards.size() - 3; i >= 0; i--)
		{
			if (isTriplet(playerCards.begin() + i, playerCards.begin() + i + 3) == true && playerCards.at(i)->getNumber() > otherCards.at(startCards)->getNumber())
			{
				hints.pushBack(playerCards.at(i));
				hints.pushBack(playerCards.at(i + 1));
				hints.pushBack(playerCards.at(i + 2));
				start = i;
				can = true;
				break;
			}
		}
		if (can)
		{
			for (int i = playerCards.size() - 2; i >= 0; i--)
			{
				if (i >= start - 1 && i <= start + 2)
					continue;
				else
				{
					if (isPair(playerCards.begin() + i, playerCards.begin() + 2 + i) == true)
					{
						hints.pushBack(playerCards.at(i));
						hints.pushBack(playerCards.at(i + 1));
						find = true;
						break;
					}
				}
			}
		}
	}
	break;
	case CardsType::Sequence:
	{
		int size = otherCards.size();
		std::vector<bool> temp(12, false);
		for (int i = 0; i < playerCards.size(); i++)
		{
			if (playerCards.at(i)->getNumber() < 13)
				temp[playerCards.at(i)->getNumber() - 1] = true;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--) // i - i - size + 1
			{
				if (temp[j] == false)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > otherCards.at(0)->getNumber())
			{
				for (int j = 0; j < playerCards.size(); j++)//挑出那些数
				{
					if (playerCards.at(j)->getNumber() - 1 <= i  && playerCards.at(j)->getNumber() - 1 >= i - size + 1 && temp[playerCards.at(j)->getNumber() - 1] == true)
					{
						hints.pushBack(playerCards.at(j));
						temp[playerCards.at(j)->getNumber() - 1] = false;
					}
				}
				find = true;
				break;
			}
		}
	}
	break;
	case SequenceOfPairs:
	{
		int size = otherCards.size() / 2;
		std::vector<int> temp(12);
		for (int i = 0; i < playerCards.size(); i++)
		{
			if (playerCards.at(i)->getNumber() < 13)
				temp[playerCards.at(i)->getNumber() - 1]++;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--)
			{
				if (temp[j] < 2)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > otherCards.at(0)->getNumber())
			{
				for (int j = i; j >= i - size + 1; j--)
					temp[j] = 4;
				for (int j = 0; j < playerCards.size(); j++)//挑出那些数
				{
					if (playerCards.at(j)->getNumber() - 1 <= i && playerCards.at(j)->getNumber() - 1 >= i - size + 1 && temp[playerCards.at(j)->getNumber() - 1] > 2)
					{
						hints.pushBack(playerCards.at(j));
						temp[playerCards.at(j)->getNumber() - 1]--;
					}
				}
				find = true;
				break;
			}
		}
	}
	break;
	case SequenceOfTriplets:
	{
		int size = otherCards.size() / 3;
		std::vector<int> temp(12);
		for (int i = 0; i < playerCards.size(); i++)
		{
			if (playerCards.at(i)->getNumber() < 13)
				temp[playerCards.at(i)->getNumber() - 1]++;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--)
			{
				if (temp[j] < 3)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > otherCards.at(0)->getNumber())
			{
				for (int j = i; j >= i - size + 1; j--)
					temp[j] = 5;
				for (int j = 0; j < playerCards.size(); j++)//挑出那些数
				{
					if (playerCards.at(j)->getNumber() - 1 <= i && playerCards.at(j)->getNumber() - 1 >= i - size + 1 && temp[playerCards.at(j)->getNumber() - 1] > 2)
					{
						hints.pushBack(playerCards.at(j));
						temp[playerCards.at(j)->getNumber() - 1]--;
					}
				}
				find = true;
				break;
			}
		}
	}
	break;
	case SequenceOfTripletsWithAttachedCards:
	{
		int start = -1;
		for (int i = 0; i < otherCards.size() - 2; i++)
		{
			if (isTriplet(otherCards.begin() + i, otherCards.begin() + i + 3) == true)
			{
				start = i;
				break;
			}
		}
		int size = otherCards.size() / 4;
		std::vector<int> temp(12);
		for (int i = 0; i < playerCards.size(); i++)
		{
			if (playerCards.at(i)->getNumber() < 13)
				temp[playerCards.at(i)->getNumber() - 1]++;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--)
			{
				if (temp[j] < 3)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > otherCards.at(start)->getNumber())
			{
				for (int j = i; j >= i - size + 1; j--)
					temp[j] = 5;
				int singles = 0;
				for (int j = playerCards.size() - 1; j >= 0; j--)//挑出那些数
				{
					if (playerCards.at(j)->getNumber() - 1 <= i && playerCards.at(j)->getNumber() - 1 >= i - size + 1)
					{
						if (temp[playerCards.at(j)->getNumber() - 1] > 2)
						{
							hints.pushBack(playerCards.at(j));
							temp[playerCards.at(j)->getNumber() - 1]--;
						}
					}
					else if (temp[playerCards.at(j)->getNumber() - 1] > 0)
					{
						if (singles == size)
							break;
						hints.pushBack(playerCards.at(j));
						temp[playerCards.at(j)->getNumber() - 1] = 0;
						singles++;
					}
				}
				if (singles == size)
				{
					find = true;
					break;
				}
			}
		}
	}
	break;
	case SequenceOfTripletsWithAttachedPairs:
	{
		int start = -1;
		for (int i = 0; i < otherCards.size() - 2; i++)
		{
			if (isTriplet(otherCards.begin() + i, otherCards.begin() + i + 3) == true)
			{
				start = i;
				break;
			}
		}
		int size = otherCards.size() / 5;
		std::vector<int> temp(12);
		for (int i = 0; i < playerCards.size(); i++)
		{
			if (playerCards.at(i)->getNumber() < 13)
				temp[playerCards.at(i)->getNumber() - 1]++;
		}
		for (int i = size - 1; i < 12; i++)
		{
			bool can = true;
			for (int j = i; j >= i - size + 1; j--)
			{
				if (temp[j] < 3)
				{
					can = false;
					break;
				}
			}
			if (can && i + 1 > otherCards.at(start)->getNumber())
			{
				for (int j = i; j >= i - size + 1; j--)
					temp[j] = 5;
				for (int j = 0; j < i - size + 1; j++)
				{
					if (temp[j] > 2)
						temp[j] = 4;
				}
				for (int j = i + 1; j < 11; j++)
				{
					if (temp[j] > 2)
						temp[j] = 4;
				}
				int doubles = 0;
				for (int j = playerCards.size() - 1; j > 0; j--)//挑出那些数
				{
					if (playerCards.at(j)->getNumber() - 1 <= i && playerCards.at(j)->getNumber() - 1 >= i - size + 1 && temp[playerCards.at(j)->getNumber() - 1] > 2)
					{
						hints.pushBack(playerCards.at(j));
						temp[playerCards.at(j)->getNumber() - 1]--;
					}
					else if (temp[playerCards.at(j)->getNumber() - 1] > 2)
					{
						hints.pushBack(playerCards.at(j));
						temp[playerCards.at(j)->getNumber() - 1]--;
						doubles++;
					}
				}
				//std::vector<int> singles;
				//for (int j = 0; j < player1Cards.size(); j++)//挑出那些数
				//{
				//	if (player1Cards[j]->getNumber() - 1 <= i && player1Cards[j]->getNumber() - 1 >= i - size + 1 && temp[player1Cards[j]->getNumber() - 1] > 2)
				//	{
				//		hints.pushBack(player1Cards[j]);
				//		temp[player1Cards[j]->getNumber() - 1]--;
				//	}
				//	else
				//	{
				//		if (singles.size() < size)
				//		{
				//			bool have = false;
				//			for (int k = 0; k < singles.size(); k++)
				//			{
				//				if (singles[k] == player1Cards[j]->getNumber())
				//				{
				//					have = true;
				//					break;
				//				}
				//			}
				//			if (!have)
				//			{
				//				singles.pushBack(player1Cards[j]->getNumber());
				//				hints.pushBack(player1Cards.at(i));
				//			}
				//		}
				//	}
				//}
				if (doubles == size)
				{
					find = true;
					break;
				}
			}
		}
	}
	break;
	case Bomb:
	{
		for (int i = playerCards.size() - 4; i >= 0; i--)
		{
			if (isBomb(playerCards.begin() + i, playerCards.begin() + i + 4) == true && playerCards.at(i)->getNumber() > otherCards.at(0)->getNumber())
			{
				for (int j = i; j < i + 4; j++)
				{
					hints.pushBack(playerCards.at(j));
				}
				find = true;
				break;
			}
		}
	}
	break;
	case QuadplexSet:
	{
		bool can = false;
		int startCards = -1;
		for (int i = 0; i < otherCards.size(); i++) //找到炸弹起始位置
		{
			if (isBomb(otherCards.begin() + i, otherCards.begin() + i + 4) == true)
			{
				startCards = i;
				break;
			}
		}
		int start = -1;
		for (int i = playerCards.size() - 4; i >= 0; i--)//找到自己牌里炸弹的位置
		{
			if (isBomb(playerCards.begin() + i, playerCards.begin() + i + 4) == true && playerCards.at(i)->getNumber() > otherCards.at(startCards)->getNumber())
			{
				hints.pushBack(playerCards.at(i));
				hints.pushBack(playerCards.at(i + 1));
				hints.pushBack(playerCards.at(i + 2));
				hints.pushBack(playerCards.at(i + 3));
				can = true;
				start = i;
				break;
			}
		}
		if (can)
		{
			int single1 = -1;
			for (int i = playerCards.size() - 1; i >= 0; i--)
			{
				if (i >= start && i <= start + 2)
					continue;
				else
				{
					if (single1 == -1)
					{
						single1 = i;
						hints.pushBack(playerCards.at(single1));
					}
					else
					{
						hints.pushBack(playerCards.at(i));
						find = true;
						break;
					}
				}
			}
		}
	}
	break;
	case QuadPlexSets:
	{
		bool can = false;//标记是找到炸弹
		int startCards = -1;//标记上家炸弹位置
		for (int i = 0; i < otherCards.size(); i++)//找到上家炸弹位置
		{
			if (isBomb(otherCards.begin() + i, otherCards.begin() + i + 4) == true)
			{
				startCards = i;
				break;
			}
		}
		int start = -1;//记录自己炸弹位置
		for (int i = playerCards.size() - 4; i >= 0; i--)//从小至大找炸弹，且比上家大
		{
			if (isBomb(playerCards.begin() + i, playerCards.begin() + i + 4) == true && playerCards.at(i)->getNumber() > otherCards.at(startCards)->getNumber())
			{
				hints.pushBack(playerCards.at(i));
				hints.pushBack(playerCards.at(i + 1));
				hints.pushBack(playerCards.at(i + 2));
				hints.pushBack(playerCards.at(i + 3));
				can = true;
				start = i;
				break;
			}
		}
		if (can)
		{
			int double1 = -1;
			for (int i = playerCards.size() - 2; i >= 0; i--)
			{
				if (i >= start - 1 && i <= start + 3)//略过炸弹
					continue;
				else
				{
					if (isPair(playerCards.begin() + i, playerCards.begin() + i + 2) == true)
					{
						if (double1 == -1)
						{
							double1 = i;
							hints.pushBack(playerCards.at(double1));
							hints.pushBack(playerCards.at(double1 + 1));
						}
						else
						{
							hints.pushBack(playerCards.at(i));
							hints.pushBack(playerCards.at(i + 1));
							find = true;
							break;
						}
					}
				}
			}
		}
	}
	break;
	default:
		break;
	}
	/*for (int i = 0; i < hints.size(); i++)
	{
		hints.at(i)->setScale(0.5);
		hints.at(i)->setContentSize(getContentSize() * 0.5);
		hints.at(i)->runAction(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width * 2 / 3 + i * INTERVAL * 0.5, Director::getInstance()->getVisibleSize().height / 2)));
	}*/
	for (int i = 0; i < hints.size(); i++)//将这些牌做标记
	{
		hints.at(i)->setSelect(true);
	}
	for (std::vector<Card*>::iterator i = playerCards.begin(); i != playerCards.end();)//移除这些牌
	{
		if ((*i)->isSelect())
		{
			playerCards.erase(i);
		}
		else
		{
			++i;
		}
	}
	return hints;
}

bool GameScene::compare(cocos2d::Vector<Card*> lhs, cocos2d::Vector<Card*> rhs) // 按大于比较
{
	CardsType lType = getCardsType(lhs);
	CardsType rType = getCardsType(rhs);
	if (lType == rType) //同类型比较
	{
		switch (lType)
		{
		case SingleCard:
			if (lhs.at(0)->getNumber() > rhs.at(0)->getNumber())
				return true;
			break;
		case Pair:
			if (lhs.at(0)->getNumber() > rhs.at(0)->getNumber())
				return true;
			break;
		case Triplet:
			if (lhs.at(0)->getNumber() > rhs.at(0)->getNumber())
				return true;
			break;
		case TripletWithAnAttachedCard:
		case TripletWithAnAttachedPair:
		case SequenceOfTriplets:
		case SequenceOfTripletsWithAttachedCards:
		case SequenceOfTripletsWithAttachedPairs:
		{
			int lf = -1, rf = -1;
			for (int i = 0; i < lhs.size() - 2; i++)
			{
				if (isTriplet(lhs.begin() + i, lhs.begin() + i + 3) == true)
				{
					lf = i;
					break;
				}
			}
			for (int i = 0; i < rhs.size() - 2; i++)
			{
				if (isTriplet(rhs.begin() + i, rhs.begin() + i + 3) == true)
				{
					rf = i;
					break;
				}
			}
			if (lhs.at(lf)->getNumber() > rhs.at(rf)->getNumber())
				return true;
		}
		break;
		case CardsType::Sequence:
			if (lhs.at(0)->getNumber() > rhs.at(0)->getNumber())
				return true;
			break;
		case SequenceOfPairs:
			if (lhs.at(0)->getNumber() > rhs.at(0)->getNumber())
				return true;
			break;
		case Bomb:
			if (lhs.at(0)->getNumber() > rhs.at(0)->getNumber())
				return true;
			break;
		case QuadplexSet:
		case QuadPlexSets:
		{
			int lf = -1, rf = -1;
			for (int i = 0; i < lhs.size() - 3; i++)
			{
				if (isBomb(lhs.begin() + i, lhs.begin() + i + 4) == true)
				{
					lf = i;
					break;
				}
			}
			for (int i = 0; i < rhs.size() - 3; i++)
			{
				if (isTriplet(rhs.begin() + i, rhs.begin() + i + 4) == true)
				{
					rf = i;
					break;
				}
			}
			if (lhs.at(lf)->getNumber() > rhs.at(rf)->getNumber())
				return true;
		}
		default:
			return false;
		}
	}
	else //不同类型的比较
	{
		if (lType == Rocket)
			return true;
		else if (lType == Bomb)
		{
			if (rType == Rocket)
				return false;
			else
				return true;
		}
	}
	return false;
}

void GameScene::passFunc()
{
	Sprite* sprite = Sprite::create("HLDDZ_MainGame0_2x_42_01.png");
	sprite->setAnchorPoint(Vec2(0, 0));
	sprite->setPosition(Director::getInstance()->getVisibleSize().width / 4, Director::getInstance()->getVisibleSize().height / 9);
	//sprite->runAction(Sequence::create(FadeIn::create(1), FadeOut::create(1),RemoveSelf::create(true), NULL));
	put1.pushBack(sprite);
	addChild(sprite);
	turn = static_cast<Turn>((turn + 1) % 3);
	removeChildByTag(MENUTAG);
	removeChildByTag(CLOCKTAG);
	clock = nullptr;
}

void GameScene::displayLandlordCards()
{
	for (int i = 0; i < landlordCards.size(); i++)
	{
		Sprite* temp = Card::create(landlordCards.at(i)->getType(), landlordCards.at(i)->getNumber());
		temp->setPosition(Director::getInstance()->getVisibleSize().width / 2 + INTERVAL * i * 0.4, Director::getInstance()->getVisibleSize().height * 7 / 8);
		temp->setScale(0.4);
		temp->setContentSize(landlordCards.at(i)->getContentSize() * 0.4);
		addChild(temp);
	}
}

void GameScene::insert(cocos2d::Vector<Card*>& player, Turn turn)
{
	//显示地主图表
	Sprite* landlord = Sprite::create("HLDDZ_MainGame0_2x_247.png");
	landlord->setScale(0.7);
	landlord->setContentSize(landlord->getContentSize() * 0.7);
	addChild(landlord);
	if (turn == player1)
		landlord->setPosition(Director::getInstance()->getVisibleSize().width / 8, Director::getInstance()->getVisibleSize().height * 2 / 5);	
	else if (turn == player3)
		landlord->setPosition(Director::getInstance()->getVisibleSize().width / 8, Director::getInstance()->getVisibleSize().height * 6 / 7);
	else
		landlord->setPosition(Director::getInstance()->getVisibleSize().width * 7 / 8, Director::getInstance()->getVisibleSize().height * 6 / 7);
	for (int i = 0; i < landlordCards.size(); i++)//插入地主牌
	{
		player.pushBack(landlordCards.at(i));
		landlordCards.at(i)->setSelect(true);
	}
	std::sort(player.begin(), player.end(), Card::cmp);//重新排序
	refresh();//更新牌的位置
	if (turn == player1)//判断是否需要向上移动
	{
		for (int i = 0; i < player.size(); i++)
		{
			if (player.at(i)->isSelect())
			{
				//auto action = MoveBy::create(0.1, Vec2(0, -MOVEHEIGHT));
				/*player.at(i)->setSelect(false);*/
				player.at(i)->setPosition(player.at(i)->getPosition().x, player.at(i)->getPosition().y + MOVEHEIGHT);
				//player.at(i)->runAction(action);
			}
		}
	}
	else
	{
		for (int i = 0; i < landlordCards.size(); i++)//插入地主牌
		{
			landlordCards.at(i)->setSelect(false);
		}
		if (turn == player2)//更新牌的数量 并缩小牌的大小
			label1->setString(StringUtils::format("%d", player.size()));
		else if (turn == player3)
			label2->setString(StringUtils::format("%d", player.size()));
		for (int i = 0; i < landlordCards.size(); i++)
		{
			landlordCards.at(i)->setScale(0.5);//进行缩放到适合大小
			landlordCards.at(i)->setAnchorPoint(Vec2(0, 0));
			landlordCards.at(i)->setContentSize(player2Cards.at(i)->getContentSize() * 0.5);
		}
	}
	if(turn == player1)
		refresh(player);
}

cocos2d::Sprite * GameScene::makeSprite(std::string word1, std::string word2)
{
	Sprite* bg = Sprite::create();
	Sprite* w1 = Sprite::create(word1);
	Sprite* w2 = Sprite::create(word2);
	w1->setPosition(w1->getContentSize().width / 2, w1->getContentSize().height / 2);
	w2->setPosition(w2->getContentSize().width * 3 / 2, w2->getContentSize().height / 2);
	bg->addChild(w1);
	bg->addChild(w2);
	bg->setScale(0.8);
	bg->setContentSize(bg->getContentSize()*0.8);
	return bg;
}

cocos2d::Sprite * GameScene::makeSprite(std::string word1, std::string word2, std::string word3)
{
	Sprite* bg = Sprite::create();
	Sprite* w1 = Sprite::create(word1);
	Sprite* w2 = Sprite::create(word2);
	Sprite* w3 = Sprite::create(word3);
	w1->setPosition(w1->getContentSize().width / 2, w1->getContentSize().height / 2);
	w2->setPosition(w2->getContentSize().width * 3 / 2, w2->getContentSize().height / 2);
	w3->setPosition(w3->getContentSize().width * 5 / 2, w3->getContentSize().height / 2);
	bg->addChild(w1);
	bg->addChild(w2);
	bg->addChild(w3);
	bg->setScale(0.8);
	bg->setContentSize(bg->getContentSize()*0.8);
	return bg;
}

void GameScene::refresh(cocos2d::Vector<Card*>& player)
{
	for (int i = 0; i < player.size(); i++)
	{
		player.at(i)->removeFromParent();
	}
	for (int i = 0; i < player.size(); i++)
	{
		addChild(player.at(i));
	}
}

void GameScene::again()
{
	Scene* scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void GameScene::finish()
{
	unscheduleAllSelectors();
	Sprite* bg = Sprite::create();
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	/*bg->setScaleX(Director::getInstance()->getVisibleSize().width / bg->getContentSize().width);
	bg->setScaleY(Director::getInstance()->getVisibleSize().height / bg->getContentSize().height);
	bg->setContentSize(Director::getInstance()->getVisibleSize());*/
	std::string stateW, stateD;
	if (winner == static_cast<Turn>(grabLandlord == none ? callLandlord : grabLandlord))
	{
		stateW = "lw.png";
		stateD = "fd.png";
	}
	else
	{
		stateW = "fw.png";
		stateD = "ld.png";
	}
	Sprite* statew = Sprite::create(stateW);
	Sprite* stated = Sprite::create(stateD);
	Size temp = statew->getContentSize();
	statew->setPosition(temp.width / 2, temp.height / 2);
	stated->setPosition(temp.width / 2, -temp.height / 2);
	bg->addChild(statew);
	bg->addChild(stated);
	addChild(bg);
	bg->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
	MenuItemImage* menuItem = MenuItemImage::create("againNormal.png", "againSelect.png", [&](Ref* ref) {
		again();
	});
	Menu* menu = Menu::create(menuItem, NULL);
	menu->setAnchorPoint(Vec2(0.5, 0.5));
	menu->setPosition(temp.width / 2, -temp.height * 3 / 2);
	bg->addChild(menu);
}
		
CardsType GameScene::getCardsType(cocos2d::Vector<Card*> &cards)
{
	if (isSingleCard(cards))
		return SingleCard;
	else if (isPair(cards))
		return Pair;
	else if (isTriplet(cards))
		return Triplet;
	else if (isTripletWithAnAttachedCard(cards))
		return TripletWithAnAttachedCard;
	else if (isTripletWithAnAttachedPair(cards))
		return TripletWithAnAttachedPair;
	else if (isSequence(cards))
		return CardsType::Sequence;
	else if (isSequenceOfPairs(cards))
		return SequenceOfPairs;
	else if (isSequenceOfTriplets(cards))
		return SequenceOfTriplets;
	else if (isSequenceOfTripletsWithAttachedCards(cards))
		return SequenceOfTripletsWithAttachedCards;
	else if (isSequenceOfTripletsWithAttachedPairs(cards))
		return SequenceOfTripletsWithAttachedPairs;
	else if (isBomb(cards))
		return Bomb;
	else if (isRocket(cards))
		return Rocket;
	else if (isQuadplexSet(cards))
		return QuadplexSet;
	else if (isQuadPlexSets(cards))
		return QuadPlexSets;
	else
		return Invalid;
}
void GameScene::adjustCards()
{
	int midNum = player1Cards.size() / 2;
	int offsetLeft = 0;
	int offsetRight = 0;
	for (int i = 0; i < player1Cards.size(); ++i)
	{
		if (player1Cards.at(i)->isSelect())
		{
			if (i >= midNum)
				++offsetLeft;
			else
				++offsetRight;
		}
	}
	for (std::vector<Card*>::iterator i = player1Cards.begin(); i != player1Cards.end();)
	{
		if ((*i)->isSelect())
		{
			player1Cards.erase(i);
		}
		else
		{
			++i;
		}
	}
	if (player1Cards.size() == 0)
	{
		winner = player1;
		finish();
	}
	for (int i = 0; i < selectCards.size(); ++i)
	{
		selectCards.at(i)->setScale(0.5);
		selectCards.at(i)->setContentSize(selectCards.at(i)->getContentSize() * 0.5);
		selectCards.at(i)->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2 + i * INTERVAL * 0.5, Director::getInstance()->getVisibleSize().height * 4 / 9)),
			/*RemoveSelf::create(true),*/ NULL));//?????????
	}
	startx = startx + offsetRight * INTERVAL / 2 + offsetLeft * INTERVAL / 2;
	//displayCards();
	refresh();
}