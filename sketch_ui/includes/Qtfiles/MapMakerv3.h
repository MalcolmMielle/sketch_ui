#ifndef MapMakerv3_H
#define MapMakerv3_H

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
#include <QSlider>
#include<QLCDNumber>
#include <QSpinBox>
#include <QDir>

#include "imageLabel.h"
namespace AASS{
		
	class MapMakerv3 : public QMainWindow
	{
		Q_OBJECT
		
		
	protected:
		QWidget *_zoneCentrale;
		QVBoxLayout * _full_window;
		QGridLayout *_grid;
		QGridLayout *_down_grid;
		
		QPushButton *_path ; 
		QPushButton *_obstacle;
		QPushButton *_robot ;
		QPushButton *_quit ;
// 		QPushButton *_mode;
		QPushButton *_reset;
		QPushButton *_xport;
		QPushButton *_doors;
		
		ImageLabel *_image;
		
		QString _path_for_export;
		

	public:
		MapMakerv3(int x, int y, const QString& path);
		virtual ~MapMakerv3();
		
		virtual void setImage(){
			_image->printImage();
		}
		
	public slots:
		void change2Obstacle();
		void change2Path();
		void change2Robot();
		void change2Doors();

		void reset();
		virtual void Sysexport();

	};
}

#endif // MapMakerv3_H
