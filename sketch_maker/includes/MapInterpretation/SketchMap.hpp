#ifndef MAPREADER_MAP
#define MAPREADER_MAP

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sys/stat.h> 
 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Robot.hpp"
#include "vodigrex/voronoidiagram/Thinker.hpp"
#include "vodigrex/linefollower/MultipleLineFollowerKeypoints.hpp"
#include "sketchAlgorithm/MapComparator/PlaceExtractorBase.hpp"
// #include "GraphLine.hpp"

/******************************************/

namespace AASS{ 
	/**
	 * @brief Master class containing instances
	 * 
	 * This is the sketch SketchMap class. 
	 * 
	 * Contains the cv::Mat of all elements
	 * 
	 */
	class SketchMap{ 
	protected : 
		
		std::string _path_to_data;
		
		int _rows;
		int _cols;
		cv::Point2f _previous_point;
		cv::Mat _obstacle_map;
		cv::Mat _path_map;
		cv::Mat _result;
		cv::Mat _lines;
		 
		cv::Scalar _obstacle_color;
		cv::Scalar _path_color;
		std::deque<Robot> _list_robot;
		std::deque<cv::Point2i> _list_obstacle;
		std::deque<cv::Point2i> _list_path;
		std::deque<cv::Point2i> _list_doors;
		AASS::vodigrex::Thinker* _think;
		AASS::vodigrex::MultipleLineFollowerKeypoints<topologicalmap::NodeLine, vodigrex::SimpleEdge> _line_follower;
		graphmatch::PlaceExtractorBase* _place_extractor;
				
		
		
	public:
		
		SketchMap(AASS::vodigrex::Thinker& t, graphmatch::PlaceExtractorBase& pe) : _rows(500), _cols(500),_previous_point(0,0), 
		_obstacle_map(cv::Mat::zeros(500, 500, CV_8UC3) ), 
		_path_map(cv::Mat::zeros(500, 500, CV_8UC3) ), 
		_result(cv::Mat::zeros(500, 500, CV_8UC3) ),  
		_lines(cv::Mat::zeros(500, 500, CV_8UC3) ), 
		_obstacle_color(cv::Scalar(0,255,0)),  
		_path_color(cv::Scalar(0,0,255)), 
		_think(&t),
		_place_extractor(&pe)
		{
			cv::rectangle(_obstacle_map, cv::Point2f(1,1), cv::Point2f(409, 409), _obstacle_color, 5);
			
			for (int i=10;i<480;i=i+(500/40)){
				_list_obstacle.push_back(cv::Point2f(1, i));
				_list_obstacle.push_back(cv::Point2f(i, 1));
				_list_obstacle.push_back(cv::Point2f(499, i));
				_list_obstacle.push_back(cv::Point2f(i, 499));
			}
			
			setMode(4);
			setDownSample(1);
			
		}; 
		
		SketchMap(int width, int height, AASS::vodigrex::Thinker& t, graphmatch::PlaceExtractorBase& pe) :_rows(width), _cols(height), 
		_obstacle_map(cv::Mat::zeros(width, height, CV_8UC3) ), 
		_path_map(cv::Mat::zeros(width, height, CV_8UC3) ), 
		_result(cv::Mat::zeros(width, height, CV_8UC3) ), 
		_lines(cv::Mat::zeros(500, 500, CV_8UC3) ), 
		_obstacle_color(cv::Scalar(0,255,0)), 
		_path_color(cv::Scalar(0,0,255)), 
		_think(&t),
		_place_extractor(&pe)
		{
			cv::rectangle(_obstacle_map, cv::Point2f(1,1), cv::Point2f(height-1, width-1), _obstacle_color, 5);
			
			for (int i=10;i<height - 10;i=i+(height/40)){
				_list_obstacle.push_back(cv::Point2f(1, i));
				_list_obstacle.push_back(cv::Point2f(height-1, i));
			}
			for (int i=10;i<width - 10;i=i+(width/40)){
				_list_obstacle.push_back(cv::Point2f(i, 1));
				_list_obstacle.push_back(cv::Point2f(i, width-1));
			}
			
			setMode(4);
			setDownSample(1);
			
		};
		
		virtual ~SketchMap() {std::cout << "Removing SketchMap" << std::endl;}
		
		void setMode(int m){ _think->setMode(m);};
		
		int getMode(){
			return _think->getMode();
		}
		
		void setObstacleMat(cv::Mat& m){_obstacle_map = m;}
		
		void setObstacleColor(const cv::Scalar& color){_obstacle_color = color;}
		void setPathColor(const cv::Scalar& color){_path_color = color;}
		void setPathData(const std::string& path){_path_to_data = path;}
		const std::string& getPathData(){return _path_to_data;}
		
		//TODO multi robot functionnality
		void addRobot(const Robot& r){
			if(r.getX() >= 0 && r.getX() < _cols && r.getY() >= 0 && r.getY() < _rows){
				_list_robot.clear();_list_robot.push_back(r);drawRobot(r);
				//Let's the flood Fill start
				cv::Mat tmp = _obstacle_map.clone();
				cv::floodFill(tmp, _list_robot[0].getPoint(), _obstacle_color);
				tmp=_obstacle_color-tmp;
				_obstacle_map = _obstacle_map + tmp;
			}
			
		}
// 		//Draw obstacles on map and add them to obstacle vector. In case to point are two far away, draw a line of point in between them.
		void addObstacle(int x, int y, bool first_click);
		void addPath(int x, int y, bool first_click){
			_list_path.push_back(cv::Point2f(x, y));
			drawPath(x,y, first_click);
			
		}
		
		void addDoors(const cv::Point2i& d, const cv::Point2i d1){
			_list_doors.push_back(d);
			_list_doors.push_back(d1);
		}
		
		
		//This is how you do both const and non const get for big gets, However it's ugly here...
		/*
		* const cv::Mat& getSketchMap() const {return _map;}
		cv::Mat& getSketchMap() {return const_cast<cv::Mat&>(static_cast<const SketchMap&>(*this).getSketchMap());}
		const cv::Mat& getResult() const {return _result;}
		cv::Mat& getResult() {return const_cast<cv::Mat&>(static_cast<const SketchMap&>(*this).getResult());}
		const std::vector<int>& getObstacles() const {return _list_obstacle;}
		std::vector<int>& getObstacles() {return const_cast< std::vector<int>& >(static_cast< const SketchMap&>(*this).getObstacles());}
		const std::vector<int>& getPath() const {return _list_path;}
		std::vector<int>& getPath() {return const_cast< std::vector<int>& >(static_cast< const SketchMap&>(*this).getPath());}
		const std::vector<Robot>& getRobot() const {return _list_robot;}
		std::vector<Robot>& getRobot() {return const_cast< std::vector<Robot>& >(static_cast< const SketchMap&>(*this).getRobot());}
		
		*/
		
		const cv::Mat& getObstacleMat() const {return _obstacle_map;}
		const cv::Mat& getPathMat() const {return _path_map;}
		const cv::Mat& getResult() const {return _result;}
		const std::deque<cv::Point2i>& getObstacles() const {return _list_obstacle;}
		const std::deque<cv::Point2i>& getPath() const {return _list_path;}
		const std::deque<Robot>& getRobots() const {return _list_robot;}
		const std::deque<cv::Point2i>& getDoors() const {return _list_doors;}
		int getRows(){return _rows;}
		int getCols(){return _cols;}
		
		//TODO shouldn't granted direct access to thinker and landmark maker
		const AASS::vodigrex::Thinker* getThinker() const {return _think;}
		
		cv::Mat& getObstacleMat() {return _obstacle_map;}
		cv::Mat& getPathMat() {return _path_map;}
		cv::Mat& getResult() {return _result;}
		std::deque<cv::Point2i>& getObstacles() {return _list_obstacle;}
		std::deque<cv::Point2i>& getPath() {return _list_path;}
		std::deque<Robot>& getRobots() {return _list_robot;}
		std::deque<cv::Point2i>& getDoors(){return _list_doors;}
		
		const bettergraph::PseudoGraph<topologicalmap::NodeLine, vodigrex::SimpleEdge>& getGraphLines() const {return _line_follower.getGraph(0);}
		const graphmatch::GraphPlace& getGraphPlace() const {return _place_extractor->getGraph();}
		bettergraph::PseudoGraph<topologicalmap::NodeLine, vodigrex::SimpleEdge> getGraphLines() {return _line_follower.getGraph(0);}
		graphmatch::GraphPlace& getGraphPlace() {return _place_extractor->getGraph();}
		
		AASS::vodigrex::Thinker* getThinker(){return _think;}
		
		vodigrex::MultipleLineFollowerKeypoints<topologicalmap::NodeLine, vodigrex::SimpleEdge>& getLineFollower(){return _line_follower;}
		cv::Mat& getLines(){
			_lines = _line_follower.getResult();
			if(_think->getMode() == 4 ){
				for (int i=0; i< _think->getDownSample() ; i++){
					cv::pyrUp(_lines, _lines, cv::Size(_lines.cols*2, _lines.rows*2 ));
				}
			}
			return _lines;}
		
		void setDownSample(int d){
			_think->setDownSample(d);
		};
		
		///@brief clear the object and drew the border again
		void reset();
		
		///@brief clear everything
		void clear();
		
		/**
		* @brief Calculate Voronoi lines + topological map + places
		* 
		* Start by using AASS::vodigrex::Thinker to extract the Voronoi Lines
		* 
		* Then use LineFollower to extract the lines 
		* 
		* Finally uses Place extractor to create a graph of places.
		*/
		void think(){
			std::cout << "map think" << std::endl;
			
// 			cv::imshow("inpput", _obstacle_map);
// 			cv::waitKey(0);
			 
			_think->think( _obstacle_map );
			
			_result = cv::Mat::zeros(_think->getResult().size(), CV_8UC1);
			
			if(_think->getMode() == 4 || _think->getMode() == 8 || _think->getMode() == 9){
				
// 				cv::imshow("inpputsdfsd",  _think->getResult());
// 				cv::waitKey(0);
				_line_follower.inputMap( _think->getResult() );
				_line_follower.setDoors(_list_doors);
				std::cout << "thin algo" << std::endl;
	// 			exportSystem2File();
				try{
					_line_follower.thin();
	// 				_line_follower.postProcessing(_obstacle_map);
					_lines = _line_follower.getResult();
// 					_line_follower.drawGraph(_result);
// 					_line_follower.printGraph();
				}
				catch(std::exception e){
					std::cout << "Catch exception in the line thining process : " << e.what() << std::endl;
				}
				
				try{
					//Scale graph
					_line_follower.sort();
					
// 					for(int i = 0 ; i < _line_follower.size() ; ++i){
// 						topologicalmap::GraphLine graphl = _line_follower.getGraph(i);
// 						cv::Mat gl = cv::Mat::zeros(getObstacleMat().size(), CV_8U);
// 						graphl.draw(gl);
// 						cv::imshow("gr",gl);
// 						cv::waitKey(0);
// 					}
					
					
					topologicalmap::GraphLine gl = _line_follower.getGraph(0);
					//Scale back if needed.
					if(_think->getMode() == 4 ){
						gl.scale(2);
					}
					
					_place_extractor->inputMapIn(_obstacle_map);
					_place_extractor->inputGraph(gl);
					_place_extractor->extract();
					_result = _place_extractor->getResult();
					
				}
				catch(std::exception e){
					std::cout << "Catch exception in the place extraction process : " << e.what() << std::endl;
				}
				
			}

			
		}
		
		
		
		/*
		 * Main functionnality
		 */
		
		void run(){
			importFile2System();
			think();
		}
		
		/*
		* Export the system in Export folder in the current directory
		*/
		void exportSystem2File();
		
		
		/*
		* Import le system from a Export folder in the courrent directory
		*/
		void importFile2System();
		
	private:
			/******* Drawing functions*******/
		void drawRobot(const Robot& r);
		void drawObstacle(int x, int y, bool first_click);
		void drawPath(int x, int y, bool first_click);
		
		void save_deque(const std::deque<cv::Point2i>& src, const std::string& file);
		void save_deque(const std::deque<Robot>& src, const std::string& file);
		
		void import_deque(std::deque< cv::Point2i >& target, const std::string& file);
		void import_deque(std::deque<Robot>& target, const std::string& file);
		
	};



	/*******************FUNCTIONS**************************/





//TODO input string here

}

#endif
