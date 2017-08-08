#include <QtGui/QApplication>
#include "MapMakerExpert.h" 

/**
@mainpage SketchMaker
@author Malcolm Mielle
@date 21st September 2015
@note Use c++ and qt

**/

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    int rows = 550;
    int cols = 600;
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
    
    AASS::MapMakerExpert mapmakerv3(cols, rows, "./");
    mapmakerv3.show();
	
    mapmakerv3.setImage();
	 
    return app.exec();
}
