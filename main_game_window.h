#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainGameWindow.h"
#include "game_model.h"
#include <QKeyEvent>
#include <QPaintEvent>
#include <QLabel>

class GameModel;//前置声明游戏模型类

class MainGameWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainGameWindow(QWidget *parent = Q_NULLPTR);
	~MainGameWindow();
protected:
	virtual void paintEvent(QPaintEvent *event); //界面重绘
	virtual void keyPressEvent(QKeyEvent *event); //键盘控制
private:
	Ui::MainGameWindowClass *ui;

	GameModel *game;
	QLabel *scoreLabel;//计分数字
	QLabel *speedLabel;//当前速度
	QTimer *timer;//计时器,用来实现定时刷新屏幕.

	void handleGameState(GameModel *game);//裁判

	void start();
	void very_slow_entry() { entry(500); }
	void slow_entry() { entry(400); }
	void normal_entry(){ entry(300);}
	void fast_entry() { entry(200); }
	void very_fast_entry() { entry(100); }

	void pauseResume();
	void quit();

	void entry(int t);

	QAction *startAction;
	QAction *vsEntryAction;
	QAction *sEntryAction;
	QAction *nEntryAction;
	QAction *fEntryAction;
	QAction *vfEntryAction;
	QAction *quitAction;
	//QAction *pauseAction;

	void updateScore();
	void updateTimer();

	int tick;
	bool isPause;
};
