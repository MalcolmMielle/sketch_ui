#ifndef MAPREADER_MAP_WINDOWS_13042016
#define MAPREADER_MAP_WINDOWS_13042016

#include "SketchComparator.hpp"
#include "WindowsSocket.hpp"


namespace AASS{
	class SketchComparatorWindows : public SketchComparator{
		
	protected:

		int _listenfd; 
		int _connfd;
		struct sockaddr_in _serv_addr; 

		time_t _ticks; 
		
	public:
		SketchComparatorWindows(const SketchMap& input, const SketchMap& model, AASS::graphmatch::GraphMatcherBase* cc, const std::string& path_result) : AASS::SketchComparator(input, model, cc, path_result), _listenfd(0), _connfd(0){
			
			_listenfd = socket(AF_INET, SOCK_STREAM, 0);
			memset(&_serv_addr, '0', sizeof(_serv_addr));

			_serv_addr.sin_family = AF_INET;
			_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			_serv_addr.sin_port = htons(DEFAULT_PORT); 

			bind(_listenfd, (struct sockaddr*)&_serv_addr, sizeof(_serv_addr)); 
			listen(_listenfd, 10); 
			
		};
		
		void compare(){
			printf("accepting connection on port 8861...\n");
			_connfd = accept(_listenfd, (struct sockaddr*)NULL, NULL); 

			_ticks = time(NULL);
			printf("client connected at %.24s\r\n", ctime(&_ticks));

			std::string input = _input.getPathData();
			std::string input_obstacle = input + "/obstaclemap.png";
			std::string input_path = input + "/path.png";
			std::string input_keypoints = input + "/keypoints.txt";
			// client connected -- read picture sent by client
			bool good = receive_file(_connfd, input_obstacle.c_str());
			bool good_1 = receive_file(_connfd, input_path.c_str());
			bool good_2 = receive_file(_connfd, input_keypoints.c_str());
			
			if(good == true && good_1 == true && good_2 == true){
				importFile2System();
				SketchComparator::compare();
				exportResult();
			}
			//TODO : bad hardcoded value
			std::string res_str = getPath2SaveResult();
			std::string res_final_str = res_str + "/result.jpg";
			send_file(_connfd, res_final_str);
			
			close(_connfd);
			printf("transfers completed, closing client socket\n");
			sleep(1);
			
		}
		
		
		
		
	};

}

#endif
