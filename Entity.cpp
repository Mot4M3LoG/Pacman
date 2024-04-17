#include "Entity.h"

Entity::Entity(QGraphicsItem* parent): QObject(), QGraphicsPixmapItem(parent) {
	this->position = QPointF(100, 100);
}

Entity::~Entity(){}

void Entity::setPos(QPointF point){
	this->position = point;
}

void Entity::animate(){}

void Entity::move(){}

