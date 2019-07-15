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
#include "libterraclear/src/camera_flir_blackfly_system.hpp"
#include "libterraclear/src/camera_flir_blackfly.hpp"
#include "libterraclear/src/camera_async.hpp"


namespace tc = terraclear;


typedef std::pair<bool, int > VectorResult ;

template < typename T>
std::pair<bool, int > GetVectorElementIndex(const std::vector<T>  & vecOfElements, const T  & element)
{
	std::pair<bool, int > result;
 
	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
 
	if (it != vecOfElements.end())
	{
		result.second = std::distance(vecOfElements.begin(), it);
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
    //setting up FLIR parameters.
    tc::camera_flir_blackfly::flir_settings blkfly_settings;
    blkfly_settings.pixel_format =  tc::FLIR_PixelFormat::PixelFormat_BayerRG8;
    blkfly_settings.width = 720;//1440;//720;
    blkfly_settings.height = 540;//1080;//540;
    blkfly_settings.bin_vertical = 2;//1;//2;
    blkfly_settings.bin_horizontal = 2;//1;//2;
    blkfly_settings.flip_y = false;
    blkfly_settings.fps = 125; //71;//125;
    blkfly_settings.exposure_auto = false;
    blkfly_settings.exposure_time = 987625 / blkfly_settings.fps;
    
    uint32_t cam_index = 0;    
    std::string cam_serial = "19244277";
    
    tc::camera_flir_blackfly_system flir_system;
    
    //get camera list.
    std::vector<std::string> cams = flir_system.get_cameras();
    
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

//    //get camera serial by index.    
//    if (cam_count > cam_index)
//    {
//        //grab 
//        cam_serial = cams.at(cam_index);
//        std::cout << "Connecting to FLIR camera " << cam_index << " with serial# " << cam_serial << std::endl << std::flush;
//    }
//    else
//    {
//        std::cout << "No FLIR cameras found." << std::endl;
//        return -1;
//    }
    
    //create camera by serial
    tc::camera_flir_blackfly poe_cam(&flir_system, blkfly_settings, cam_serial);
    
    //Create OpenCV Window
    std::string window_name = "FLIR";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO | cv::WINDOW_AUTOSIZE);

    cv::Mat camera_mat;

    //async cam..
    tc::camera_async my_cam(&poe_cam, 78, false);  
    my_cam.thread_start("cam");

    
    uint32_t retry_count = 0;
    while (true)
    {
        if (retry_count > 10)
            return -1;
        
//        poe_cam.update_frames();
//        cam_img = poe_cam.getRGBFrame();
        camera_mat = my_cam.get_ImageBuffer();
        
        if ((camera_mat.rows > 0) && (camera_mat.cols > 0))
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
    uint32_t screen_fps = 0;
    uint32_t frame_cnt = 0;
    float frame_ms = 0.0f;
    float avg_lag_ms = 0.0f;

   
    //camera loop..
    for ( ;; )
    {
        //grab image from FLIR stack..
        sw2.reset();

//        poe_cam.update_frames();

        camera_mat = my_cam.get_ImageBuffer();

        frame_ms = sw2.get_elapsed_ms();
        total_lag_ms += sw2.get_elapsed_ms();

        //wait for Key and quit on ESC button
        int x = 0;

        //construct & paint fps and ms delay text.
        std::stringstream fpsstr;
        fpsstr << "cam: " << my_cam.get_fps() << ", " ;
        fpsstr << "scrn: " << std::fixed << std::setprecision(0) << screen_fps << ", frame lag T:" << frame_ms << "ms, A:" << avg_lag_ms << "ms";
        cv::putText(camera_mat, fpsstr.str(), cv::Point(10,50), cv::FONT_HERSHEY_PLAIN, 2,  cv::Scalar(0x00, 0x00, 0xff), 2);   

        //check if rows are blank.
        cv::Scalar sum_channels = cv::sum(camera_mat(cv::Rect(0, camera_mat.rows-3, camera_mat.cols, 2)));
        bool skip_frame = (sum_channels[0] + sum_channels[1] +  sum_channels[2] == 0);
        
        //update onscreen img.
        cv::imshow(window_name, camera_mat);
        x = cv::waitKey(1);

        if(x == 27) //ESC = 27
        {
            break;       
        }        

        //calc avg fps over 100 frames..
         frame_cnt++;
         total_ms += sw2.get_elapsed_ms();            
         if (frame_cnt >= 60)
         {
             avg_lag_ms = (float)total_lag_ms / (float)frame_cnt;
             screen_fps = 1000.0f / frame_ms;
             frame_cnt = 0;
             total_ms = 0;
             total_lag_ms = 0;
         }

        

    }    
    
  
    return 0;
}

