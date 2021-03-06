#ifndef MAPREADER_MAP_08042016
#define MAPREADER_MAP_08042016

#include "SketchMap.hpp"
#include "sketchAlgorithm/MapComparator/GraphMatcherBase.hpp"


namespace AASS{
	class SketchComparator{
		
	protected:
		std::string _path_to_save_result;
		SketchMap _input;
		SketchMap _model;
		cv::Mat _result;
		cv::Mat _result_graph;
		AASS::graphmatch::GraphMatcherBase* _comparator;
		
	public:
		SketchComparator(const SketchMap& input, const SketchMap& model, AASS::graphmatch::GraphMatcherBase* cc, const std::string& path_result) : _path_to_save_result(path_result), _input(input), _model(model), _comparator(cc){}
		
		void compare(){
						
			_input.think();
			std::cout << "MODEL" << std::endl;
			_model.think();
			graphmatch::GraphPlace gp_input = _input.getGraphPlace();
			graphmatch::GraphPlace gp_model = _model.getGraphPlace(); 
			
// 			cv::Mat graph = cv::Mat::zeros(500, 500, CV_8UC3);
// 			gp_model.draw(graph);
// 			cv::imshow("GRAPH", graph);
// 			cv::waitKey(0);
			
// 			std::cout << "COMPARE graph of size " <<gp_input.getNumVertices() << " " << gp_model.getNumVertices() << std::endl;
			//Comparaison
			_comparator->match(gp_input, gp_model);
			
// 			std::cout << "Done comparing" << std::endl;
			
			std::deque<	
				AASS::graphmatch::Hypothese
			> hypothesis_final_custom = _comparator->getResult();
			_comparator->sort(hypothesis_final_custom);
			
			//EXPORT result image 
			
			_result.release();
			
			hypothesis_final_custom[0].drawLinks(gp_input, gp_model, _input.getObstacleMat(), _model.getObstacleMat(), "ALL FINAL CUSTOM", 1, _result);
			
			topologicalmap::GraphLine graphl = _input.getLineFollower().getGraph(0);
			cv::Mat gl = cv::Mat::zeros(_input.getObstacleMat().size(), CV_8U);
			gp_input.draw(gl);
			topologicalmap::GraphLine graphl2 = _model.getLineFollower().getGraph(0);
			cv::Mat gl2 = cv::Mat::zeros(_model.getObstacleMat().size(), CV_8U);
			gp_model.draw(gl2);
			
			hypothesis_final_custom[0].drawLinks(gp_input, gp_model, gl, gl2, "ALL FINAL CUSTOM", 1, _result_graph);
						
// 			cv::imshow("OBSTres", _result);
// 			std::cout << "FIN COMPARISON" << std::endl;
						
			
		};
		
		void importFile2System(){
			_input.importFile2System();
			_model.importFile2System();
		}
		
		void exportSystem2File(){
			_input.exportSystem2File();
			_model.exportSystem2File();
			exportResult();
		};
		
		void setInputPath(const std::string& path){
			_input.setPathData(path);
		}
		void setModelPath(const std::string& path){
			_model.setPathData(path);
		}
		
		void setPath2SaveResult(const std::string& str){
			_path_to_save_result = str;
		}
		const std::string& getPath2SaveResult(){ return _path_to_save_result;}
		
		void exportResult(){
			cv::imwrite(_path_to_save_result + "/result.jpg", _result);
			cv::imwrite(_path_to_save_result + "/result_graph.jpg", _result_graph);
		 	
		}
		
		void reset(){
			_input.reset();
			_model.reset();
			_comparator->clear();
		}
		
		
		
		
	};

}

#endif
