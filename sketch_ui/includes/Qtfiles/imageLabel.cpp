#include "imageLabel.h"


AASS::ImageLabel::ImageLabel(int x, int y): QLabel(), _robot(-1, -1), _flag_obstacle(true), _flag_path(false), _flag_robot(false), _flag_doors(false), _left_press(false), _obstacle_image(x , y, QImage::Format_ARGB32_Premultiplied), _path_image(x , y, QImage::Format_ARGB32_Premultiplied){
// 	std::cout << x << y << _obstacle_image.height() << std::endl;
// 	std::cout << "This " << this->height() << std::endl;
	reset();
	this->setAlignment(Qt::AlignCenter);

	//set MouseTracking true to capture mouse event even its key is not pressed
	this->setMouseTracking(true);
	
	this->printImage();
	this->show();
	
// 	std::cout << "This " << this->height() << std::endl;
}
	
void AASS::ImageLabel::setFlagObstacle(){
	_flag_obstacle=true;
	_flag_path=false;
	_flag_robot=false;
	_flag_doors = false;

	
}
void AASS::ImageLabel::setFlagPath(){
	_flag_obstacle=false;
	_flag_path=true;
	_flag_robot=false;
	_flag_doors = false;
}
void AASS::ImageLabel::setFlagRobot(){
	_flag_obstacle=false;
	_flag_path=false;
	_flag_robot=true;
	_flag_doors = false;
}
void AASS::ImageLabel::setFlagDoors(){
	_flag_obstacle=false;
	_flag_path=false;
	_flag_robot=false;
	_flag_doors = true;
}
	
void AASS::ImageLabel::mouseMoveEvent ( QMouseEvent * event )
{
	_last_mouse = _mouse;
	_mouse = event -> pos();

	draw(false);
}
	
void AASS::ImageLabel::mouseReleaseEvent ( QMouseEvent * event )
{
	if(event->button() == Qt::LeftButton)
	{
		if(_flag_doors == true){
			_doors.push_back(event -> pos());
		}
		_left_press=false;
		this->printImage();

	}  
	
}; 
	
void AASS::ImageLabel::mousePressEvent(QMouseEvent * event){
	
	if(event->buttons() == Qt::LeftButton){
		if(_flag_doors == true){
			_doors.push_back(event -> pos());
		}
		else if(_flag_robot == true){
			_robot = event->pos();
		}
		_left_press=true;
		this->draw(true);
	}
}
	
void AASS::ImageLabel::printImage(){
	
	
	QImage full = QImage(_obstacle_image.size(), QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&full);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(full.rect(), Qt::transparent);
	painter.drawImage(0, 0, _obstacle_image);
	fuseQImage(full, _path_image);
	
	QPen pen;
	pen.setColor(Qt::color0);
	pen.setWidth(10);
	painter.setPen(pen);
			
	if(_doors.size() > 0){
		for(size_t i = 1 ; i < _doors.size() ; i = i+2){
			painter.drawLine(_doors[i-1], _doors[i]);
		}
		painter.drawEllipse(_doors[_doors.size()-1], 1, 1);
	}
		
	if(_robot.x() != -1 && _robot.y() != -1){
		QPen pen_rob;
		pen_rob.setColor(Qt::blue);
		pen_rob.setWidth(30);
		painter.setPen(pen_rob);
		painter.drawEllipse(_robot.x(), _robot.y(), 30, 30);
	}
	
	painter.end();
	
	this->setPixmap(QPixmap::fromImage(full));
}

void AASS::ImageLabel::draw(bool first_click){
	if(_left_press == true){
		if(_flag_obstacle == true){
			QPen pen;
			pen.setColor(Qt::green);
			pen.setWidth(10);
			pen.setCapStyle(Qt::RoundCap);
			pen.setJoinStyle(Qt::RoundJoin);
			QPainter painter(&_obstacle_image);

			painter.setCompositionMode(QPainter::CompositionMode_Source);
			painter.setPen(pen);
			if(first_click != true){
				painter.drawLine(_last_mouse, _mouse);
			}
			else{
				painter.drawPoint(_mouse);
			}
			painter.end();
			
		}
		else if(_flag_path == true){
			QPen pen;
			pen.setColor(Qt::red);
			pen.setWidth(10);
			pen.setCapStyle(Qt::RoundCap);
			pen.setJoinStyle(Qt::RoundJoin);
			QPainter painter(&_path_image);

			painter.setCompositionMode(QPainter::CompositionMode_Source);
			painter.setPen(pen);
			if(first_click != true){
				painter.drawLine(_last_mouse, _mouse);
			}
			else{
				painter.drawPoint(_mouse);
			}
			painter.end();
		}
		
		else{
		}
		
		//Update map result state
		this->printImage();

	}
}

void AASS::ImageLabel::exportDoors(const QString& path)
{
	std::ofstream myfile;
	QString total_path = path + "/keypoints.txt";
	myfile.open (total_path.toStdString().c_str() );
	for(size_t i = 0 ; i < _doors.size() ; i = i + 2){
// 		std::cout << "Exporting keypoint" << std::endl;
		int x = _doors[i].x();
		int y = _doors[i].y();
		int x_2 = _doors[i+1].x();
		int y_2 = _doors[i+1].y();
		myfile << x << " " << y << std::endl << x_2 << " " << y_2 << "\n";
	}
	myfile.close();

}


void AASS::ImageLabel::reset()
{
	
	_doors.clear();
	
	_obstacle_image = QImage(_obstacle_image.size(), QImage::Format_ARGB32_Premultiplied);
	_path_image = QImage(_path_image.size(), QImage::Format_ARGB32_Premultiplied);
	
	QPen pen;
	pen.setColor(Qt::green);
	pen.setWidth(10);
	QPainter painter(&_obstacle_image);
	painter.setPen(pen);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(_obstacle_image.rect(), Qt::black);
	
	QPainter ppainter(&_path_image);
	ppainter.setCompositionMode(QPainter::CompositionMode_Source);
	ppainter.fillRect(_path_image.rect(), Qt::transparent);
	ppainter.end();
	
	painter.drawLine(QPoint(0, 0), QPoint(0, _obstacle_image.size().height()));
	painter.drawLine(QPoint(0, _obstacle_image.size().height()), QPoint(_obstacle_image.size().width(), _obstacle_image.size().height()));
	painter.drawLine(QPoint(_obstacle_image.size().width(), _obstacle_image.size().height()), QPoint(_obstacle_image.size().width(), 0));
	painter.drawLine(QPoint(_obstacle_image.size().width(), 0), QPoint(0, 0));
	
	painter.end();
	
	_robot.setX(-1);
	_robot.setY(-1);
	

}


#include "imageLabel.moc"