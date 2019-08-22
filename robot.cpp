#include "robot.h"
#include <QtDebug>
#include "simulation.h"


bool Robot::checkCenter()
{
    if((position[0] == 8 && position[1] == 8)
            ||(position[0] == 8 && position[1] == 9)
            ||(position[0] == 9 && position[1] == 8)
            ||(position[0] == 9 && position[1] == 9))
    {
        return true;
    }
    return false;
}

void Robot::spillWater()
{
    int map_temp[18][18];
    resetMap(WATER_MAP);
    int step = 0;
    for(int i =0; i < 18; i++)
    {
        for(int j = 0; j < 18; j++)
        {
            map_temp[i][j] = -1;
        }
    }
    map[8][8] = map_temp[8][8] = 0;
    map[8][9] = map_temp[8][9] = 0;
    map[9][8] = map_temp[9][8] = 0;
    map[9][9] = map_temp[9][9] = 0;

    while(1)
    {
        step++;
        for(int i = 1; i < 17; i++)
        {
            for(int j = 0; j < 17; j++)
            {
                if(map[i][j] != -1)
                {
                    continue;
                }
                if(highestNeighbourSector(i,j) != -1)
                {
                    map_temp[i][j] = step;
                }
            }
        }
        for(int i = 0; i < 18; i++)
        {
            for(int j = 0; j < 18; j++)
            {
                map[i][j] = map_temp[i][j];
            }
        }
        if(map[position[0]][position[1]] != -1)
        {
            break;
        }
    }
}

void Robot::resetMap(int type)
{
    for(int i = 0; i <18; i++)
    {
        for(int j = 0; j<18; j++)
        {
            if(type == WALL_MAP)
                wall_map[i][j] = 0;
            if(type == WATER_MAP)
                map[i][j] = -1;
        }
    }
}

void Robot::scanSector()
{
    walls_list = sim->getWalls(this);
    if(direction == 1)
    {
      d_north.front = sensor_f->checkSensor(walls_list);
      d_north.left = sensor_l->checkSensor(walls_list);
      d_north.right = sensor_r->checkSensor(walls_list);
      wall_map[position[0]][position[1]] = *(uint8_t*)&d_north | wall_map[position[0]][position[1]];
    }
    if(direction == 2)
    {
        d_east.front = sensor_f->checkSensor(walls_list);
        d_east.left = sensor_l->checkSensor(walls_list);
        d_east.right = sensor_r->checkSensor(walls_list);
        wall_map[position[0]][position[1]] = *(uint8_t*)&d_east | wall_map[position[0]][position[1]];
    }
    if(direction == 4)
    {
        d_south.front = sensor_f->checkSensor(walls_list);
        d_south.left = sensor_l->checkSensor(walls_list);
        d_south.right = sensor_r->checkSensor(walls_list);
        wall_map[position[0]][position[1]] = *(uint8_t*)&d_south | wall_map[position[0]][position[1]];
    }
    if(direction == 8)
    {
        d_west.front = sensor_f->checkSensor(walls_list);
        d_west.left = sensor_l->checkSensor(walls_list);
        d_west.right = sensor_r->checkSensor(walls_list);
        wall_map[position[0]][position[1]] = *(uint8_t*)&d_west | wall_map[position[0]][position[1]];
    }
}

void Robot::turn(uint8_t new_direction)
{
    direction = new_direction;
}

void Robot::findPath()
{
    int x = position[0];
    int y = position[1];
    int iter = 0;
    for(int i = 0; i < 256; i++)
    {
        path[i] = -1;
    }
    while (1)
    {
//        path[iter] = lowestNeighbourSector(x, y);
        if(path[iter] == 1)
        {
            y--;
        } else if(path[iter] == 2)
        {
            x++;
        } else if(path[iter] == 4)
        {
            y++;
        } else if(path[iter] == 8)
        {
            x--;
        }
        if(checkCenter())
        {
            break;
        }
        iter++;
    }
}

uint8_t Robot::choiceDirection()
{
    uint8_t new_direction = 0;
    uint8_t wallls_position = wall_map[position[0]][position[1]];
    if(map[position[0] - 1][position[1]] < map[position[0]][position[1]] && (wallls_position & MASK_N) == 0 && map[position[0] - 1][position[1]]!= -1 )
    {
        new_direction = 1;
    }
    if(map[position[0]][position[1] + 1] < map[position[0]][position[1]] && (wallls_position & MASK_E) == 0 && map[position[0]][position[1] + 1]!= -1)
    {
        new_direction = 2;
    }
    if(map[position[0] + 1][position[1]] < map[position[0]][position[1]] && (wallls_position & MASK_S) == 0 && map[position[0] + 1][position[1]] != -1)
    {
        new_direction = 4;
    }
    if(map[position[0]][position[1] - 1] < map[position[0]][position[1]] && (wallls_position & MASK_W) == 0 && map[position[0]][position[1] - 1] != -1)
    {
        new_direction = 8;
    }
    return new_direction;
}

int Robot::highestNeighbourSector(int i, int j)
{
    int a=-1;
    if((wall_map[i][j] & MASK_N) == 0 /*&& map[i][j] == -1*/) if(map[i - 1][j] > a) a = map[i - 1][j];
    if((wall_map[i][j] & MASK_E) == 0 /*&& map[i][j] == -1*/) if(map[i][j + 1] > a) a = map[i][j + 1];
    if((wall_map[i][j] & MASK_S) == 0 /*&& map[i][j] == -1*/) if(map[i + 1][j] > a) a = map[i + 1][j];
    if((wall_map[i][j] & MASK_W) == 0 /*&& map[i][j] == -1*/) if(map[i][j - 1] > a) a = map[i][j - 1];

    return a;
}


Robot::Robot(int _x, int _y, uint8_t _direction, Simulation *_sim)
{
    sim = _sim;
    x = _x;
    y = _y;
    direction = _direction;
    distance = 256;
    distance_temp = 0;
    sensor_f = new Sensor_F(30, this);
    sensor_r = new Sensor_S(30, "right", this);
    sensor_l = new Sensor_S(30, "left", this);
    resetMap(WATER_MAP);
    resetMap(WALL_MAP);
    for(int i = 1; i < 17; i++)
        wall_map[1][i] = 1 | wall_map[1][i];
    for(int i = 1; i < 17; i++)
        wall_map[16][i] = 4 | wall_map[16][i];
    for(int i = 1; i < 17; i++)
        wall_map[i][1] = 8 | wall_map[i][1];
    for(int i = 1; i < 17; i++)
        wall_map[i][16] = 2 | wall_map[i][16];
    position[0] = 16;
    position[1] = 1;
}

void Robot::move()
{
    if(direction == 1)
    {
        y -= 40;
        position[0]--;
    }
    if(direction == 2)
    {
        x += 40;
        position[1]++;
    }
    if(direction == 4)
    {
        y += 40;
        position[0]++;
    }
    if(direction == 8)
    {
        x -= 40;
        position[1]--;
    }
    distance_temp++;
}

void Robot::maping()
{
    uint8_t new_direction = 0;
    if(!checkCenter())
    {
        resetMap(WATER_MAP);
        scanSector();
        spillWater();
        new_direction = choiceDirection();
        if(new_direction != direction && new_direction != 0)
        {
            turn(new_direction);
        }
        else
        {
            move();
        }
    }
    else
    {
        if(distance_temp < distance)
        {
            distance = distance_temp;
        }
        else if(distance_temp == distance)
        {
//            sim->stopTimer();
        }
        position[0] = 16;
        position[1] = 1;
        x = 60;
        y = 660;
        distance_temp = 0;
    }
}
