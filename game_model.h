#pragma once
#include <list>
using namespace std;
enum Direction
{
	UP,DOWN,LEFT,RIGHT
};
enum Speed
{
	VERY_SLOW, SLOW, NORMAL, FAST, VERY_FAST
};
enum GameState
{
	PLAYING,OVER
};
enum BlockState //头尾没加入,因为前端可以正确处理头尾
{
	HORIZONTAL,
	VERTICAL,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT
};
struct Block
{
	int XPos, YPos;
	Direction blockDir;
	BlockState blockState;
};
class GameModel
{
public:
	GameModel();
	~GameModel();
public:
	void startGame(Speed spd);
	void restartGame(); // 重新开始当前难度的游戏
public:
	void bump();
	void changeDirection(Direction dir);//由键盘监听模块传入方向.
	void move();//包含了吃
	void checkBump();//检查是否碰撞(撞墙/撞自己)
	bool isBumpSelf();//是否撞自己
	bool isInBody(int X, int Y); // 生成食物之前检查是否与蛇身体重合
	//x,y是食物预定的生成位置

	void makeNewFood();//生成食物

	int foodXPos, foodYPos;//当前的目标食物
	Direction dirNow;//蛇移动方向,这是一状态机
	Direction dirPrevious;//存储上一步的蛇移动方向,为了避免快速连续转弯而撞到自己 // 暂未使用
	Speed spdNow;//当前游戏速度
	list<Block> snakeFlesh;

	GameState gameState;//当前游戏状态.
	int score; // 积分.

};
