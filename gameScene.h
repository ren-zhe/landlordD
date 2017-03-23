#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
#include"cocos2d.h"
#include"card.h"
#include"clock.h"
#define MOVEHEIGHT 20 //扑克向上移动的高度
#define INTERVAL 35 //扑克之间的间隔
#define STARTY 40 //扑克底边高度

#define MENUTAG 40 //menu标签
#define PLAYER1TAG 41//玩家扑克标签
#define PLAYER2TAG 42
#define PLAYER3TAG 43
#define CLOCKTAG 50
#define LANDLORDTAG 60
enum Turn {
	player1,
	player2,
	player3
};
enum Landlord
{
	p1,
	p2,
	p3,
	none
};
enum CardsType
{
	SingleCard,
	Pair,
	Triplet,
	TripletWithAnAttachedCard,
	TripletWithAnAttachedPair,
	Sequence,
	SequenceOfPairs,
	SequenceOfTriplets,
	SequenceOfTripletsWithAttachedCards,
	SequenceOfTripletsWithAttachedPairs,
	Bomb,
	Rocket,
	QuadplexSet,
	QuadPlexSets,
	Invalid,
};

class GameScene : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(GameScene);
	static cocos2d::Scene* createScene();
	void createCards();
	void shuffleCards();
	void discardCards();
	void sortCards();
	void displayCards();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	CardsType getCardsType(cocos2d::Vector<Card*> &cards);//
	bool isSingleCard(cocos2d::Vector<Card*> &cards);//
	bool isSingleCard(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isPair(cocos2d::Vector<Card*> &cards);
	bool isPair(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isTriplet(cocos2d::Vector<Card*> &cards);
	bool isTriplet(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isTripletWithAnAttachedCard(cocos2d::Vector<Card*> &cards);
	bool isTripletWithAnAttachedCard(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isTripletWithAnAttachedPair(cocos2d::Vector<Card*> &cards);
	bool isTripletWithAnAttachedPair(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isSequence(cocos2d::Vector<Card*> &cards);
	bool isSequence(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isSequenceOfPairs(cocos2d::Vector<Card*> &cards);
	bool isSequenceOfPairs(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isSequenceOfTriplets(cocos2d::Vector<Card*> &cards);
	bool isSequenceOfTriplets(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isSequenceOfTripletsWithAttachedCards(cocos2d::Vector<Card*> &cards);
	bool isSequenceOfTripletsWithAttachedCards(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isSequenceOfTripletsWithAttachedPairs(cocos2d::Vector<Card*> &cards);
	bool isSequenceOfTripletsWithAttachedPairs(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isBomb(cocos2d::Vector<Card*> &cards);
	bool isBomb(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isRocket(cocos2d::Vector<Card*> &cards);
	bool isRocket(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isQuadplexSet(cocos2d::Vector<Card*> &cards);
	bool isQuadplexSet(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);
	bool isQuadPlexSets(cocos2d::Vector<Card*> &cards);
	bool isQuadPlexSets(cocos2d::Vector<Card*>::iterator begin, cocos2d::Vector<Card*>::iterator end);//
	cocos2d::Sprite* makeMenuItem(std::string bg, std::string word1, std::string word2);//
	cocos2d::Sprite* makeMenuItem(std::string bg, std::vector<std::string> words);//
	cocos2d::Sprite* makeMenuItem(std::string bg, std::string word1, std::string word2, std::string word3);//
	void delayRemove();
	void adjustCards();
	void refresh();
	bool hintCards(cocos2d::Vector<Card*> &cards);
	void AIplayer2(float);
	void AIplayer3(float);
	cocos2d::Menu* makeMenu();
	cocos2d::Menu* makeMenuA();
	void myTurn(float dt);
	cocos2d::Vector<Card*> hint(cocos2d::Vector<Card*> & playerCards);
	bool compare(cocos2d::Vector<Card*> lhs, cocos2d::Vector<Card*> rhs);//
	void passFunc();
	void displayLandlordCards();
	void insert(cocos2d::Vector<Card*> &player, Turn turn);
	cocos2d::Sprite* makeSprite(std::string word1, std::string word2);//
	cocos2d::Sprite* makeSprite(std::string word1, std::string word2, std::string word3);//
	void refresh(cocos2d::Vector<Card*> & player);
	void again();
	void finish();
private:
	int startTouchNum;
	float startx;
	cocos2d::Vector<Card*> allCards;
	cocos2d::Vector<Card*> player1Cards;
	cocos2d::Vector<Card*> player2Cards;
	cocos2d::Vector<Card*> player3Cards;
	cocos2d::Vector<Card*> landlordCards;
	cocos2d::Vector<Card*> selectCards;
	cocos2d::Vector<Card*> otherCards;
	cocos2d::Menu* turnMenu;
	cocos2d::Vector<cocos2d::Sprite*> put1;
	cocos2d::Vector<cocos2d::Sprite*> put2;
	cocos2d::Vector<cocos2d::Sprite*> put3;
	cocos2d::Animate* animate;
	cocos2d::Label* label1;
	cocos2d::Label* label2;
	ClockCtrl* clock;
	Turn turn;
	Turn big;
	Landlord oriLandlord;
	Landlord callLandlord;
	Landlord grabLandlord;
	int circleNum;
	bool doneLandlord;
	Turn winner;
};
#endif
