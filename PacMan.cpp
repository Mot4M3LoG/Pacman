#include "PacMan.h"
#include "GameMap.h"
#include <QDebug>
#include <QGraphicsPixmapItem>

#define SPRITE_SPEED 2000

#define PACSIZE 20
#define HALF_PAC 10

#define PORTAL_CD 50

#define LEFT_LIST 3
#define DOWN_LIST 2
#define RIGHT_LIST 1
#define UP_LIST 0

PacMan::PacMan(QGraphicsItem *parent) : Entity(parent){
    this->position = QPointF(270, 460);
    this->curr_move = 'N';
    this->sprite_cycle = 0;
    this->portal_used = 0;
    this->isPowered = false;
    
    this->sprites = setupPacMan();
    
    QPixmap basepng("pac-full.png");
    setPixmap(basepng.scaled(PACSIZE, PACSIZE));
    
    setFlag(ItemIsFocusable);
    setFocus();
}

PacMan::~PacMan() {   
}

void PacMan::move() {
    QPointF new_position = position;
    
    if (buff_move != 'N'){
		if (buff_move == 'W') {
			new_position.ry() -= PACSIZE;
		} else if (buff_move  == 'S') {
			new_position.ry() += PACSIZE;
		} else if (buff_move  == 'A') {
			new_position.rx() -= PACSIZE;
		} else if (buff_move  == 'D') {
			new_position.rx() += PACSIZE;
		}
		
		if (isValidPosition(new_position)) {
        curr_move = buff_move;
        buff_move = 'N';
		}
		new_position = position;
	}
	
    if (curr_move == 'N'){
		curr_move = buff_move;
	}

    if (curr_move == 'W') {
        new_position.ry() -= 1; // Move up
    } else if (curr_move == 'S') {
        new_position.ry() += 1; // Move down
    } else if (curr_move == 'A') {
        new_position.rx() -= 1; // Move left
    } else if (curr_move == 'D') {
        new_position.rx() += 1; // Move right
    }
    
    
    if (isValidPosition(new_position)) {
		int grid_x = (static_cast<int>(new_position.rx()) + HALF_PAC) / PACSIZE; 
		int grid_y = (static_cast<int>(new_position.ry()) + HALF_PAC) / PACSIZE;
		
		if ((grid_y == 14 && grid_x == 27) && portal_used == 0){
			new_position = QPointF(10, new_position.ry());
			portal_used = PORTAL_CD;
		}
	
		if ((grid_y == 14 && grid_x == 0) && portal_used == 0){
			new_position = QPointF(610, new_position.ry());
			portal_used = PORTAL_CD;
		}
		
        setPos(new_position);
        position = new_position;
        
        emit pacmanMoved(position);
    }
    
    if (portal_used < PORTAL_CD && portal_used > 0){
		portal_used--;
	}
}

bool PacMan::isValidPosition(QPointF new_pos){
	int grid_x = (static_cast<int>(new_pos.rx()) + HALF_PAC) / PACSIZE; 
    int grid_y = (static_cast<int>(new_pos.ry()) + HALF_PAC) / PACSIZE;
    
    if (grid_x >= 0 && grid_x < MAP_WIDTH && grid_y >= 0 && grid_y < MAP_HEIGHT) {
        if (pacman_map[grid_y][grid_x] == 1 || pacman_map[grid_y][grid_x] == 4) {
            return false;
        }
    }
    return true;
}

void PacMan::animate() {
    if (curr_move == 'W'){
		setPixmap(this->sprites.at(UP_LIST).at((this->sprite_cycle++)/(SPRITE_SPEED/2)));
	}else if(curr_move == 'D'){
		setPixmap(this->sprites.at(RIGHT_LIST).at((this->sprite_cycle++)/(SPRITE_SPEED/2)));
	}else if(curr_move == 'S'){
		setPixmap(this->sprites.at(DOWN_LIST).at((this->sprite_cycle++)/(SPRITE_SPEED/2)));
	}else if(curr_move == 'A'){
		setPixmap(this->sprites.at(LEFT_LIST).at((this->sprite_cycle++)/(SPRITE_SPEED/2)));
	}
    
    if (this->sprite_cycle >= (SPRITE_SPEED*2)){
		this->sprite_cycle = 0;
	}
}

QRectF PacMan::boundingRect() const {
    return QRectF(0, 0, PACSIZE, PACSIZE);
}

void PacMan::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
    Q_UNUSED(widget);
	this->animate();
    painter->drawPixmap(position, pixmap());
}

void PacMan::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W) {
        this->changeMoveDirection('W');
    } else if (event->key() == Qt::Key_S) {
        this->changeMoveDirection('S');
    } else if (event->key() == Qt::Key_A) {
        this->changeMoveDirection('A');
    } else if (event->key() == Qt::Key_D) {
        this->changeMoveDirection('D');
    }

    update();
}

void PacMan::changeMoveDirection(char key){
	this->buff_move = key;
}

QList<QList<QPixmap>> PacMan::setupPacMan(){
	
	QList<QList<QPixmap>> full_list;
	
	QPixmap sprite01("pac-full.png");
	QPixmap sprite02("pac-34up.png");
	QPixmap sprite03("pac-12up.png");
	QPixmap sprite04("pac-34up.png");
	
	QList<QPixmap> list1;
	list1 << sprite01.scaled(PACSIZE, PACSIZE) << sprite02.scaled(PACSIZE, PACSIZE) << sprite03.scaled(PACSIZE, PACSIZE) << sprite04.scaled(PACSIZE, PACSIZE);
	
	QPixmap sprite11("pac-full.png");
	QPixmap sprite12("pac-34right.png");
	QPixmap sprite13("pac-12right.png");
	QPixmap sprite14("pac-34right.png");
	
	QList<QPixmap> list2;
	list2 << sprite11.scaled(PACSIZE, PACSIZE) << sprite12.scaled(PACSIZE, PACSIZE) << sprite13.scaled(PACSIZE, PACSIZE) << sprite14.scaled(PACSIZE, PACSIZE);
	
	QPixmap sprite21("pac-full.png");
	QPixmap sprite22("pac-34down.png");
	QPixmap sprite23("pac-12down.png");
	QPixmap sprite24("pac-34down.png");
	
	QList<QPixmap> list3;
	list3 << sprite21.scaled(PACSIZE, PACSIZE) << sprite22.scaled(PACSIZE, PACSIZE) << sprite23.scaled(PACSIZE, PACSIZE) << sprite24.scaled(PACSIZE, PACSIZE);
	
	QPixmap sprite31("pac-full.png");
	QPixmap sprite32("pac-34left.png");
	QPixmap sprite33("pac-12left.png");
	QPixmap sprite34("pac-34left.png");
	
	QList<QPixmap> list4;
	list4 << sprite31.scaled(PACSIZE, PACSIZE) << sprite32.scaled(PACSIZE, PACSIZE) << sprite33.scaled(PACSIZE, PACSIZE) << sprite34.scaled(PACSIZE, PACSIZE);
	
	full_list << list1 << list2 << list3 << list4;
	return full_list;
}

void PacMan::resetPacMan(){
	this->position = QPointF(270, 460);
    this->curr_move = 'N';
    this->sprite_cycle = 0;
    this->portal_used = 0;
    this->isPowered = 0;
}
