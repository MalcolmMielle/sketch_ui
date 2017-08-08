#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <iostream>
#include <QtGui/QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include<QImage>
#include<QPainter>

#include <iostream>
#include <fstream>

#include "matconversions.hpp"

namespace AASS{
		
	class ImageLabel : public QLabel
	{
		Q_OBJECT
	protected:

		QPoint _mouse;
		QPoint _last_mouse;
		QPoint _robot;
		bool _flag_obstacle;
		bool _flag_path;
		bool _flag_robot;
		bool _left_press;
		bool _flag_doors;
		QImage _obstacle_image;
		QImage _path_image;
		
		std::vector < QPoint > _doors;
		

	public:
		ImageLabel(int x, int y);
		~ ImageLabel(){};
		
		void setFlagObstacle();
		void setFlagPath();
		void setFlagRobot();
		void setFlagDoors();
		void mouseMoveEvent ( QMouseEvent * event );
		void mouseReleaseEvent ( QMouseEvent * event );
		void mousePressEvent(QMouseEvent * event);
		void printImage();
		void exportDoors(const QString& path);
		void reset();
		
		QImage getObstacles(){return _obstacle_image;}
		QImage getPath(){
			
			QImage path = QImage(QSize(_path_image.size().width() , _path_image.size().height()), QImage::Format_ARGB32_Premultiplied);
			QPainter painter(&path);
			painter.setCompositionMode(QPainter::CompositionMode_Source);
			painter.fillRect(path.rect(), Qt::black);
			fuseQImage(path, _path_image);
			return path;
		}

		
	private:
		void draw(bool first_click);
	};
}

#endif // MapMakerv3_H
