#ifndef ROBOTG_H
#define ROBOTG_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "robot.h"

class Robot_G : public QGraphicsPixmapItem
{
private:
    Robot* robot;
    QGraphicsScene* scene;
public:
    Robot_G(Robot* _robot, QGraphicsScene* _scene);
    void draw();
};

#endif // ROBOTG_H
