#include <unistd.h>
#include <iostream>
#include <sstream> 

//opencv
#include "opencv2/opencv.hpp"

//libterraclear
#include "libterraclear/src/stopwatch.hpp"

#ifndef TC_USE_BLACKFLY
    #define TC_USE_BLACKFLY
#endif
#include "libterraclear/src/camera_flir_blackfly.hpp"

namespace tc = terraclear;
namespace flir = Spinnaker;
namespace flir_api = Spinnaker::GenApi;
namespace flir_icam = Spinnaker::GenICam;

typedef std::pair<bool, int > VectorResult ;

template < typename T>
std::pair<bool, int > GetVectorElementIndex(const std::vector<T>  & vecOfElements, const T  & element)
{
	std::pair<bool, int > result;
 
	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
 
	if (it != vecOfElements.end())
	{
		result.second = distance(vecOfElements.begin(), it);
		result.first = true;
	}
	else
	{
		result.first = false;
		result.second = -1;
	}
 
	return result;
}

int main(int argc, char** argv) 
{
    
    uint32_t cam_index = 0;    
    std::string cam_serial = "";

    //get camera list.
    std::vector<std::string> cams = tc::camera_flir_blackfly::get_cameras();
    
    //Get cam count
    uint32_t cam_count = cams.size();
    
    //print all connected cams serials.
    std::cout << "Found " << cam_count << " FLIR cameras:" << std::endl;
    for (auto cam : cams)
        std::cout << "\t" << cam << std::endl;

    //line spacer..
    std::cout << std::endl;

    //If serial supplied on cmdline params, find index of camera by serial
    if (argc > 1)
    {
        cam_serial = argv[1];
        VectorResult vr = GetVectorElementIndex(cams, cam_serial);
        if (vr.first)
        {
            cam_index = vr.second;
        }
        else
        {
            std::cout << std::endl << "FLIR camera with serial " << cam_serial << " not found." << std::endl;
            return -1;
        }
    }

    //load camera by index.    
    if (cam_count > cam_index)
    {
        //grab 
        cam_serial = cams.at(cam_index);
        std::cout << "Connecting to camera " << cam_index << " with serial# " << cam_serial << std::endl << std::flush;
    }
    else
    {
        std::cout << "No FLIR cameras found." << std::endl;
        return -1;
    }
    
    tc::camera_flir_blackfly poe_cam(cam_serial);
    
    //Create OpenCV Window
    std::string window_name = "FLIR";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO | cv::WINDOW_AUTOSIZE);
    cv::Mat cam_img = poe_cam.getRGBFrame();

    uint32_t retry_count = 0;
    while (true)
    {
        if (retry_count > 10)
            return -1;
        
        poe_cam.update_frames();
        cam_img = poe_cam.getRGBFrame();
        if ((cam_img.rows > 0) && (cam_img.cols > 0))
        {
            break;
        }
        else
        {
            retry_count ++;
            usleep(250000);
        }

        std::cout << "." << std::flush;
    }
    std::cout << std::endl;

    //timer
    tc::stopwatch sw2;
    sw2.start();
    uint64_t total_ms = 0;
    uint64_t total_lag_ms = 0;
    uint32_t cam_fps = 0;
    uint32_t frame_cnt = 0;
    float avg_ms = 0.0f;
    float avg_lag_ms = 0.0f;

    //camera loop..
    for ( ;; )
    {
        //grab image from FLIR stack..
        sw2.reset();
        poe_cam.update_frames();
        total_lag_ms += sw2.get_elapsed_ms();
        
        //construct & paint fps and ms delay text.
        std::stringstream fpsstr;
        fpsstr << "fps: " << std::fixed << std::setprecision(0) << cam_fps << ", frame lag T:" << avg_ms << "ms, A:" << avg_lag_ms << "ms";
        cv::putText(cam_img, fpsstr.str(), cv::Point(10,50), cv::FONT_HERSHEY_PLAIN, 2,  cv::Scalar(0x00, 0x00, 0xff), 2);   

        //update onscreen img.
        cv::imshow(window_name, cam_img);

        //wait for Key and quit on ESC button
        int x = cv::waitKey(1);
        if(x == 27) //ESC = 27
        {
            break;       
        }        
        
        //calc avg fps over 100 frames..
         frame_cnt++;
         total_ms += sw2.get_elapsed_ms();            
         if (frame_cnt >= 60)
         {
             avg_ms = (float)total_ms / (float)frame_cnt;
             avg_lag_ms = (float)total_lag_ms / (float)frame_cnt;
             cam_fps = 1000.0f / avg_ms;
             frame_cnt = 0;
             total_ms = 0;
             total_lag_ms = 0;
         }

    }    
    
  
    return 0;
}

