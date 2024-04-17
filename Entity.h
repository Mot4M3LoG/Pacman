#ifndef ENTITY_H
#define ENTITY_H

#include <QList>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

class Entity : public QObject, public QGraphicsPixmapItem{
	Q_OBJECT
public:
    Entity(QGraphicsItem *parent = nullptr);
    virtual ~Entity();

    virtual void move();
    virtual void animate();
    void setPos(QPointF point);
    
    QList<QList<QPixmap>> sprites;
    QPointF position;
};

#endif
