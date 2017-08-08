#include "MapMakerExpert.h"



AASS::MapMakerExpert::MapMakerExpert(int x, int y, const QString& path) : MapMakerv3(x, y, path)
{
	
	//Fixing the string with image position
	_path_to_result = path + "/result.jpg";
	
	_result = new QLabel();
	
	
	_down_grid->addWidget(_image, 0, 0);
	_down_grid->addWidget(_result, 0, 1);

	_full_window->addLayout(_down_grid);
	
	
	timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
	
// 	QImage image;
// 	image.load(_path_to_result);
// 	image = image.scaledToHeight(_image->height() );
// 	_result->setPixmap( QPixmap::fromImage(image) );
	
	update();
	
}
	

AASS::MapMakerExpert::~MapMakerExpert()
{}

//Update the result image
void AASS::MapMakerExpert::update()
{
	
	QImage image;
	image.load(_path_to_result);
	image = image.scaledToHeight(_image->height() );
	_result->setPixmap(QPixmap::fromImage(image) );

}



#include "MapMakerExpert.moc"
