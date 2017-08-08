#include "MapMakerv3.h"



AASS::MapMakerv3::MapMakerv3(int x, int y, const QString& path) : _path_for_export(path)
{
	
	//Check export folder exist
	bool exist = QDir(path).exists();
	if(exist == false){
		QDir().mkdir(path);
	}
	
	_image = new ImageLabel(x, y);
	std::cout << "First heigh " << _image->height() << x << y << std::endl;
	//TODO move this on top
	_zoneCentrale = new QWidget;

	_grid = new QGridLayout;
	_down_grid = new QGridLayout;
	
	_path = new QPushButton("Path");
	_obstacle = new QPushButton("Obstacle");
	_robot = new QPushButton("Robot");
	_quit = new QPushButton("Quit");
	_reset = new QPushButton("Reset");
	_xport = new QPushButton("Export");
	_doors = new QPushButton("Doors");

	
	_grid->addWidget(_path, 0,0);
	_grid->addWidget(_obstacle, 0,1);
	_grid->addWidget(_robot, 0,2);
	_grid->addWidget(_quit,0 ,3);
	_grid->addWidget(_doors, 1, 1);
	_grid->addWidget(_reset, 1, 2);
	_grid->addWidget(_xport, 1, 3);
	
	_full_window = new QVBoxLayout();
	
	_full_window->addLayout(_grid);
	_full_window->addWidget(_image);
	
// 	std::cout << "First heigh " << _image->height() << std::endl;


	_full_window->addLayout(_down_grid);
	
    _zoneCentrale->setLayout(_full_window);
	
	

    setCentralWidget(_zoneCentrale);
	
	//Slots
	QObject::connect(_obstacle, SIGNAL(clicked()), this, SLOT(change2Obstacle()));
	QObject::connect(_path, SIGNAL(clicked()), this, SLOT(change2Path()));
	QObject::connect(_robot, SIGNAL(clicked()), this, SLOT(change2Robot()));
	QObject::connect(_doors, SIGNAL(clicked()), this, SLOT(change2Doors()));
	QObject::connect(_quit, SIGNAL(clicked()), this, SLOT(close()));
	QObject::connect(_reset, SIGNAL(clicked()), this, SLOT(reset()));
	QObject::connect(_xport, SIGNAL(clicked()), this, SLOT(Sysexport()));
	
	std::cout << "First heigh " << _image->height() << std::endl;
	
}
	

AASS::MapMakerv3::~MapMakerv3()
{}

void AASS::MapMakerv3::change2Obstacle()
{
	_image->setFlagObstacle();
	std::cout <<"You're now drawing obstacles"<<std::endl;
}

void AASS::MapMakerv3::change2Path()
{
	_image->setFlagPath();
	std::cout <<"You're now drawing path"<<std::endl;
}

void AASS::MapMakerv3::change2Robot()
{
	_image->setFlagRobot();
	std::cout <<"You're now drawing robots"<<std::endl;
}

void AASS::MapMakerv3::change2Doors()
{
	_image->setFlagDoors();
	std::cout <<"You're now drawing doors"<<std::endl;
}


void AASS::MapMakerv3::reset()
{

	std::cout << "Reset"<<std::endl;
	_image->reset();
	_image->printImage();
}



void AASS::MapMakerv3::Sysexport(){
	std::cout << "EXPORTING" << std::endl;
	QString path_obs = _path_for_export + "/obstaclemap.png";
	QString path_path = _path_for_export + "/path.png";
	_image->getObstacles().save(path_obs, "PNG");
	_image->getPath().save(path_path, "PNG");
	_image->exportDoors(_path_for_export);
	std::cout << "exporting finished" << std::endl;
}




#include "MapMakerv3.moc"
