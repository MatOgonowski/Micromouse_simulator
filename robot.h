#ifndef ROBOT_H
#define ROBOT_H

#include <cstdint>
#include <QList>
#include "wall.h"
#include "sensor_f.h"
#include "sensor_s.h"


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
    int position[2];
    int path[256];
    int distance, distance_temp;
    QList<Wall* > walls_list;
    Sensor_F* sensor_f;
    Sensor_S* sensor_r;
    Sensor_S* sensor_l;
    Simulation* sim;

    bool checkCenter();
    void spillWater();
    void resetMap(int type);
    void scanSector();
    void turn(uint8_t new_direction);
    void findPath();
    uint8_t choiceDirection();
    int highestNeighbourSector(int i, int j);

public:
    Robot(int _x, int _y, uint8_t _direction, Simulation* _sim);
    int getX() const {return x;}
    int getY() const {return y;}
    uint8_t getDirection() const {return direction;}
    void move();
    void maping();

};

#endif // ROBOT_H
