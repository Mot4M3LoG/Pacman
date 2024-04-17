#ifndef REDGHOST_H
#define REDGHOST_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include "Entity.h"
#include "Ghost.h"
#include "PacMan.h"

class RedGhost : public Ghost {
	Q_OBJECT
public:
    RedGhost(PacMan* pacman);
    ~RedGhost() override;
	
	void move() override;
    void animate() override;
    
    void resetGhost() override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool isValidMove(QPointF key);
    void leaveHouse(QPointF pos);
    void correctGridPos(QPointF pos);
    char chooseDirection(QPointF target);
    
    QList<QList<QPixmap>> setupRedGhost();
    
    int sprite_cycle;
    PacMan* pacmanInstance;
    
    bool insideHouse;
    int portal_used;
    
    QPointF gridSpawnpoint;
};

#endif
