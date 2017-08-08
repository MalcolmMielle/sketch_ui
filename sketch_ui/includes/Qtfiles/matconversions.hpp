#ifndef MAT_CONVERSION_TO_QT
#define MAT_CONVERSION_TO_QT

#include <QImage>



namespace cv {
class Mat;
}

namespace AASS{
	
	inline void fuseQImage(QImage& src, const QImage& to_overlay){
		
		for(size_t i = 0 ; i < to_overlay.height() ; i++){
			
			for(size_t j = 0 ; j < to_overlay.width() ; j++){
// 				std::cout << "Good point first " << dest.pixel(j, i) << " " << std::endl;
				if(to_overlay.pixel(j, i) != 0){
// 					std::cout << "Good point" << dest.pixel(j, i) << " " <<src.pixel(j, i) << std::endl;
					src.setPixel(j, i, to_overlay.pixel(j, i) );
				}
			}
		}
		
	}
}

#endif