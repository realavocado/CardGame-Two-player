// CardGame.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include"Card.h"
#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include "Player.h"
using namespace std;


//游戏界面
void gameInterface(Player &p1, Player &p2) {
	cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "            Pairs::Target Score->31" << endl;
	cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Name: " << p1.getName() << "\t"<<"Score: " <<p1.score<< endl;
	cout << "Hand: ";
	for (int i = 0; i < p1.handCard.size(); i++) {
		cout << p1.handCard[i].display();
	}
	cout << endl;
	//cout << "Hand: "<<p1.showCard(); cout << "\n";
	cout << "	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Name: " << p2.getName() << "\t "<<"Score: " << p2.score<< endl;
	cout << "Hand:";
	for (int i = 0; i < p2.handCard.size(); i++) {
		cout << p2.handCard[i].display();
	}
	cout << endl;
	//cout << "Hand: "<<p2.showCard(); cout << "\n";
	cout << "	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
}

//显示规则
void showRule() {

	cout << "Welcome to the card game. Here's the rule of the game." << endl;
	cout << "\n";
	fstream fin("rule.txt", ios::in);
	char lineContent[1000];
	while (fin.eof() == 0)
	{
		fin.getline(lineContent, 1000);
		cout << lineContent << endl;
	}
	fin.close();
	system("pause");
	system("cls");
}

//发牌给玩家 (初始化游戏时和每一轮结束后)
void sendCard(Player &p1, Player &p2) {
	vector<int> set;  //过渡vector
	fstream f;

	f.open("cardSet.txt", ios::in | ios::out);
	int card;  //纸牌面值
	while (!f.eof()) {
		f >> card;
		set.push_back(card);
	}
	f.close();
	int len = set.size();
	int newRank1 = set[len - 1];	
	int newRank2 = set[len - 2];
	Card newCard1 = Card(newRank1);
	Card newCard2 = Card(newRank2);
	p1.handCard.push_back(newCard1);
	p2.handCard.push_back(newCard2);
	
	set.pop_back();                                   //删除txt文件里末尾的2张牌
	set.pop_back();
	f.open("cardSet.txt", ios::trunc | ios::out);             
	for (int i = 0; i < set.size(); i++) {
		f << set[i];
		if (i < set.size() - 1)
			f << '\n';
	}
	f.close();
}

//如果出现最小牌打平（tie）的情况，重新发牌
void reSend(Player &p1, Player &p2) {
	p1.handCard.erase(p1.handCard.begin());         //把所有人最小牌删掉
	p2.handCard.erase(p2.handCard.begin());
	
	sendCard(p1, p2);
}

//如果最小牌打平（tie）重发，重发后自身又构成了对子，继续重发
void reSendToP(Player& p) {
	p.handCard.erase(p.handCard.begin()+p.handCard.size()-1);     //先把末尾牌删掉
	vector<int> set;  //过渡vector
	fstream f;
	f.open("cardSet.txt", ios::in | ios::out);
	int card;  //纸牌面值
	while (!f.eof()) {
		f >> card;
		set.push_back(card);
	}
	f.close();

	int len = set.size();
	int newRank1 = set[len - 1];
	Card newCard1 = Card(newRank1);
	p.handCard.push_back(newCard1);
	set.pop_back();

	f.open("cardSet.txt", ios::trunc | ios::out);
	for (int i = 0; i < set.size(); i++) {
		f << set[i];
		if (i < set.size() - 1)
			f << '\n';
	}
	f.close();
}

//判断牌量是否充足
bool isCardEmpty() {
	fstream fin("cardSet.txt", ios::in);
	vector<int> set;                   //过渡vector
	int card;
	while (!fin.eof()) {
		fin >> card;
		set.push_back(card);
	}
	fin.close();
	if (set.size() < 4) {      //如果纸牌数量少于4
		return true;           //确实不够
	}
	else {
		return false;
	}
}

//补充纸牌数量
void reFillCard() {
	fstream fin("cardSet.txt", ios::in);
	vector<int> set;                   //过渡vector
	int card;
	while (!fin.eof()) {
		fin >> card;
		set.push_back(card);
	}
	fin.close();
	int len = set.size();      //剩余纸牌数量
	int fill = 55 - len;        //需要补充的纸牌数量

	srand(time(0));           //初始化随机数发生器

	fstream fout("cardSet.txt", ios::out | ios::app);     //向文件里输入所需数量的1-10之间的随机整数
	for (int i = 0; i < fill; i++) {
		int temp = rand() % 10 + 1;
		fout << "\n";
		fout << temp;
	}
	fout.close();
}

//保存游戏进度
void storeGame(Player &p)
{
	string m = ".txt";
	string n = "Score";
	string name1 = p.getName();
	string fileNameCard1 = name1.append(m);         //储存卡牌 "player1.txt"
	string fileNameScore1 = name1.append(n).append(m);     //储存分数  "player1Score.txt"

	fstream fout (fileNameCard1, ios::out | ios::trunc);
	for (int i = 0; i < p.handCard.size(); i++) {
		fout << p.handCard[i].rank;		
		if (i < p.handCard.size()-1) {
			fout << "\n";
		}
	}
	fout.close();

	fstream foutB(fileNameScore1, ios::out | ios::trunc);
	foutB << p.score;
	foutB.close();
}

//检查单个玩家姓名是否已经存在  防止名字重复 加载出错误的游戏进度
bool isNameExist(string name) {
	fstream fin("playerSaved.txt", ios::in);
	string name1, name2;
	while (!fin.eof()) {
		fin >> name1 >> name2;
		if (name1 == name || name2 == name ) {
			fin.close();
			return true;
		}
	}
	fin.close();
	return false;
}

//检查这两个玩家的信息是否被储存过 用来加载上次游戏进度
bool isPlayerSaved(Player& p1, Player& p2) {
	fstream fin("playerSaved.txt", ios::in);
	string name1, name2;
	while (!fin.eof()) {
		fin >> name1 >> name2;
		if ((name1 == p1.getName() && name2 == p2.getName()) || (name1 == p2.getName() && name2 == p1.getName())) {
			fin.close();
			return true;
		}
	}
	fin.close();
	return false;
}

//向文件里保存save game的玩家 成对验证
void savePlayer(Player& p1, Player& p2) {
	if (!isPlayerSaved(p1, p2)) {                                             //不重复保存
		fstream fout("playerSaved.txt", ios::out | ios::app);
		fout << "\n";
		fout << p1.getName() << "\t" << p2.getName();
		fout.close();
	}
}

//继续上次游戏缓存
void continueLastGame(Player &p)
{
	string m = ".txt";
	string n = "Score";
	string name = p.getName();
	string fileNameA = name.append(m);          //读取卡牌"player.txt"
	string fileNameB = name.append(n).append(m);     //读取分数  "playerScore.txt"
	fstream fin(fileNameA, ios::in);
	int success = 0;
	int cardRank;
	while (!fin.eof()) {
		fin >> cardRank;
		p.handCard.push_back(cardRank);
	}
	fin.close();
		
	fstream finB(fileNameB, ios::in);
	int score;
	finB >> score;
	p.score = score;
	finB.close();
}

//判断游戏是否结束
bool isGameOver(Player &p1, Player &p2) {
	if (p1.score>=31 || p2.score >= 31) {
		return true;
	}
	else {
		return false;
	}
}

int main()
{
	begin:
	cout << "Welcome to the card game." << endl;

	cout << "Player NO. 1" << ", " << "tell me your name." << endl;           //输入第一个玩家姓名
	string name1;
	cin >> name1;
	Player p1 = Player(name1);

	cout << "Player NO. 2" << ", " << "tell me your name." << endl;             //输入第二个玩家姓名
	string name2;
	cin >> name2;
	Player p2 = Player(name2);

	cout << "[1] Start new game.   [2] Continue last game." << endl;
	string choice;
	cin >> choice;
	if (choice == "1") {
		if (isNameExist(name1) || isNameExist(name2)) {
			cout << "There's a name already exist. Please input again." << endl;
			system("pause");
			system("cls");
			goto begin;
		}
		cout << "The game is going to start..." << endl;
	}
	else if (choice == "2") {
		if (isPlayerSaved(p1, p2)) {           //如果玩家已被储存过，加载上次游戏进度
			continueLastGame(p1);
			continueLastGame(p2);
			goto newGame;
		}
		else {                                         //否则开始新游戏
			cout << "There's no information saved about these 2 players in a game." << endl;
			cout << "Start a new game due to default option." << endl;
			system("pause");
			system("cls");
		}
	}
	else {
		cout << "Invalid choice. Please input correctly." << endl;
		system("pause");
		system("cls");
		goto begin;
	}

	if (!isCardEmpty()) {                         //若牌充足，发牌给玩家
		sendCard(p1, p2);                             	 
	}
	else {
		reFillCard();
		sendCard(p1, p2);
	}

newGame:
	showRule();                                                  //显示规则，游戏准备开始
	int target = 31;                 //目标分数公式
	//bool successB = 0;              //一轮是否结束
	int tempCom;                     //接收玩家指令
	Player* p = new Player();                                       //Player型指针 用来指定谁先出牌
	int roundNum = 1;            //记录轮数
	while (true) {    
	loop:                                                              //loop标识 被goto语句查找
		 if (!isCardEmpty()) {                                  //一轮开始循环（开始条件：循环）判断牌量充不充足，需不需要补牌
			while (!isCardEmpty()) {                    
				if (!p1.isMinTie(p1, p2)) {                                //判断玩家最小纸牌是否一样
					for (int i = 0; i < 2; i++) {                           //一轮正式开始
						cout << "Round " << roundNum << endl;                          //显示轮数
						gameInterface(p1, p2);                 	                       //游戏开始，显示游戏界面
						
						if (p1.minLargerThanOther(p2) == false) {                    //判断每一轮谁先出牌  p1牌最小
							if (i == 0) {
								p = &p1;
								cout << p1.getName() << ", it's yout turn." << endl;
							}
							else {
								p = &p2;
								cout << p2.getName() << ", it's yout turn." << endl;
							}
						}
						else if (p1.minLargerThanOther(p2) == true) {                  //p2牌最小
							if (i == 0) {
								p = &p2;
								cout << p2.getName() << ", it's yout turn." << endl;
							}
							else {
								p = &p1;
								cout << p1.getName() << ", it's yout turn." << endl;
							}
						}

						cout << "Please give command. 1: Hit   2: Fold   3: Quit   4: Save game" << endl;                  //玩家选择操作（Hit或Fold或Quit）
						cin >> tempCom;					
						switch (tempCom)
						{
						case 1:
							p->Hit();
							if (p->hasPair(p->handCard)) {
								cout << p->getName() << " ended this round with a PAIR." << p->handCard[p->handCard.size() - 1].display() << p->handCard[p->handCard.size() - 1].display() << endl;
								p1.handCard.clear();
								p2.handCard.clear();             //新的一轮，清除卡牌								
								sendCard(p1, p2);                             	   //若牌充足，发牌给玩家															
								system("pause");
								system("cls");             //每轮结束之后清屏
								 
								//及时判断游戏是否结束
								if (isGameOver(p1, p2)) {
									system("cls");
									gameInterface(p1, p2);
									if (p1.score >= 31) {
										cout << "Player " << p1.getName() << " has scored 31 points and" << " lose the game." << endl;
									}
									else {
										cout << "Player " << p2.getName() << " has scored 31 points and" << " lose the game." << endl;
									}
									cout << "GAMEOVER" << endl;
									system("pause");
									return 0;
								}
								roundNum++;           //如果Hit之后凑成对子，这一轮结束
								goto loop;        //goto 直接跳至最外层循环
							}
							cout << "\n";
							
											
							break;
						case 2:

							if (!p->isMinTie(p1, p2)) {
								p->Fold(p1, p2);
								cout << p->getName() << " ended this round with the [Fold] operation." << endl;
								p1.handCard.clear();
								p2.handCard.clear();             //新的一轮，清除卡牌					
								sendCard(p1, p2);                             	   //若牌充足，发牌给玩家															
								system("pause");
								system("cls");   //每轮结束之后清屏	  					                     
	                        }
							else {                               //如果最小牌打平了，则一直重新发牌，直到有且仅有一张最小牌
								cout << "There are players tied for the lowest card. Deal additional cards." << endl;
								reSend(p1, p2);
							}
																			
							//及时判断游戏是否结束
							if (isGameOver(p1, p2)) {
								system("cls");
								gameInterface(p1, p2);								
								if (p1.score >= 31) {
									cout << "Player " << p1.getName() << " has scored 31 points and" << " lose the game." << endl;
								}
								else {
									cout << "Player " << p2.getName() << " has scored 31 points and" << " lose the game." << endl;
								}
								cout << "GAMEOVER" << endl;
								system("pause");
								return 0;
							}
							roundNum++;     //有玩家执行fold操作后，一轮结束	
							goto loop;    //goto跳至最外层循环重新开始
							break;

						case 3:
							if (p->Quit()) {
								cout << "Player " << p->getName() << " has quit the game." << endl;
								cout << "GAME OVER." << endl;
								system("pause");
								return 0;                             //两人对局，一名玩家退出游戏，游戏结束							
							}
							break;
							  
						case 4:                                       //保存游戏进度
							storeGame(p1); 
							storeGame(p2);
							savePlayer(p1, p2);
							cout << "Your game has been saved. Continue playing next time." << endl;
							system("pause");
							system("cls");
							return 0;
							break;
						default:
							break;
						} 
						system("pause");
						system("cls");
					}
				}
				else {
				cout << "Round " << roundNum << endl;
				    gameInterface(p1, p2);
					cout << "There are players tied for the lowest card.Deal additional cards." << endl;
					reSend(p1, p2);      //若最小纸牌构成tie，重发
					system("pause");
					system("cls");				
					
				    while (p1.hasPair(p1.handCard) || p2.hasPair(p2.handCard))        //同时 重发之后自身不能形成对子
					{
						gameInterface(p1, p2);
						cout << "A pair again. Deal additional cards" << endl;
						if (p1.hasPair(p1.handCard)) { 
							reSendToP(p1);                  //否则继续重发
						}
						if (p2.hasPair(p2.handCard)) {
							reSendToP(p2);                  //否则继续重发							
						}
						system("pause");
						system("cls");
					}
				}
			}
		}
		else {
			reFillCard();     //如果开始新一轮发牌时纸牌不够，则补充纸牌
		}
	}
	delete p;            //释放内存
	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
