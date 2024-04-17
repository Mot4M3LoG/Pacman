#ifndef GAMEWIDGETS_H
#define GAMEWIDGETS_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QTimer>
#include "GameMap.h"
#include "Entity.h"
#include "PacMan.h"

class HighScoreWidget : public QWidget {
    Q_OBJECT
public:
    explicit HighScoreWidget(QWidget *parent = nullptr);
    int highscore;
    void writeHighScore(int highscore);
    
public slots:
    void updateHighScore(int newHighScore);

private:
    QLabel *highScoreLabel;
};

class ScoreWidget : public QWidget {
    Q_OBJECT
public:
    explicit ScoreWidget(QWidget *parent = nullptr);
    
    int score;
    
public slots:
	void updateScore(int score);

private:
	QLabel *scoreLabel;
};

class LevelWidget : public QWidget {
    Q_OBJECT
public:
    explicit LevelWidget(QWidget *parent = nullptr);
    
    int level;

public slots:
	void updateLevel();

private:
	QLabel *levelLabel;
};

class LivesWidget : public QWidget {
    Q_OBJECT
public:
    explicit LivesWidget(QWidget *parent = nullptr);
    
    int lives;
    
public slots:
	int updateLives(int value);

private:
	QLabel *livesLabel;
};





//---------------------

class GameMapWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameMapWidget(HighScoreWidget *hsWidget, ScoreWidget *scoreWidget, LivesWidget *livesWidget, LevelWidget *levelWidget, QWidget *parent = nullptr);
    void paintPebbles();
    void removePebble(int row, int col);
    
    GameMap *gameMap;
    ScoreWidget *scoreWidget; 
    LivesWidget *livesWidget;
    LevelWidget *levelWidget;
    HighScoreWidget *hsWidget;
    QVector<QVector<QGraphicsEllipseItem*>> pebbleItems;
    void setGhostsVulnerable(bool value);
    bool checkDeath();
    void resetLevel(int livesValue);

public slots:
    void updateGame();
    void checkCollision(QPointF pos);
    void updateScore(int newScore);
    void revertFrightenedState();

signals:
    void scoreUpdated(int newScore);
    void superPebbleEaten();
    
private:
	QTimer *gameTimer;
	QTimer* frightenedTimer; 
	
	
	QGraphicsScene *scene;
	QGraphicsView *view;
    
    QList<Entity*> entities;
};

#endif
