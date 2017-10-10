#include "MainGameWindow.h"
#include "game_model.h"
#include <qstring.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qmenubar.h>
#include <qcoreapplication.h>

//---全局常量---///
const int RowCol = 15;
const int blockSize = 16;
const int offset = 8;//边框
const int spaceY = 40;//上边空白,计分板
//-------------///


MainGameWindow::MainGameWindow(QWidget *parent)
	: QMainWindow(parent),ui(new Ui::MainGameWindowClass)
{
	ui->setupUi(this);
	
	setWindowTitle(tr("Snake v1.0"));

	startAction = new QAction(tr("start"), this);
	connect(startAction, &QAction::triggered, this, &MainGameWindow::start);
	//选择速度
	vsEntryAction = new QAction(tr("very slow"), this);
	sEntryAction = new QAction(tr("slow"), this);
	nEntryAction = new QAction(tr("normal"), this);
	fEntryAction = new QAction(tr("fast"), this);
	vfEntryAction = new QAction(tr("very fast"), this);
	connect(vsEntryAction, &QAction::triggered, this, &MainGameWindow::very_slow_entry);
	connect(sEntryAction, &QAction::triggered, this, &MainGameWindow::slow_entry);
	connect(nEntryAction, &QAction::triggered, this, &MainGameWindow::normal_entry);
	connect(fEntryAction, &QAction::triggered, this, &MainGameWindow::fast_entry);
	connect(vfEntryAction, &QAction::triggered, this, &MainGameWindow::very_fast_entry);
	//
	quitAction = new QAction(tr("quit"), this);
	connect(quitAction, &QAction::triggered, this, &MainGameWindow::quit);

	//生成菜单
	QMenu *spdMenu = menuBar()->addMenu(tr("&Speed"));
	spdMenu->addActions({ vsEntryAction,sEntryAction,nEntryAction,fEntryAction,vfEntryAction });

	QMenu *gameMenu = menuBar()->addMenu(tr("&Game"));
	gameMenu->addActions({ startAction,quitAction });

	//创建计分和速度数字标签
	speedLabel = new QLabel(this);
	scoreLabel = new QLabel(this);

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &MainGameWindow::updateTimer);
	
	game = new GameModel;
	//创建游戏,初始化游戏,设置好参数,默认是中等速度,启动计时器为中等速度(300ms)
	tick = 300;
	//默认中等进入游戏
	isPause = false;
	entry(tick);//开始游戏
	pauseResume();//isPause变成了true,现在是暂停状态
}

MainGameWindow::~MainGameWindow()
{
	//清理
	delete game; game = NULL;
	delete timer;timer = NULL;
	delete speedLabel;speedLabel = NULL;
	delete scoreLabel;scoreLabel = NULL;
	delete ui;
}

void MainGameWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	//:/MainGameWindow/Resources/bmpGround.bmp // 地面
	//:/MainGameWindow/Resources/bmpSnakeFlesh.bmp // 蛇身体
	//:/MainGameWindow/Resources/food.jpg // 食品
	//0位是身,1-4位分别是头上下左右,5-8位尾巴上下左右,9身体上下 10 身体左右 11-14 左上角右上角左下角右下角
	QPixmap bmpGround(":/MainGameWindow/Resources/bmpGround.bmp");
	QPixmap bmpSnakeFlesh(":/MainGameWindow/Resources/snakePic");
	QPixmap bmpFood(":/MainGameWindow/Resources/food.jpg");

	//先绘制背景
	painter.drawPixmap(offset, spaceY + offset, bmpGround);
	//绘制完毕
	//绘制蛇,从蛇的头部开始遍历以一一绘制
	for (auto it = game->snakeFlesh.cbegin();it != game->snakeFlesh.cend();++it)
	{
		if (it == game->snakeFlesh.cbegin())//如果是头
		{
			switch (game->dirNow)//根据移动方向绘制蛇头方向
			{
			case UP:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 1, 0, blockSize, blockSize);
				break;
			case DOWN:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 2, 0, blockSize, blockSize);
				break;
			case LEFT:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 3, 0, blockSize, blockSize);
				break;
			case RIGHT:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 4, 0, blockSize, blockSize);
				break;
			default:
				break;
			}
		}

		else if (it == --game->snakeFlesh.cend())//如果是尾巴
		{
			auto itFront = it;
			itFront--; ///高能!!!!!!!!!!!!!!!必须让尾巴方向跟着尾巴前边那一块变动!否则会造成身/尾在拐弯时候不连接!!!
			switch (itFront->blockDir)//根据移动方向绘制蛇头方向
			{
			case UP:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 5, 0, blockSize, blockSize);
				break;
			case DOWN:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 6, 0, blockSize, blockSize);
				break;
			case LEFT:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 7, 0, blockSize, blockSize);
				break;
			case RIGHT:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 8, 0, blockSize, blockSize);
				break;
			default:
				break;
			}
		}
		//这里还应检测每一个节点的前一个,后一个状态是否不同 
		//相同的话直接绘制平直的蛇身
		//不同的话绘制拐点
		else//身体中段
		{
			switch (it->blockState)
			{
			case VERTICAL:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 9, 0, blockSize, blockSize);
				break;
			case HORIZONTAL:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 10, 0, blockSize, blockSize);
				break;
			case DOWN_RIGHT:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 11, 0, blockSize, blockSize);
				break;
			case DOWN_LEFT:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 12, 0, blockSize, blockSize);
				break;
			case UP_RIGHT:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 13, 0, blockSize, blockSize);
				break;
			case UP_LEFT:
				painter.drawPixmap(offset + it->XPos*blockSize, offset + spaceY + it->YPos*blockSize, bmpSnakeFlesh, blockSize * 14, 0, blockSize, blockSize);
				break;
			default:
				break;
			}
		}
	}

	//绘制食物
	//
	painter.drawPixmap(offset + game->foodXPos*blockSize, offset + spaceY + game->foodYPos*blockSize, bmpFood);

	//裁判
	handleGameState(game);
}

void MainGameWindow::handleGameState(GameModel *game)
{
	if (game->gameState == OVER)
	{
		timer->stop();
		QMessageBox::information(this, "Game Over!", "your score: " + QString::number(game->score));
		game->gameState = PLAYING; // 赶紧要改回正常状态，不然那个弹窗消失不了
	}
}

void MainGameWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Up:
		game->changeDirection(UP);
		break;
	case Qt::Key_Down:
		game->changeDirection(DOWN);
		break;
	case Qt::Key_Left:
		game->changeDirection(LEFT);
		break;
	case Qt::Key_Right:
		game->changeDirection(RIGHT);
		break;
	case Qt::Key_Space:
		pauseResume();
		break;
	default:
		break;
	}

	//这一步不能重绘...应该在每一个tick结束时重绘
}

void MainGameWindow::updateTimer()
{
	//计时器到时的时候,应该是走一步.
	game->move();
	//刷新计分板
	updateScore();
	//刷新屏幕.
	update();
}
void MainGameWindow::updateScore()
{
	//刷新计分板
	scoreLabel->setText("Score: " + QString::number(game->score));
}

void MainGameWindow::entry(int t)
{
	tick = t;
	if (t == 500) 
		game->startGame(VERY_SLOW);

	else if (t == 400) 
		game->startGame(SLOW);

	else if (t == 300) 
		game->startGame(NORMAL);

	else if (t == 200)
		game->startGame(FAST);

	else if (t == 100) 
		game->startGame(VERY_FAST);

	timer->start(tick);	
	//定义窗口大小(必须放在游戏创建之后后面,
	//该函数设置后大小不可变动,窗口强制重绘
	setFixedSize(blockSize * RowCol + offset * 2, blockSize * RowCol + offset * 2 + spaceY);
	speedLabel->setGeometry(20, spaceY / 2, 80, 20);
	speedLabel->setText("Speed: " + QString::number(game->spdNow + 1));

	scoreLabel->setGeometry(blockSize * RowCol + offset * 2 - 90, spaceY / 2, 80, 20);
	scoreLabel->setText("Score: " + QString::number(game->score));

	update();
}

void MainGameWindow::start()
{
	entry(tick);
	update();
}
void MainGameWindow::pauseResume()
{
	if (isPause)
		timer->start(tick);
	if (!isPause)
		timer->stop();
	isPause = !isPause;
}
void MainGameWindow::quit()
{
	QCoreApplication::quit();
}
