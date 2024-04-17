#include "Ghost.h"

Ghost::Ghost() {
    this->position = QPointF(100, 100);
    this->isFrightened = false;
    this->isDead = false;
}

Ghost::~Ghost() {   
}

QPointF Ghost::reverseVector(char direction){
	if (direction == 'W'){
		return QPointF(0, 1);
	}
	else if (direction == 'S'){
		return QPointF(0, -1);
	}
	else if (direction == 'A'){
		return QPointF(1, 0);
	}
	else{
		return QPointF(-1, 0);
	}
}

char Ghost::translateToDirection(QVector<QPointF> validMoves, int i, QVector<QPointF> possibleMoves){
	if (validMoves.at(i) == possibleMoves.at(3)) {
		return 'W';
	} else if (validMoves.at(i) == possibleMoves.at(2)) {
		return 'S';
	} else if (validMoves.at(i) == possibleMoves.at(0)) {
		return 'A';
	} else{
		return 'D';
	}
}
