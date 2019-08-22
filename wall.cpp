#include "wall.h"


Wall::Wall(int _x, int _y, bool _orientation): length(40)
{
    orientation = _orientation;
    if(orientation == true)
    {
        x = _x + 20;
        y = _y;
    }
    else
    {
        x = _x;
        y = _y + 20;
    }
}
