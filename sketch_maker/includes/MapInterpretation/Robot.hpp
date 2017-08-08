#ifndef ROBOT_MAP
#define ROBOT_MAP

#include <iostream>
#include <stdio.h>

#include <opencv2/opencv.hpp>
namespace AASS{
	
	class Robot{
	protected : 
		cv::Point2f _p;
		
	public : 

		Robot(int x, int y): _p(x, y){};
		
		void setX(int x){_p.x = x;}
		void setY(int y){_p.y = y;}
		
		int getX() const {return _p.x;}
		int getY() const {return _p.y;}
		cv::Point2f getPoint(){return _p;}

		
	};
}

#endif