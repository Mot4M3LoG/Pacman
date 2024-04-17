#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>
#include "Entity.h"
#include "GameMap.h"

class PacMan : public Entity{
	Q_OBJECT
public:
    PacMan(QGraphicsItem *parent = nullptr);
    ~PacMan() override;

    void move() override;
    void animate() override;
    
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void changeMoveDirection(char key);
    bool isValidPosition(QPointF key);
    QList<QList<QPixmap>> setupPacMan();
    void resetPacMan();
    
    int sprite_cycle;
    int portal_used;
    
    bool isPowered;
    
signals:
    void pacmanMoved(QPointF newPosition);
    
protected:
	void keyPressEvent(QKeyEvent *event) override;
    
private:
	char curr_move;
	char buff_move;
};

#endif
