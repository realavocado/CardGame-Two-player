#include "Player.h"
string Player::getName()
{
    return this->name;
}

void Player::setName(string name)
{
    this->name = name;
}

Player::Player()
{
}

Player::Player(string name)
{
    this->name = name;
    this->score = 0;
}

void Player::Hit()                
{
    fstream f;
    vector<int> set;                 //储存纸牌信息
    f.open("cardSet.txt", ios::in | ios::out);
    int card;                             //纸牌面值
    while (!f.eof()) {
        f >> card;
        set.push_back(card);     //把txt文件里的纸牌导入到vector
    }
    f.close();

    int len = set.size();
    int newRank = set[len - 1];
    Card newCard = Card(newRank);
    this->handCard.push_back(newCard);                  //向玩家手上加入一张牌

    if (this->hasPair(this->handCard)) {
        this->score = this->score + newRank;             //如果这张牌凑成对子 加分
    }

    set.pop_back();
    f.open("cardSet.txt", ios::trunc | ios::out);             //删除txt文件里最末尾的一张牌
    for (int i = 0; i < set.size(); i++) {
        f << set[i];
        if (i < set.size() - 1)
            f << '\n';
    }
    f.close();
}

void Player::Fold(Player& p1, Player& p2)         
{
    if (!p1.handCard.empty() && !p2.handCard.empty()) {    //如果两人都有牌
        p1.handCard = p1.sortValue(p1.handCard);
        p2.handCard = p2.sortValue(p2.handCard);
        if (p1.handCard[0].rank > p2.handCard[0].rank) {
            this->score = this->score + p2.handCard[0].rank;
            p2.handCard.erase(p2.handCard.begin());                         
        }
        else if (p1.handCard[0].rank < p2.handCard[0].rank) { 
            this->score = this->score + p1.handCard[0].rank;
            p1.handCard.erase(p1.handCard.begin());
        }
    }
}


string Player::showCard()                                 //显示自己手上所有卡牌信息 例如[1][4][7]
{
    if (!this->handCard.empty()) {
        string pCard = "\t";
        for (int i = 0; i < this->handCard.size(); i++) {           
         pCard = pCard.append(handCard[i].display());           
        }
        return pCard;
    }
    else {
        return "\t";
    }
}

bool Player::Quit()     //玩家退出游戏
{
    return true;
}

bool Player::hasPair(vector<Card> vec)        //判断一个Card型vector中是否有对子或者重复   
{                                                                      //是roundOver的充分条件之一
    if (vec.size() > 1) {
        for (int i = 0; i < vec.size(); i++) {
            for (int j = i + 1; j < vec.size(); j++) {
                if (vec[i].rank == vec[j].rank) {
                    return true;
                }
            }
        }
        return false;
    }
    else {
        return false;
    }
}

bool Player::isMinTie(Player& p1, Player& p2)                  //判断玩家的手牌最小值是否构成了tie（平手）
{
    p1.handCard = p1.sortValue(p1.handCard);
    p2.handCard = p2.sortValue(p2.handCard);
    vector<Card> mix;                                           //每个玩家手上的最小牌组在一起
    mix.push_back(p1.handCard[0]);
    mix.push_back(p2.handCard[0]);
   // mix = this->sortValue(mix);                             //每个人手上的最小牌组在一起再排出最小牌   
    if (this->hasPair(mix)) {                                      //如果最小牌重合了
        return true;
    }
    else {
        return false;
    }
}

bool Player::minLargerThanOther(Player &p)     //判断this的手牌最小值是否大于other
{
    this->handCard = this->sortValue(this->handCard);
    p.handCard = p.sortValue(p.handCard);
    if (this->handCard[0].rank > p.handCard[0].rank) {
        return true;
    }
    else {
        return false;
    }
    
}

vector<Card> Player::sortValue(vector<Card>& vec)           //从小到大排序
{
    if (vec.size() > 1) {
        for (int i = 0; i < vec.size(); i++) {
            int minIndex = i;
            for (int j = i + 1; j < vec.size(); j++) {
                if (vec[j].rank < vec[i].rank) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                int temp = vec[i].rank;
                vec[i].rank = vec[minIndex].rank;
                vec[minIndex].rank = temp;
            }
        }
        return vec;
    }
    else {
        return vec;
    }
}
