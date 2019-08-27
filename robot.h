#ifndef ROBOT_H
#define ROBOT_H

#include <cstdint>
#include <QList>
#include <cstdlib>
#include <ctime>
#include "wall.h"
#include "sensor_f.h"
#include "sensor_s.h"

// ZROBIC PORUSZANIE BRUTAL FORCE


struct north{
    uint8_t front :1;
    uint8_t right :1;
    uint8_t :1;
    uint8_t left :1;
    uint8_t :4;
};

struct east{
    uint8_t left :1;
    uint8_t front :1;
    uint8_t right :1;
    uint8_t :5;
};

struct south{
    uint8_t :1;
    uint8_t left :1;
    uint8_t front :1;
    uint8_t right :1;
    uint8_t :4;
};

struct west{
    uint8_t right :1;
    uint8_t :1;
    uint8_t left :1;
    uint8_t front :1;
    uint8_t :4;
};

class Simulation;

class Robot
{
private:
    int x;
    int y;
    const int WALL_MAP = 0;
    const int WATER_MAP = 1;
    const uint8_t MASK_N= 1;
    const uint8_t MASK_E = 2;
    const uint8_t MASK_S= 4;
    const uint8_t MASK_W = 8;
    north d_north;
    east d_east;
    south d_south;
    west d_west;
    uint8_t direction;
    uint8_t wall_map[18][18];
    int map[18][18];
    int currentPosition[2];
    int startPosition[2];
    QList<Wall* > walls_list;
    Sensor_F* sensor_f;
    Sensor_S* sensor_r;
    Sensor_S* sensor_l;
    Simulation* sim;

    bool checkCenter();
    void spillWater();
    void resetMap(int type);
    void scanSector();
    uint8_t choiceDirection();
    int highestNeighbourSector(int i, int j);
    void move();
    void turnRight();
    void turnLeft();
    void turnBack();
    template <typename T> void scanSector2()
    {
        T temp;
        temp.front = sensor_f->checkSensor();
        temp.left = sensor_l->checkSensor();
        temp.right = sensor_r->checkSensor();
        wall_map[currentPosition[0]][currentPosition[1]] = *(uint8_t*)&temp | wall_map[currentPosition[0]][currentPosition[1]];
    }

public:
    Robot(int _p1, int _p2, uint8_t _direction, Simulation* _sim);
    int getX() const {return x;}
    int getY() const {return y;}
    uint8_t getDirection() const {return direction;}
    void maping();
    void reset();
    QList<Wall* > getRobotWallList() const {return walls_list;}
    void wallFollower();
    void bruteForce();

};

#endif // ROBOT_H
