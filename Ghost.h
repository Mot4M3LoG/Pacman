#ifndef GHOST_H
#define GHOST_H

#include <QPointF>
#include <QVector>
#include "Entity.h"

class Ghost : public Entity {
public:
    Ghost();
    ~Ghost() override;

    void virtual move() = 0;
    void virtual animate() = 0;
    
    QPointF reverseVector(char direction);\
    char translateToDirection(QVector<QPointF> validMoves, int i, QVector<QPointF> possibleMoves);
    void virtual resetGhost() = 0;
    
    char curr_move;
	int gridPosX;
	int gridPosY;
	
	bool isFrightened;
	bool isDead;
};

#endif
