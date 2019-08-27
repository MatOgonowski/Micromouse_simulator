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
    while(wall_G_List.size() != 0)
    {
        delete  wall_G_List[0];
        wall_G_List.removeAt(0);
    }
    while (wall_List.size() != 0)
    {
        delete wall_List[0];
        wall_List.removeAt(0);
    }
//        scene->;
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

void Simulation::makeFileList()
{
    int i = 0;
    QString temp;
    QList<QString> temp_list;
    struct dirent * file;
    DIR * path;
    if((path = opendir("../micromouse/maps/")))
    {

       while((file = readdir(path)))
       {
           puts(file->d_name);
           for(int i = 0; i < file->d_namlen; i++){
               temp+=file->d_name[i];
           }
           temp_list.append(temp);
           temp.clear();
       }
       closedir(path);
    }
    while(i != temp_list.size())
    {
        for(i = 0; i < temp_list.size(); i++)
        {
            if(temp_list.at(i).size()<6)
            {
                temp_list.removeAt(i);
                break;
            }
        }
    }
    int k;
    for(int j = 0; j < temp_list.size(); j++)
    {
        k = temp_list.at(j).length();
        temp = temp_list.at(j);
        temp.remove(k-4,k-1);
        fileList.append(temp);
    }
}

Simulation::Simulation()
{
    window_w=900;
    window_h=700;
    scene=new QGraphicsScene();
    scene->setSceneRect(0,0,window_w,window_h);
    makeFileList();
    gview = new QGraphicsView(scene);
    gview->setScene(scene);
    gview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gview->setFixedSize(window_w,window_h);
    gview->setWindowTitle("MICROMOUSE SIMULATOR");
    start = addButton(720, 100, "START");
    connect(start, SIGNAL(clicked()), this, SLOT(startClicked()));

    stop = addButton(720, 170, "STOP");
    connect(stop, SIGNAL(clicked()), this, SLOT(stopClicked()));

    mapComboBox = addComboBox(720, 370);
    mapComboBox->addItems(fileList);
    connect(mapComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=]{ chooseMap();});

    reset = addButton(720, 240, "RESET");
    connect(reset, SIGNAL(clicked()), this, SLOT(resetClicked()));

    algorithmComboBox = addComboBox(720, mapComboBox->y() + 50);
    QList<QString > algorithmList{"Propagacja fali", "Śledzenie ściany", "Losowe"};
    algorithmComboBox->addItems(algorithmList);
    connect(algorithmComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=]{ chooseAlgorithm();});

    mapLabel = new QLabel("MAPA:", gview);
    mapLabel->setGeometry(720, mapComboBox->y() - 20, 150, 20);
    algorithmLabel = new QLabel("ALGORYTM:", gview);
    algorithmLabel->setGeometry(720, algorithmComboBox->y() - 20, 150, 20);

    loadMap(mapComboBox->currentText());
    robot = new Robot (16, 1, 1, this);
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

QComboBox *Simulation::addComboBox(int _x, int _y)
{
    QComboBox* comboBox;
    comboBox = new QComboBox(gview);
    comboBox->setGeometry(_x, _y, 150, 20);
    return comboBox;
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

void Simulation::stopClicked()
{
    timer->stop();
}

void Simulation::resetClicked()
{
    timer->stop();
    robot->reset();
    robot_g->draw();
}

void Simulation::update()
{
    if(algorithmComboBox->currentIndex() == SPILLING_WATER)
    {
        robot->maping();
    }
    else if (algorithmComboBox->currentIndex() == WALL_FOLLOWER)
    {
        robot->wallFollower();
    }
    else if (algorithmComboBox->currentIndex() == BRUTE_FORCE)
    {
        robot->bruteForce();
    }
    robot_g->draw();
}

void Simulation::chooseMap()
{
    timer->stop();
    robot->reset();
    robot_g->draw();
    loadMap(mapComboBox->currentText());
}

void Simulation::chooseAlgorithm()
{
    timer->stop();
    robot->reset();
    robot_g->draw();
}
