#include "simulation.h"
#include <QDebug>




void Simulation::loadMap(QString mapName)
{
    QString path = "../micromouse/maps/";
    QString name;
    name.append(path).append(mapName).append(".txt");
    ifstream file;
    file.open(name.toLatin1());
    if(!file.is_open())
    {
        qDebug() <<"BLAD ODCZYTU MAPY" <<endl;
    }
    int pom;
    int position_x = 40;
    int position_y = 40;
    for(int i =1; i<257; i++)
    {
        file >> pom;
        if(((uint8_t)pom & MASK_N)!=0)
        {
            wall = new Wall(position_x,position_y,true);
            wall_g = new Wall_G(wall, getScene());
            wall_List.append(wall);
            wall_G_List.append(wall_g);
        }
        if(((uint8_t)pom & MASK_W)!=0)
        {
            wall = new Wall(position_x,position_y,false);
            wall_g = new Wall_G(wall, getScene());
            wall_List.append(wall);
            wall_G_List.append(wall_g);
        }
        position_x +=40;
        if (i % 16 == 0)
        {
            position_y += 40;
            position_x = 40;
        }
    }
    position_x = 40;
    position_y = 40;
    for(int i=1; i<17; i++)
    {
        wall = new Wall(position_x * 17, position_y * i, false);
        wall_g = new Wall_G(wall, getScene());
        wall_List.append(wall);
        wall_G_List.append(wall_g);
    }
    for(int i=1; i<17; i++)
    {
        wall = new Wall(position_x * i, position_y * 17, true);
        wall_g = new Wall_G(wall, getScene());
        wall_List.append(wall);
        wall_G_List.append(wall_g);
    }
}

Simulation::Simulation()
{
    window_w=900;
    window_h=700;
    scene=new QGraphicsScene();
    scene->setSceneRect(0,0,window_w,window_h);

    gview = new QGraphicsView(scene);
    gview->setScene(scene);
    gview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gview->setFixedSize(window_w,window_h);
    loadMap("map1");
    start = addButton(720, 100, "START");
//    connect(start, SIGNAL(clicked()), this, SLOT(start));    // naprawic connect
    connect(start,SIGNAL(clicked()),this,SLOT(startClicked()));
    stop = addButton(720, 170, "STOP");
    connect(stop,SIGNAL(clicked()),this,SLOT(resetClicked()));
//    addComboBox(750, 240, comboBox);

    robot = new Robot (60, 660, 1, this);
    robot_g = new Robot_G(robot, getScene());
    scene->addItem(robot_g);
    robot_g->draw();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
}

QPushButton *Simulation::addButton(int _x, int _y, QString _name)
{
    QPushButton* button;
    button = new QPushButton(_name,gview);
    button->setGeometry(_x, _y, 150, 50);
    return button;
}

void Simulation::addComboBox(int _x, int _y, QComboBox *comboBox)
{
    comboBox = new QComboBox(gview);
    comboBox->setGeometry(_x, _y, 100, 50);
}

Simulation::~Simulation()
{

}

void Simulation::show()
{
    gview->show();
}

void Simulation::stopTimer()
{
    timer->stop();
}

QList<Wall *> Simulation::getWalls(Robot *_robot)
{
    QList<Wall* > walls;
    if(wall_List.size()==0)
    {
        return walls;
    }
    for(int i = 0; i < wall_List.size(); i++)
    {
        if( (abs(_robot->getX() - wall_List[i]->getX()) < 60) && (abs(_robot->getY() - wall_List[i]->getY()) < 60))
        {
            walls.append(wall_List[i]);
        }
    }
    return walls;
}

void Simulation::startClicked()
{
    timer->start(100);
}

void Simulation::resetClicked()
{
    timer->stop();
}

void Simulation::update()
{
    robot->maping();
    robot_g->draw();
}
