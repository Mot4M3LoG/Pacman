#include "RedGhost.h"
#include "GameMap.h"
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <QVector>

#define SPRITE_SPEED 2000

#define GHOSTSIZE 20
#define HALF_GHOST 10
#define CELL_SIZE 20

#define PORTAL_CD 50

#define DEAD_LIST 5
#define FRIGHT_LIST 4
#define LEFT_LIST 3
#define DOWN_LIST 2
#define RIGHT_LIST 1
#define UP_LIST 0

RedGhost::RedGhost(PacMan* pacman) : pacmanInstance(pacman){
	this->position = QPointF(260, 280);
    this->curr_move = 'W';
    this->portal_used = 0;
    this->sprite_cycle = 0;
    this->insideHouse = true;
    this->gridSpawnpoint = QPointF(14, 11);
    
    this->sprites = setupRedGhost();
    
    this->gridPosX = 14;
    this->gridPosY = 15;
    
    QPixmap basepng("UP_1.png");
    setPixmap(basepng.scaled(GHOSTSIZE, GHOSTSIZE));
}

void RedGhost::move(){
	QPointF pacmanPosition = pacmanInstance->position;  
	QPointF ghostPosition = this->position;
	QPointF new_pos = ghostPosition;
	
	if(!insideHouse){
		if (this->isDead == false){
			this->curr_move = chooseDirection(pacmanPosition);
		}
		else{
			this->curr_move = chooseDirection(QPointF(gridSpawnpoint.rx()*20, (gridSpawnpoint.ry()-2)*20));
			if ((gridPosX == gridSpawnpoint.rx() || gridPosX == gridSpawnpoint.rx() + 2) && gridPosY == gridSpawnpoint.ry()){
				this->isDead = false;
			}
		}
	}
	else{
		leaveHouse(position);
	}
	
	correctGridPos(position);
	
	if (curr_move == 'W') {
		new_pos.ry() = new_pos.ry() - 1; // Move up
    } else if (curr_move == 'S') {
        new_pos.ry() = new_pos.ry() + 1; // Move down
    } else if (curr_move == 'A') {
        new_pos.rx() = new_pos.rx() - 1; // Move left
    } else if (curr_move == 'D') {
        new_pos.rx() = new_pos.rx() + 1; // Move right
    }
    
    if ((gridPosY == 14 && gridPosX == 27) && portal_used == 0){
		gridPosX = 0;
		new_pos = QPointF(5, new_pos.ry());
		portal_used = PORTAL_CD;
	}
	
	if ((gridPosY == 14 && gridPosX == 0) && portal_used == 0){
		gridPosX = 27;
		new_pos = QPointF(605, new_pos.ry());
		portal_used = PORTAL_CD;
	}
    
    position = new_pos;
    setPos(new_pos);
    
    if (portal_used < PORTAL_CD && portal_used > 0){
		portal_used--;
	}
}

void RedGhost::leaveHouse(QPointF pos){
	int grid_x = (static_cast<int>(pos.rx())+HALF_GHOST) / CELL_SIZE; 
    int grid_y = (static_cast<int>(pos.ry())+HALF_GHOST) / CELL_SIZE;
    
    if (grid_x >= 0 && grid_x < MAP_WIDTH && grid_y >= 0 && grid_y < MAP_HEIGHT) {
        if (pacman_map[grid_y][grid_x] == 0) {
            this->insideHouse = false;
            curr_move = 'W';
        }
    }
}

void RedGhost::correctGridPos(QPointF pos){
	int grid_x = (static_cast<int>(pos.rx())+HALF_GHOST) / CELL_SIZE; 
    int grid_y = (static_cast<int>(pos.ry())+HALF_GHOST) / CELL_SIZE;
    
	int centerGridX = grid_x * CELL_SIZE + HALF_GHOST;
	int centerGridY = grid_y * CELL_SIZE + HALF_GHOST;
    
    if (grid_x >= 0 && grid_x < MAP_WIDTH && grid_y >= 0 && grid_y < MAP_HEIGHT) {
		this->gridPosX = grid_x;
		this->gridPosY = grid_y;
	}
	
	if (this->curr_move == 'W' || this->curr_move == 'S'){
		this->position = QPointF(gridPosX * CELL_SIZE + HALF_GHOST-1, centerGridY);
	}
	else{
		this->position = QPointF(centerGridX, gridPosY * CELL_SIZE + HALF_GHOST-1);
	}
}

char RedGhost::chooseDirection(QPointF target){
	QVector<QPointF> possibleDirections = {
		QPointF(-1, 0), //A
		QPointF(1, 0),  //D
		QPointF(0, 1),  //S
		QPointF(0, -1)  //W
	};
	
	QVector<float> distances;
	QVector<QPointF> validMoves;
	
	for (const auto& direction : possibleDirections){
		
		QPointF reverse = reverseVector(curr_move);
		
		if (direction == reverse){
			continue;
		}
		
		QPointF newPosition = QPointF(position + direction);
		
		int newXpos = newPosition.rx();
		int newYpos = newPosition.ry();
		
		if (isValidMove(direction)){
			validMoves.append(direction);
			distances.append((newXpos-target.rx()) * (newXpos-target.rx()) + (newYpos-target.ry()) * (newYpos-target.ry()));
		}
		
	}
	if (this->isFrightened == false){
		float minDistance = 999999;
		for (int i = 0; i < validMoves.size(); i++){
			if (distances.at(i) < minDistance){
				curr_move = translateToDirection(validMoves, i, possibleDirections);
				minDistance = distances.at(i);
			}
		}
	}
	else{
		int validMovesNo = validMoves.size();
		
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		int randomMoveIndex = std::rand() % validMovesNo;
		
		curr_move = translateToDirection(validMoves, randomMoveIndex, possibleDirections);
	}
	
	return curr_move;
}

bool RedGhost::isValidMove(QPointF move_dir){
	int move_dirX = move_dir.rx();
	int move_dirY = move_dir.ry();
	
	return (pacman_map[gridPosY+move_dirY][gridPosX + move_dirX] != 1 && pacman_map[gridPosY+move_dirY][gridPosX + move_dirX] != 4);
}

void RedGhost::animate() {
	if (isDead){
		setPixmap(this->sprites.at(DEAD_LIST).at((this->sprite_cycle++)/SPRITE_SPEED));
	}else if(isFrightened){
		setPixmap(this->sprites.at(FRIGHT_LIST).at((this->sprite_cycle++)/(SPRITE_SPEED/2)));
	}else if (curr_move == 'W'){
		setPixmap(this->sprites.at(UP_LIST).at((this->sprite_cycle++)/(SPRITE_SPEED/2)));
	}else if(curr_move == 'D'){
		setPixmap(this->sprites.at(RIGHT_LIST).at((this->sprite_cycle++)/(SPRITE_SPEED/2)));
	}else if(curr_move == 'S'){
		setPixmap(this->sprites.at(DOWN_LIST).at((this->sprite_cycle++)/(SPRITE_SPEED/2)));
	}else if(curr_move == 'A'){
		setPixmap(this->sprites.at(LEFT_LIST).at((this->sprite_cycle++)/(SPRITE_SPEED/2)));
	}
    
    if (this->sprite_cycle >= SPRITE_SPEED){
		this->sprite_cycle = 0;
	}
}

QRectF RedGhost::boundingRect() const {
    return QRectF(0, 0, GHOSTSIZE, GHOSTSIZE);
}

void RedGhost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
    Q_UNUSED(widget);
	this->animate();
    painter->drawPixmap(position, pixmap());
}

QList<QList<QPixmap>> RedGhost::setupRedGhost(){
	
	QList<QList<QPixmap>> full_list;
	
	QPixmap sprite01("UP_1.png"); 
	QPixmap sprite02("UP_2.png");
	
	QList<QPixmap> list1;
	list1 << sprite01.scaled(GHOSTSIZE, GHOSTSIZE) << sprite02.scaled(GHOSTSIZE, GHOSTSIZE);
	
	QPixmap sprite11("RIGHT_1.png");
	QPixmap sprite12("RIGHT_2.png");
	
	QList<QPixmap> list2;
	list2 << sprite11.scaled(GHOSTSIZE, GHOSTSIZE) << sprite12.scaled(GHOSTSIZE, GHOSTSIZE);
	
	QPixmap sprite21("DOWN_1.png");
	QPixmap sprite22("DOWN_2.png");
	
	QList<QPixmap> list3;
	list3 << sprite21.scaled(GHOSTSIZE, GHOSTSIZE) << sprite22.scaled(GHOSTSIZE, GHOSTSIZE);
	
	QPixmap sprite31("LEFT_1.png");
	QPixmap sprite32("LEFT_2.png");
	
	QList<QPixmap> list4;
	list4 << sprite31.scaled(GHOSTSIZE, GHOSTSIZE) << sprite32.scaled(GHOSTSIZE, GHOSTSIZE);
	
	QPixmap sprite41("FRIGHTENED_1.png");
	QPixmap sprite42("FRIGHTENED_3.png");
	
	QList<QPixmap> list5;
	list5 << sprite41.scaled(GHOSTSIZE, GHOSTSIZE) << sprite42.scaled(GHOSTSIZE, GHOSTSIZE);
	
	QPixmap sprite51("eyes.png");
	
	QList<QPixmap> list6;
	list6 << sprite51.scaled(GHOSTSIZE, GHOSTSIZE);
	
	full_list << list1 << list2 << list3 << list4 << list5 << list6;
	return full_list;
}

void RedGhost::resetGhost(){
	this->position = QPointF(260, 280);
    this->curr_move = 'W';
    this->portal_used = 0;
    this->sprite_cycle = 0;
    this->insideHouse = true;
    
    this->gridPosX = 14;
    this->gridPosY = 15;
    this->isDead = false;
}

RedGhost::~RedGhost(){
}
