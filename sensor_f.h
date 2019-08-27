#ifndef SENSORF_H
#define SENSORF_H

#include <cstdint>
#include <QList>
#include "wall.h"

class Robot;

class Sensor_F
{
private:
    int range;
    Robot* robot;
    uint8_t direction;
public:
    Sensor_F(int _range, Robot* _robot);
    bool checkSensor();
};

#endif // SENSORF_H
