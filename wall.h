#ifndef WALL_H
#define WALL_H


class Wall
{
    int x;
    int y;
    bool orientation; //true poziomo, false pionowo
    const int length;
public:
    Wall(int _x, int _y, bool _orientation);
    int getX() {return x;}
    int getY() {return y;}
    bool getOrientation() {return orientation;}
    int getLength(){return length;}
};

#endif // WALL_H
