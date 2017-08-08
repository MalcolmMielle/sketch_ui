#include "SketchComparator.hpp"
#include "sketchAlgorithm/MapComparator/GraphMatcherClusterFiltered.hpp"
#include "sketchAlgorithm/MapComparator/GraphMatcherOld.hpp"
#include "sketchAlgorithm/MapComparator/GraphMatcherNeighbor.hpp"
#include "sketchAlgorithm/MapComparator/PlaceExtractorRoomCorner.hpp"
#include "sketchAlgorithm/MapComparator/PlaceExtractorList2Place.hpp"

/**
@mainpage SketchMaker
@author Malcolm Mielle
@date 21st September 2015
@note Use c++ and qt

**/

int main(int argc, char** argv)
{
    int rows = 500;
	int cols = 500;
	std::cout << " argc : "<<argc<<std::endl;
	if(argc == 2 || argc >3 ){
		std::cout << "Usage is not good" <<std::endl;
	}
	else if(argc == 3){
		std::cout << "user given dimensions "<<std::endl;
		rows = atoi(argv[1]);
		cols = atoi(argv[2]);
	}
	
	try{
		if(rows <= 0 || cols <= 0){
			throw 0;
		}
	}
	catch (int e){
		std::cout << "Rows or Cols should not be less than or 0. Going for default value 500x500" << std::endl;
		rows = 500;
		cols = 500;
	}
	 
	//Declaration of a Map 
	//cv::Mat mat= cv::imread("map.png");
	AASS::vodigrex::ThinkerVoronoi  t;
	AASS::graphmatch::PlaceExtractorList2Place rc;
// 	Thinker_CGA t; 
	AASS::SketchMap m(rows, cols, t, rc);
	m.setDownSample(1);
	m.setMode(4);
	
	
	AASS::vodigrex::ThinkerVoronoi  t_model;
	AASS::graphmatch::PlaceExtractorList2Place rc_model;
// 	Thinker_CGA t; 
	AASS::SketchMap mm(rows, cols, t_model, rc_model);
	mm.setDownSample(1);
	mm.setMode(4);
	
// 	m.importFile2System();
	AASS::graphmatch::GraphMatcherNeighbor gm;
	AASS::SketchComparator model(m, mm, &gm, "../result");
	
	model.setInputPath("../input");
	model.setModelPath("../model");
	model.setPath2SaveResult("../result");
	
	while(true){
		model.importFile2System();
		
		model.compare();
		model.exportResult();
		
		//Suppress marker importFile2System
		
		model.reset();
		cv::waitKey(50);
	}
	
	AASS::graphmatch::GraphMatcherNeighbor comparator;

}
