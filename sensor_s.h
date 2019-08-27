#ifndef SENSORS_H
#define SENSORS_H

#include <cstdint>
#include <QList>
#include "wall.h"

class Robot;

class Sensor_S
{
private:
    int range;
    QString name;
    Robot* robot;
    uint8_t direction;
public:
    Sensor_S(int _range, QString _name, Robot* _robot);
    bool checkSensor();
};

#endif // SENSORS_H
