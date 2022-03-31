#pragma once
#ifndef _PLAYER
#define _PLAYER
using namespace std;
#include <iostream>
#include<string>
#include"Card.h"
#include<vector>
#include<fstream>
#include <algorithm>    // std::sort
class Player
{
	//friend void sendCard(vector<Player>& player);   //友元函数

private:
	string name;

public:
	int score;
	vector<Card> handCard;

public:
	string getName();
	void setName(string name);
	Player();
	Player(string name);

	void Hit();
	void Fold(Player &p1, Player &p2);
	string showCard();                                                              //显示自己手上所有卡牌信息 例如[1][4][7]
	bool Quit();                                //退出游戏
	bool hasPair(vector<Card> vec);                                     //判断手中的牌是否有对子
	bool isMinTie(Player &p1, Player &p2);                             //如果最小牌重复了（tie）则重新发牌
	bool minLargerThanOther(Player& p);        //判断this的手牌最小值是否大于other
	vector<Card> sortValue(vector<Card> &vec);                  //从小到大排序
};
#endif
