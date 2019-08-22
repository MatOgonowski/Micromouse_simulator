#include "robotg.h"
#include <QtDebug>

Robot_G::Robot_G(Robot* _robot, QGraphicsScene* _scene)
{
    robot= _robot;
    scene = _scene;
    setPixmap(QPixmap(":/micromouse_pic.png"));
    setPos(0,0);
    setPos(robot->getX() - 15, robot->getY() - 15);
}

void Robot_G::draw()
{
    setPos(robot->getX() - 15, robot->getY() - 15);
    setTransformOriginPoint(15, 15);
    if(robot->getDirection() == 1)
        setRotation(0);
    if(robot->getDirection() == 2)
        setRotation(90);
    if(robot->getDirection() == 4)
        setRotation(180);
    if(robot->getDirection() == 8)
        setRotation(-90);
}
