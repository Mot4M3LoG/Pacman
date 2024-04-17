#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QList>
#include <QPixmap>
#include <QTimer>
#include <iostream>
#include "GameMap.h"
#include "GameWindow.h"
#include "Ghost.h"
#include "GameWidgets.h"
#include "Entity.h"
#include "PacMan.h"
#include "RedGhost.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	
	GameWindow gameMapWindow;
    gameMapWindow.show();

    return a.exec();
}
