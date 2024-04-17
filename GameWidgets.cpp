#include <QDebug>
#include <QObject>
#include "GameWidgets.h"
#include "GameMap.h"
#include "Entity.h"
#include "PacMan.h"
#include "Ghost.h"
#include "RedGhost.h"

#define PEBBLE 2
#define PEBBLE_SIZE 5
#define SUPER_PEBBLE_SIZE 20
#define OFFSET 7
#define ENTITY_SIZE 20
#define HALF_ENTITY 10
#define CELL_SIZE 20
#define SUPER_PEBBLE 3
#define PEBBLE_POINTS 100
#define S_PEBBLE_POINTS 500
#define GHOST_POINTS 1000
#define DEATH_RANGE 400

void setupPacMan(PacMan *pac);

HighScoreWidget::HighScoreWidget(QWidget *parent) : QWidget(parent) {
    highScoreLabel = new QLabel("Highscore: 0", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(highScoreLabel);
    setLayout(layout);
}

void HighScoreWidget::updateHighScore(int newHighScore) {
    highScoreLabel->setText("HighScore: " + QString::number(newHighScore));
    highscore = newHighScore;
}

void HighScoreWidget::writeHighScore(int newHighest){
	QFile file("highscore.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        stream << newHighest;

        file.close();
    }
}

ScoreWidget::ScoreWidget(QWidget *parent) : QWidget(parent) {
	scoreLabel = new QLabel("Score: 0", this);
	
	QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(scoreLabel);
    setLayout(layout);
    
   this->score = 0;
}

void ScoreWidget::updateScore(int newScore) {
	this->score = this->score + newScore;
    scoreLabel->setText("Score: " + QString::number(this->score));
}

LivesWidget::LivesWidget(QWidget *parent) : QWidget(parent){
	livesLabel = new QLabel("Lives: 3", this);
	
	this->lives = 3;
	
	QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(livesLabel);
    setLayout(layout);
    
}

int LivesWidget::updateLives(int value) {
	lives = lives + value;
    livesLabel->setText("Lives: " + QString::number(lives));
    return lives;
}

LevelWidget::LevelWidget(QWidget *parent) : QWidget(parent){
	levelLabel = new QLabel("Level: 1", this);
	
	this->level = 1;
	
	QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(levelLabel);
    setLayout(layout);
}

void LevelWidget::updateLevel() {
    levelLabel->setText("Level: " + QString::number(level + 1));
}


// --------------------

GameMapWidget::GameMapWidget(HighScoreWidget* windowshsWidget, ScoreWidget* windowsScoreWidget, LivesWidget* windowsLivesWidget, LevelWidget* windowsLevelWidget, QWidget *parent) : QWidget(parent), scoreWidget(windowsScoreWidget), frightenedTimer(new QTimer(this)) {
	this->scoreWidget = windowsScoreWidget;
	this->livesWidget = windowsLivesWidget;
	this->levelWidget = windowsLevelWidget;
	this->hsWidget = windowshsWidget;
	this->gameMap = new GameMap;
	
	connect(frightenedTimer, &QTimer::timeout, this, &GameMapWidget::revertFrightenedState);
	frightenedTimer->setInterval(15000);
	
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(updateGame()));
    gameTimer->start(16); 

    scene = new QGraphicsScene;
    scene->setSceneRect(QRectF(0, 0, CELL_SIZE*MAP_WIDTH, CELL_SIZE*MAP_HEIGHT)); 
    
    view = new QGraphicsView(this);
    view->setScene(scene);
    
    view->setRenderHint(QPainter::SmoothPixmapTransform, true);
    view->setRenderHint(QPainter::Antialiasing, true);
    view->setRenderHint(QPainter::HighQualityAntialiasing, true);
    
    QPixmap backgroundImage("pacmanmap.jpg"); 
    scene->setBackgroundBrush(backgroundImage.scaled(CELL_SIZE*MAP_WIDTH, CELL_SIZE*MAP_HEIGHT));

    PacMan *pacman = new PacMan;
    entities.append(pacman); 
    
    connect(pacman, &PacMan::pacmanMoved, this, &GameMapWidget::checkCollision);
	connect(this, &GameMapWidget::scoreUpdated, scoreWidget, &ScoreWidget::updateScore);

    RedGhost *redG = new RedGhost(pacman);
    entities.append(redG);
    
    /*Ghost *ghost2 = new Ghost; //ghost appending
    entities.append(ghost2);*/
    
    pebbleItems.resize(MAP_HEIGHT);

    for (int row = 0; row < MAP_HEIGHT; row++) {
        pebbleItems[row].resize(MAP_WIDTH);
    }

    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            if (gameMap->pacManMap[row][col] == PEBBLE) {
                QGraphicsEllipseItem* pebbleItem = new QGraphicsEllipseItem(CELL_SIZE * col + OFFSET, CELL_SIZE * row + OFFSET, PEBBLE_SIZE, PEBBLE_SIZE);
                pebbleItem->setBrush(Qt::yellow);
                
                scene->addItem(pebbleItem);
                pebbleItems[row][col] = pebbleItem;
            }
            if (gameMap->pacManMap[row][col] == SUPER_PEBBLE) {
                QGraphicsEllipseItem* pebbleItem = new QGraphicsEllipseItem(CELL_SIZE * col, CELL_SIZE * row, SUPER_PEBBLE_SIZE, SUPER_PEBBLE_SIZE);
                pebbleItem->setBrush(Qt::red);
                
                scene->addItem(pebbleItem);
                pebbleItems[row][col] = pebbleItem;
            } 
        }
    }
    
    for (Entity *entity : entities) {
        scene->addItem(entity);
    }
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    setLayout(layout);
}

void GameMapWidget::updateGame() {
	if (checkDeath()){
		resetLevel(-1);
	}
	
	bool lvlcomplete = true;
	for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            if (gameMap->pacManMap[row][col] == PEBBLE || gameMap->pacManMap[row][col] == SUPER_PEBBLE){
				lvlcomplete = false;
			}
		}
    }
    
    if (lvlcomplete){
		levelWidget->updateLevel();
		resetLevel(0);
	}
	
    for (Entity *entity : entities) {
        entity->move();
    }
    
    scene->update();
    update();
}

void GameMapWidget::resetLevel(int livesValue){
	int life_check = livesWidget->updateLives(livesValue);
	if(life_check == 0){
		if (scoreWidget->score > hsWidget->highscore) {
			int highScore = scoreWidget->score;
			
			hsWidget->writeHighScore(highScore);

			qDebug() << "New highscore: " << highScore;
		} else {
			qDebug() << "Current score is not higher than the high score.";
		}
		
		qDebug() << "GAME OVER!";
		std::exit(10);
	}
	gameMap->resetMap();
	
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			if (gameMap->pacManMap[row][col] == PEBBLE || gameMap->pacManMap[row][col] == SUPER_PEBBLE){
				pebbleItems[row][col]->show();
			}
		}
	}
	for (Entity* entity : entities){
		if (PacMan* pacman = dynamic_cast<PacMan*>(entity)) {
			pacman->resetPacMan();
		}
		else{
			Ghost* ghost = dynamic_cast<Ghost*>(entity);
			ghost->resetGhost();
		}
	}
}

bool GameMapWidget::checkDeath(){
	PacMan* pacman = dynamic_cast<PacMan*>(entities.at(0));
	
	for (Entity* entity : entities){
		if (PacMan* check = dynamic_cast<PacMan*>(entity)){
			continue;
		}
		else if(abs((pacman->position.rx() - entity->position.rx()) * (pacman->position.rx() - entity->position.rx()) + (pacman->position.ry() - entity->position.ry()) * (pacman->position.ry() - entity->position.ry())) < DEATH_RANGE){
			Ghost* ghost = dynamic_cast<Ghost*>(entity);
			if(ghost->isDead){
				return false;
			}
			else if (pacman->isPowered){
				ghost->isDead = true;
				ghost->isFrightened = false;
				emit scoreUpdated(GHOST_POINTS);
				return false;
			}
			else{
				return true;
			}
		}
	}
	return false;
}

void GameMapWidget::checkCollision(QPointF pos) {
	int grid_x = (static_cast<int>(pos.rx()) + HALF_ENTITY) / ENTITY_SIZE; 
    int grid_y = (static_cast<int>(pos.ry()) + HALF_ENTITY) / ENTITY_SIZE;
    
	 if (grid_x >= 0 && grid_x < MAP_WIDTH && grid_y >= 0 && grid_y < MAP_HEIGHT) {
        if (gameMap->pacManMap[grid_y][grid_x] == PEBBLE) {
            gameMap->removePebble(grid_y, grid_x);
            this->pebbleItems[grid_y][grid_x]->hide();
			emit scoreUpdated(PEBBLE_POINTS);
        }
        if (gameMap->pacManMap[grid_y][grid_x] == SUPER_PEBBLE){
			gameMap->removePebble(grid_y, grid_x);
			this->pebbleItems[grid_y][grid_x]->hide();
			emit scoreUpdated(S_PEBBLE_POINTS);
			emit superPebbleEaten();
			
            setGhostsVulnerable(true);

            frightenedTimer->start();
		}
    }
}

void GameMapWidget::updateScore(int newScore) {
	if (scoreWidget) {
		scoreWidget->updateScore(newScore);
	}
}

void GameMapWidget::revertFrightenedState() {
    setGhostsVulnerable(false);
    frightenedTimer->stop();
}

void GameMapWidget::setGhostsVulnerable(bool value){
	for (Entity* entity : entities){
		if (Ghost* ghost = dynamic_cast<Ghost*>(entity)) {
			ghost->isFrightened = value;
		}
		else{
			PacMan* pacman = dynamic_cast<PacMan*>(entity);
			pacman->isPowered = value;
		}
	}
}


