#include "GameWindow.h"

GameWindow::GameWindow(QWidget *parent) : QWidget(parent) {
    highScoreWidget = new HighScoreWidget(this);
    scoreWidget = new ScoreWidget(this);
    levelWidget = new LevelWidget(this);
    livesWidget = new LivesWidget(this);
    gameMapWidget = new GameMapWidget(highScoreWidget, scoreWidget, livesWidget, levelWidget, this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topRowLayout = new QHBoxLayout;
	
	
	int highScore = readHighScore();
	highScoreWidget->updateHighScore(highScore);
	
    topRowLayout->addWidget(highScoreWidget);
    topRowLayout->addWidget(scoreWidget);
    topRowLayout->addWidget(levelWidget);
    topRowLayout->addWidget(livesWidget);

    mainLayout->addLayout(topRowLayout);

    mainLayout->addWidget(gameMapWidget);
    
    setFixedSize(600, 720);

    setLayout(mainLayout);
}

int GameWindow::readHighScore() {
    int highScore = 0;

    QFile file("highscore.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        QString scoreStr = stream.readAll();
        highScore = scoreStr.toInt();

        file.close();
    }

    return highScore;
}
