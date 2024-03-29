#include "sensor_f.h"
#include "robot.h"
#include <QtDebug>

Sensor_F::Sensor_F(int _range, Robot *_robot)
{
    range = _range;
    robot = _robot;
}

bool Sensor_F::checkSensor()
{
    QList<Wall* > walls;
    int x = robot->getX();
    int y = robot->getY();
    direction = robot->getDirection();
    walls = robot->getRobotWallList();
    for (int i = 0; i < walls.size(); i++)
    {
        if(direction == 1 && walls[i]->getOrientation() == true
                && y - walls[i]->getY() < range
                && y - walls[i]->getY() > 0
                && abs(walls[i]->getX()- x) < 10)
            return true;
        if(direction == 2 && walls[i]->getOrientation() == false
                && walls[i]->getX() - x < range
                && walls[i]->getX() - x > 0
                && abs(walls[i]->getY()- y) < 10)
            return true;
        if(direction == 4 && walls[i]->getOrientation() == true
                && walls[i]->getY() - y < range
                && walls[i]->getY() - y > 0
                && abs(walls[i]->getX()- x) < 10)
            return true;
        if(direction == 8 && walls[i]->getOrientation() == false
                && x - walls[i]->getX() < range
                && x - walls[i]->getX() > 0
                && abs(walls[i]->getY()- y) < 10)
            return true;
    }
    return false;
}
