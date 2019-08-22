#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>
#include <fstream>
#include <QList>
#include "wall.h"
#include "wall_g.h"
#include "robot.h"
#include "robotg.h"

namespace Ui {
class Simulation;
}
using namespace std;

class Simulation : public QObject
{
    Q_OBJECT
private:
    int window_h;
    int window_w;
    const uint8_t MASK_N= 1;
    const uint8_t MASK_W = 8;
    QTimer* timer;
    QGraphicsScene* scene;
    QGraphicsView* gview;
    QList<Wall*> wall_List;
    QList<Wall_G*> wall_G_List;
    Robot* robot;
    Robot_G* robot_g;
    Wall* wall;
    Wall_G* wall_g;
    QPushButton* start;
    QPushButton* stop;
    QComboBox* comboBox;
    QPushButton* addButton(int _x, int _y, QString _name);
    void addComboBox(int _x, int _y, QComboBox* comboBox);
    void loadMap(QString mapName);
public:
    QGraphicsScene* getScene() {return scene;}
     Simulation();
    ~Simulation();
     void show();
     void stopTimer();
     QList<Wall* > getWalls(Robot* _robot);
private slots:
    void startClicked();
    void resetClicked();
    void update();
};

#endif // SIMULATION_H
