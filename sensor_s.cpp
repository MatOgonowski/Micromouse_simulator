#include "sensor_s.h"
#include "robot.h"

Sensor_S::Sensor_S(int _range, QString _name, Robot *_robot)
{
    range = _range;
    name = _name;
    robot = _robot;
}

bool Sensor_S::checkSensor(QList<Wall *> walls)
{
    int x = robot->getX();
    int y = robot->getY();
    direction = robot->getDirection();
    for (int i = 0; i < walls.size(); i++)
    {
        if(name == "right")
        {
            if(direction == 1 && walls[i]->getOrientation() == false
                    && walls[i]->getX() - x  < range
                    && walls[i]->getX() - x > 0
                    && abs(walls[i]->getY()- y) < 10)
                return true;
            if(direction == 2 && walls[i]->getOrientation() == true
                    && walls[i]->getY() - y < range
                    && walls[i]->getY() - y > 0
                    && abs(walls[i]->getX()- x) < 10)
                return true;
            if(direction == 4 && walls[i]->getOrientation() == false
                    && x - walls[i]->getX() < range
                    && x - walls[i]->getX() > 0
                    && abs(walls[i]->getY()- y) < 10)
                return true;
            if(direction == 8 && walls[i]->getOrientation() == true
                    && y - walls[i]->getY() < range
                    && y - walls[i]->getY() > 0
                    && abs(walls[i]->getX()- x) < 10)
                return true;
        }
        else
        {
            if(direction == 1 && walls[i]->getOrientation() == false
                    && x - walls[i]->getX() < range
                    && x - walls[i]->getX() > 0
                    && abs(walls[i]->getY()- y) < 10)
                return true;
            if(direction == 2 && walls[i]->getOrientation() == true
                    && y - walls[i]->getY() < range
                    && y - walls[i]->getY() > 0
                    && abs(walls[i]->getX()- x) < 10)
                return true;
            if(direction == 4 && walls[i]->getOrientation() == false
                    && walls[i]->getX() - x  < range
                    && walls[i]->getX() - x  > 0
                    && abs(walls[i]->getY()- y) < 10)
                return true;
            if(direction == 8 && walls[i]->getOrientation() == true
                    && walls[i]->getY() - y < range
                    && walls[i]->getY() - y  > 0
                    && abs(walls[i]->getX()- x) < 10)
                return true;
        }
    }
    return false;
}
