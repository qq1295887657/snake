#include "MainGameWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainGameWindow w;
	w.show();
	return a.exec();
}
