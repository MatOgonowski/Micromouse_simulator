#include "robot.h"
#include <QtDebug>
#include "simulation.h"


bool Robot::checkCenter()
{
    if((currentPosition[0] == 8 && currentPosition[1] == 8)
            ||(currentPosition[0] == 8 && currentPosition[1] == 9)
            ||(currentPosition[0] == 9 && currentPosition[1] == 8)
            ||(currentPosition[0] == 9 && currentPosition[1] == 9))
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
        if(map[currentPosition[0]][currentPosition[1]] != -1)
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
      d_north.front = sensor_f->checkSensor();
      d_north.left = sensor_l->checkSensor();
      d_north.right = sensor_r->checkSensor();
      wall_map[currentPosition[0]][currentPosition[1]] = *(uint8_t*)&d_north | wall_map[currentPosition[0]][currentPosition[1]];
    }
    if(direction == 2)
    {
        d_east.front = sensor_f->checkSensor();
        d_east.left = sensor_l->checkSensor();
        d_east.right = sensor_r->checkSensor();
        wall_map[currentPosition[0]][currentPosition[1]] = *(uint8_t*)&d_east | wall_map[currentPosition[0]][currentPosition[1]];
    }
    if(direction == 4)
    {
        d_south.front = sensor_f->checkSensor();
        d_south.left = sensor_l->checkSensor();
        d_south.right = sensor_r->checkSensor();
        wall_map[currentPosition[0]][currentPosition[1]] = *(uint8_t*)&d_south | wall_map[currentPosition[0]][currentPosition[1]];
    }
    if(direction == 8)
    {
        d_west.front = sensor_f->checkSensor();
        d_west.left = sensor_l->checkSensor();
        d_west.right = sensor_r->checkSensor();
        wall_map[currentPosition[0]][currentPosition[1]] = *(uint8_t*)&d_west | wall_map[currentPosition[0]][currentPosition[1]];
    }
}

uint8_t Robot::choiceDirection()
{
    uint8_t new_direction = 0;
    uint8_t wallls_position = wall_map[currentPosition[0]][currentPosition[1]];
    if( map[currentPosition[0] - 1][currentPosition[1]] < map[currentPosition[0]][currentPosition[1]]
            && (wallls_position & MASK_N) == 0
            && map[currentPosition[0] - 1][currentPosition[1]]!= -1 )
    {
        new_direction = 1;
    }
    if(map[currentPosition[0]][currentPosition[1] + 1] < map[currentPosition[0]][currentPosition[1]]
            && (wallls_position & MASK_E) == 0
            && map[currentPosition[0]][currentPosition[1] + 1]!= -1)
    {
        new_direction = 2;
    }
    if(map[currentPosition[0] + 1][currentPosition[1]] < map[currentPosition[0]][currentPosition[1]]
            && (wallls_position & MASK_S) == 0
            && map[currentPosition[0] + 1][currentPosition[1]] != -1)
    {
        new_direction = 4;
    }
    if(map[currentPosition[0]][currentPosition[1] - 1] < map[currentPosition[0]][currentPosition[1]]
            && (wallls_position & MASK_W) == 0
            && map[currentPosition[0]][currentPosition[1] - 1] != -1)
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

void Robot::turnRight()
{
    if(direction !=8)
    {
        direction <<=1;
    }
    else
    {
        direction >>=3;
    }
}

void Robot::turnLeft()
{
    if(direction !=1)
    {
        direction >>=1;
    }
    else
    {
        direction <<=3;
    }
}

void Robot::turnBack()
{
    if (direction != 4 && direction != 8)
    {
        direction <<=2;
    }
    else if (direction == 4)
    {
        direction = 1;
    }
    else
    {
        direction = 2;
    }
}


Robot::Robot(int _p1, int _p2, uint8_t _direction, Simulation *_sim)
{
    srand(time(0));
    sim = _sim;
    startPosition[0] = _p1;
    startPosition[1] = _p2;
    currentPosition[0] = startPosition[0];
    currentPosition[1] = startPosition[1];
    x = startPosition[1] * 40 + 20;
    y = startPosition[0] * 40 + 20;
    direction = _direction;
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
}

void Robot::move()
{
    if(direction == 1)
    {
        y -= 40;
        currentPosition[0]--;
    }
    if(direction == 2)
    {
        x += 40;
        currentPosition[1]++;
    }
    if(direction == 4)
    {
        y += 40;
        currentPosition[0]++;
    }
    if(direction == 8)
    {
        x -= 40;
        currentPosition[1]--;
    }
}

void Robot::maping()
{
    uint8_t new_direction = 0;
    if(!checkCenter())
    {
        resetMap(WATER_MAP);
//        scanSector();
        walls_list = sim->getWalls(this);
        if(direction == 1)
            scanSector2<north>();
        if(direction == 2)
            scanSector2<east>();
        if(direction == 4)
            scanSector2<south>();
        if(direction == 8)
            scanSector2<west>();
        spillWater();
        new_direction = choiceDirection();
        if(new_direction != direction && new_direction != 0)
        {
            direction = new_direction;
        }
        else
        {
            move();
        }
    }
    else
    {
        currentPosition[0] = startPosition[0];
        currentPosition[1] = startPosition[1];
        x = startPosition[1] * 40 + 20;
        y = startPosition[0] * 40 + 20;
    }
}

void Robot::reset()
{
    resetMap(WATER_MAP);
    resetMap(WALL_MAP);
    currentPosition[0] = startPosition[0];
    currentPosition[1] = startPosition[1];
    x = startPosition[1] * 40 + 20;
    y = startPosition[0] * 40 + 20;
    direction = 1;
}

void Robot::wallFollower()
{
    walls_list = sim->getWalls(this);
    if(!checkCenter())
    {
        if(!sensor_r->checkSensor())
        {
            turnRight();
        }
        else if(!sensor_f->checkSensor())
        {

        }
        else if (!sensor_l->checkSensor())
        {
            turnLeft();
        }
        else
        {
            turnBack();
        }
        move();
    }
}

void Robot::bruteForce()
{
    if(!checkCenter())
    {
        walls_list = sim->getWalls(this);
        int n = 0;
        int temp[3];
        if(!sensor_f->checkSensor())
        {
            temp[n] = 1;
            n++;
        }
        if(!sensor_r->checkSensor())
        {
            temp[n] = 2;
            n++;
        }
        if(!sensor_l->checkSensor())
        {
            temp[n] = 4;
            n++;
        }
        if(n == 0)
        {
            turnBack();
        }
        else if(n == 1)
        {
            if(temp[0] == 4)
            {
                turnLeft();
            }
            if(temp[0] == 2)
            {
                turnRight();
            }
        }
        else if (n > 1)
        {
            int a = rand()%n;
            if(temp[a] == 4)
            {
                turnLeft();
            }
            if(temp[a] == 2)
            {
                turnRight();
            }
        }
        move();
    }
}


