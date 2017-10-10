#include "game_model.h"
#include <algorithm>
#include <time.h>
//--全局变量--//
const int kMapRowCol = 15;
//-----------//


GameModel::GameModel()
{
	score = 0;//开始时积分为0
	dirNow = RIGHT;//开始时候头向右
	spdNow = NORMAL;//默认中等速度
	gameState = PLAYING;
	//调用构造函数时候先不进初始化.
}

GameModel::~GameModel()
{
	//删除蛇
	snakeFlesh.clear();
	//deleteOldFood();
}

void GameModel::move()
{
	//移动是链表头加一个,链表尾去一个,实现移动
	Block previousHead = snakeFlesh.front();
	Block newHead = previousHead;
	newHead.blockDir = dirNow;//新的头的方向,会马上变成身体,也就是身体的方向,"头部"在底层逻辑中是不存在的)
	//根据蛇移动的方向,判断新的头部要往哪个方向生成.
	if (dirNow == UP) { --newHead.YPos; }
	else if (dirNow == DOWN) { ++newHead.YPos; }
	else if (dirNow == LEFT) { --newHead.XPos; }
	else if (dirNow == RIGHT) { ++newHead.XPos; }

	//////////////////////////-------------------------------------------------
	if (dirPrevious == dirNow)
	{
		if (dirNow == LEFT || dirNow == RIGHT)
			previousHead.blockState = HORIZONTAL;
		else
			previousHead.blockState = VERTICAL;
	}
	else if ((dirPrevious == DOWN && dirNow == LEFT)
		|| (dirPrevious == RIGHT && dirNow == UP))//左上拐角
	{
		previousHead.blockState = UP_LEFT;
	}
	else if ((dirPrevious == LEFT && dirNow == UP)
		|| (dirPrevious == DOWN && dirNow == RIGHT))//右上拐角
	{
		previousHead.blockState = UP_RIGHT;
	}
	else if ((dirPrevious == UP && dirNow == LEFT)
		|| (dirPrevious == RIGHT && dirNow == DOWN))//左下角
	{
		previousHead.blockState = DOWN_LEFT;
	}
	else if ((dirPrevious == UP && dirNow == RIGHT)
		|| (dirPrevious == LEFT && dirNow == DOWN))//右下角
	{
		previousHead.blockState = DOWN_RIGHT;
	}
	/////////////////////////---------------------------------------------------
	dirPrevious = dirNow;
	//注:dirPrevious只记录一个tick前的dir
	//添加新的头部
	//注,这里改变的是前一个头结点的blockState.
	//因为newHead被绘制成头(特殊图形),因此不需要blockState.
	snakeFlesh.pop_front();//把改变后的previousHEAD push回去
	snakeFlesh.push_front(previousHead);
	snakeFlesh.push_front(newHead);

	//这段留一下再写,
	//如果这时候头部跟食物,重合,就是吃到了,
	//所以不删除尾巴,实现蛇的变长.
	//否则,就删除尾巴.
	if (snakeFlesh.front().XPos == foodXPos
		&& snakeFlesh.front().YPos == foodYPos)
	{ makeNewFood(); score++; }
	else
	{ snakeFlesh.pop_back(); }
		
	//每一次移动之后都检查是否撞墙.
	checkBump();
}

void GameModel::startGame(Speed spd)
{
	snakeFlesh.clear();
	score = 0;
	dirNow = RIGHT;
	dirPrevious = RIGHT;
	spdNow = spd;
	Block temp;//
	temp.XPos = 9;temp.YPos = 7;//现在是头在地图的中间偏右二格
	temp.blockDir = RIGHT;//开始时每一块都是向右侧的
	temp.blockState = HORIZONTAL;
	for (int i = 0; i < 5;++i)
	{
		snakeFlesh.push_back(temp);
		--temp.XPos;
	}
	//生成长度为5的蛇
	makeNewFood();
	//生成新的食物
	gameState = PLAYING;
	//按p键开始游戏
}

void GameModel::restartGame()
{
	startGame(spdNow);
}

bool GameModel::isBumpSelf()
{
	//i = 1是略过现在的头部 
	//在除了头部以外的整个蛇身中遍历
	auto it = snakeFlesh.cbegin();
	++it;
	for (; it!=snakeFlesh.cend();++it)
	{
		//如果头和身体任意部位重合
		if (it->XPos == snakeFlesh.front().XPos
			&& it->YPos == snakeFlesh.front().YPos)
		{
			return true;
		}
	}
	return false;
}

void GameModel::checkBump()
{
	//如果没在界内
	
	if (!(snakeFlesh.front().XPos >= 0
		&& snakeFlesh.front().XPos < kMapRowCol
		&& snakeFlesh.front().YPos >= 0
		&& snakeFlesh.front().YPos < kMapRowCol))
		bump();//发生碰撞.
	//或者撞到自己的身体
	if (isBumpSelf())
		bump();//发生碰撞
}

void GameModel::changeDirection(Direction dir)
{
	dirPrevious = dirNow;//存储这时候的dir
	//注意,这里是不能180度转的.
	switch (dir)
	{
	case UP:
		if (dirNow != DOWN)
			dirNow = UP;
		break;
	case DOWN:
		if (dirNow != UP)
			dirNow = DOWN;
		break;
	case LEFT:
		if (dirNow != RIGHT)
			dirNow = LEFT;
		break;
	case RIGHT:
		if (dirNow != LEFT)
			dirNow = RIGHT;
		break;
	default:
		break;
	}
}

bool GameModel::isInBody(int X, int Y)
{
	for (auto i : snakeFlesh)
	{
		//食物跟蛇身体中任一元素重合
		if (i.XPos == X
			&& i.YPos == Y)
			return true;
	}
	return false;
}

void GameModel::makeNewFood()
{
	srand((unsigned)time(NULL));
	while(true)
	{
		//如果没成功生成就不断尝试,直至成功为止
		int X = rand() % kMapRowCol;
		int Y = rand() % kMapRowCol;
		if (!isInBody(X, Y))
		{
			//成功生成
			foodXPos = X;
			foodYPos = Y;
			break;
		}
	}//end while
}

void GameModel::bump()
{
	gameState = OVER;
	//snakeFlesh.clear();//不能在这里加入
}
