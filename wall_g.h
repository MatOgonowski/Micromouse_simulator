#ifndef WALLG_H
#define WALLG_H

#include "wall.h"
#include <QGraphicsLineItem>
#include <QGraphicsScene>

class Simulation;


class Wall_G : public QObject
{
private:
    Wall* wall;
    QGraphicsScene* sim;
    QGraphicsLineItem* line;
    int gx;
    int gy;
public:
    Wall_G(Wall* _wall, QGraphicsScene* _sim);
};

#endif // WALLG_H
