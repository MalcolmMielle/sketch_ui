#ifndef MapMakerv3_H_EXPERT_08042016
#define MapMakerv3_H_EXPERT_08042016

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
#include <QTimer>

#include "MapMakerv3.h"
namespace AASS{
		
	class MapMakerExpert : public MapMakerv3
	{
		Q_OBJECT
		
		
	protected:
		QLabel *_result;
		QTimer *timer;
		QString _path_to_result;
		

	public:
		MapMakerExpert(int x, int y, const QString& path);
		virtual ~MapMakerExpert();
		void setPath2Result(const QString& path){_path_to_result = path;}
		
	public slots:
		void update();

	};
}

#endif // MapMakerv3_H
