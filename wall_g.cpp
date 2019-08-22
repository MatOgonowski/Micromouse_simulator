#include "wall_g.h"

Wall_G::Wall_G(Wall *_wall, QGraphicsScene *_sim)
{
    wall=_wall;
    sim = _sim;
    if(wall->getOrientation())
    {
        gx = wall->getX() - 20;
        gy = wall->getY();
        line = sim->addLine(gx,gy,gx+wall->getLength(),gy);
    }
    else
    {
        gx = wall->getX();
        gy = wall->getY() - 20;
        line = sim->addLine(gx,gy,gx,gy+wall->getLength());
    }
    QPen pen(QColor(0,0,0));
    pen.setWidth(3);
    line->setPen(pen);
}
