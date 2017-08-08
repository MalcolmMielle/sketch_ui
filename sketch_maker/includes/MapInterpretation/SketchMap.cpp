#include "SketchMap.hpp"

void AASS::SketchMap::addObstacle(int x, int y, bool first_click){
	drawObstacle(x, y, first_click);
	if( first_click == false ){
		int size = _list_obstacle.size() -1;
		//std::cout << "Adding the obstacle on opencv"<<std::endl;
		//Add more obstacle points if the two point are two far away.
		int dist_x = _list_obstacle[size].x-x;
		int dist_y = _list_obstacle[size].y-y;
		
		//std::cout << "got the dist "<<dist_x<<" "<<dist_y<<std::endl;
		int dist_total = (dist_x*dist_x)+(dist_y*dist_y);
		int threshold = _obstacle_map.size().width;
		if(threshold < _obstacle_map.size().height){
			threshold = _obstacle_map.size().height;
		}
		
		//std::cout << "go loop" <<std::endl;
		if(dist_total > threshold/10 ){
			if(x != _list_obstacle[size].x){
				//std::cout << "Not a vertical line " <<std::endl;
				//Generatin the points : 
				float slope = (_list_obstacle[size].y -y) / (_list_obstacle[size].x - x);

				//std::cout << "new point : " << x <<" "<< y << " and old point "<< _list_obstacle[size].x << " " << _list_obstacle[size].y << " slope "<<slope <<std::endl;
				
				float low_x = _list_obstacle[size].x;
				float low_y = _list_obstacle[size].y;
				float high_x = x;
				float high_y = y;
				
				
				if(slope <=1 && slope >=-1){

					if(low_x > x){
						low_x = x;
						low_y = y;
						high_x = _list_obstacle[size].x;
						high_y = _list_obstacle[size].y;
					}
					
					for(low_x = low_x+10 ; low_x <= high_x;low_x=low_x+10){
						//std::cout << "for loop iwth x : " << low_x << " and y : " << (slope*(low_x -  high_x) + high_y) << std::endl;
						_list_obstacle.push_back(cv::Point2f(low_x, (slope*(low_x - high_x) + high_y)) );
					}
				}
				else{
					
					if(low_y > y){
						low_x = x;
						low_y = y;
						high_x = _list_obstacle[size].x;
						high_y = _list_obstacle[size].y;
					}
					
					for(low_y = low_y+10 ; low_y <= high_y;low_y=low_y+10){
						//std::cout << "for loop v2 iwth x : " << low_x << " and y : " <<  ( (low_y - high_y)/slope) + high_x  << std::endl;
						_list_obstacle.push_back(cv::Point2f(( ((low_y - high_y)/slope) + high_x) , low_y ));
					}
				}
			}
			
			//Handle vertical lines 
			else{
				//std::cout << "VERTICAL LINE"<<std::endl;
				int nu_y = y;
				int high_y = _list_obstacle[size].y;
				if(nu_y > _list_obstacle[size].y){
					nu_y = _list_obstacle[size].y;
					high_y = y;
				}
				for( ; nu_y <= high_y; nu_y = nu_y+10){
					_list_obstacle.push_back(cv::Point2f(x, nu_y) );
				} 
			}
			
		}
	}
	_list_obstacle.push_back(cv::Point2f(x, y));
		
}



 void AASS::SketchMap::exportSystem2File()
{
	//Create a export file if not aready created
	mkdir("Export", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	cv::imwrite("Export/obstaclemap.png", _obstacle_map);
	cv::imwrite("Export/path.png", _path_map);
	
	
	
	save_deque(_list_obstacle, "Export/obstacles.txt");
	save_deque(_list_path, "Export/path.txt");
	save_deque(_list_robot, "Export/robot.txt");
	save_deque(_list_doors, "Export/keypoints.txt");
	
}




 void AASS::SketchMap::importFile2System()
{
	
	std::string obstacle = _path_to_data + "/obstaclemap.png";
	std::string pathh =  _path_to_data + "/path.png";
// 	std::string result = path + "/theResult.png";
	
	_obstacle_map = cv::imread(obstacle);
	_path_map = cv::imread(pathh);
// 	_result = cv::imread(result);
	
	import_deque(_list_obstacle,  _path_to_data + "/obstacles.txt");
	import_deque(_list_path,  _path_to_data + "/path.txt");
	import_deque(_list_robot,  _path_to_data + "/robot.txt");
	import_deque(_list_doors,  _path_to_data + "/keypoints.txt");
	
}

/***************** Private funciton*************/

 void AASS::SketchMap::save_deque(const std::deque<cv::Point2i>& src, const std::string& file){
	std::ofstream myfile;
	myfile.open (file.c_str());
	for(std::deque<cv::Point2i>::const_iterator it=src.begin(); it!=src.end();){
		myfile << (*it).x <<" ";
		myfile << (*it).y <<std::endl;
		it++;
	}		
	myfile.close();
}

 void AASS::SketchMap::save_deque(const std::deque<Robot>& src, const std::string& file){
	std::ofstream myfile;
	myfile.open (file.c_str());
	for(std::deque<Robot>::const_iterator it=src.begin(); it!=src.end();){
		myfile << (*it).getX() <<" ";
		myfile << (*it).getY() <<std::endl;
		it++;
	}		
	myfile.close(); 
}

 void AASS::SketchMap::import_deque(std::deque< cv::Point2i >& target, const std::string& file)
{
	target.clear();
	std::ifstream myfile(file.c_str());
	float a;
	float b;
	while (myfile >> a)
    {
		myfile >> b;
		target.push_back(cv::Point2f(a,b));
    }
	
}

 void AASS::SketchMap::import_deque(std::deque< Robot >& target, const std::string& file)
{
	target.clear();
	std::ifstream myfile(file.c_str());
	int x;
	int y;
	//std::cout << "import robot"<<std::endl;
	while (myfile >> x)
    {
		myfile >> y;
		target.push_back(Robot(x,y));
    }
}

/********Drawing function***********/

 void AASS::SketchMap::drawObstacle(int x, int y, bool first_click)
{
	//std::cout << "Size : _list_obstacle.size() : " <<_list_obstacle.size()<<std::endl;
	if(first_click == true){
		cv::circle(_obstacle_map, cv::Point2d(x, y), 10, _obstacle_color, -1);
		_previous_point.x=x;
		_previous_point.y=y;
	}
	else{
		//std::cout << "LIIIIIINE"<<std::endl;
		cv::line(_obstacle_map, _previous_point, cv::Point2d(x, y), _obstacle_color, 20);
		_previous_point.x=x;
		_previous_point.y=y;
	}
}

 void AASS::SketchMap::drawPath(int x, int y, bool first_click)
{
	if(first_click == true){
		cv::circle(_path_map, cv::Point2d(x, y), 5, _path_color, -1);
	}
	else{
		cv::line(_path_map, _list_path[_list_path.size()-2], cv::Point2d(x, y), _path_color, 5);
	}
	
// 	imshow("path", _path_map);
	
}

 void AASS::SketchMap::drawRobot(const Robot& r)
{
	//cv::circle(_map, cv::Point2d(r.getX(), r.getY()), 5, _robot_color, -1);
}


/****CLEAR EVERYTHING***/
void AASS::SketchMap::clear()
{
	_obstacle_map = cv::Mat::zeros(_rows, _cols, CV_8UC3);
	_path_map = cv::Mat::zeros(_rows, _cols, CV_8UC3);
	_list_obstacle.clear();
	_list_path.clear();
	_list_robot.clear();
	_think->reset();
	_line_follower.clear();
	_place_extractor->reset();
}


/*************** Reset function to initial state *****************/
 void AASS::SketchMap::reset()
{
	
	clear();
	
	cv::rectangle(_obstacle_map, cv::Point2f(1,1), cv::Point2f(_cols-1, _rows-1), _obstacle_color, 5);
	for (int i=10;i<_rows - 10;i++){
		_list_obstacle.push_back(cv::Point2f(1, i));
		_list_obstacle.push_back(cv::Point2f(_rows-1, i));
	}
	for (int i=10;i<_cols - 10;i++){
		_list_obstacle.push_back(cv::Point2f(i, 1));
		_list_obstacle.push_back(cv::Point2f(i, _cols-1));
	}
		
}