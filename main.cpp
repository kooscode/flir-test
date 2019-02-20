
#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"
#include <iostream>
#include <sstream> 

#include "opencv2/opencv.hpp"
#include "libterraclear/src/stopwatch.hpp"

namespace tc = terraclear;
namespace flir = Spinnaker;
namespace flir_api = Spinnaker::GenApi;
namespace flir_icam = Spinnaker::GenICam;

int main(int argc, char** argv) {

    try
    {
        //smartptr to dlir System..
        flir::SystemPtr flir_system = flir::System::GetInstance();

        //get flir cameras attached to system
        flir::CameraList flir_cams = flir_system->GetCameras();

        uint32_t cam_count = flir_cams.GetSize();
        if (cam_count > 0)
        {
            std::cout << "Found cams: " << flir_cams.GetSize() << std::endl ;    
    
            //get camera ptr at index 0
            flir::CameraPtr pcam = flir_cams.GetByIndex(0);
            
            //init camera
            pcam->Init();
            
            //get nodemaps
            //flir_api::INodeMap& nmTL = pcam->GetTLDeviceNodeMap();
            flir_api::INodeMap& nodemap = pcam->GetNodeMap();
                    
            //get camera in AcquisitionMode mode obj
            flir_api::CEnumerationPtr penum_acquisition = nodemap.GetNode("AcquisitionMode");
                
            //check in acquisition mode obj available and writable..
            if (!flir_api::IsAvailable(penum_acquisition) || (!flir_api::IsWritable(penum_acquisition)))
            {
                std::cout << "ERROR: Could not set camera into Acquisition mode." << std::endl;
            }
            else
            {
                //get continuous capture mode entry.
                flir_api::CEnumEntryPtr pentry_continuous = penum_acquisition->GetEntryByName("Continuous");
                
                //check continuous entry is available and readable.
                if (!flir_api::IsAvailable(pentry_continuous) || (!flir_api::IsReadable(pentry_continuous)))
                {
                    std::cout << "ERROR: Could not set camera into Continuous mode." << std::endl;
                }
                else
                {
                    // read integer value from entry node
                    int64_t value_continuous = pentry_continuous->GetValue();

                    // Set integer value from on acquisition mode
                    penum_acquisition->SetIntValue(value_continuous);    
                    
                    
                    //get ptr to FlipY node item
                    flir_api::CBooleanPtr ptrReverseY = nodemap.GetNode("ReverseY");
                    
                    if (!flir_api::IsAvailable(ptrReverseY) || (!flir_api::IsWritable(ptrReverseY)))
                    {
                        std::cout << "could not ReverseY" << std::endl;
                    }
                    else
                    {
                        ptrReverseY->SetValue(true);
                    }

                    
                    //get pixel format and change to BGR8 if needed..
                    flir::PixelFormatEnums flir_format = pcam->PixelFormat.GetValue();
                    if (flir_format != flir::PixelFormatEnums::PixelFormat_BGR8)
                    {
                        //get ptr to pixel format node item
                        flir_api::CEnumerationPtr ptrPixelFormat = nodemap.GetNode("PixelFormat");
                        if (!flir_api::IsAvailable(ptrPixelFormat) || (!flir_api::IsWritable(ptrPixelFormat)))
                        {
                            std::cout << "could not get PixelFormat" << std::endl;
                        }
                        else
                        {
                                // Retrieve the desired entry node from the enumeration node
                                flir_api::CEnumEntryPtr ptrPixelFormatBGR8 = ptrPixelFormat->GetEntryByName("BGR8");
                                if (!flir_api::IsAvailable(ptrPixelFormatBGR8) || (!flir_api::IsReadable(ptrPixelFormatBGR8)))
                                {
                                        std::cout << "Pixel BGR8 not available..." << std::endl;
                                }
                                else
                                {
                                        // Retrieve the integer value from the entry node
                                        int64_t pixelFormatBGR8 = ptrPixelFormatBGR8->GetValue();
                                        // Set integer as new value for enumeration node
                                        ptrPixelFormat->SetIntValue(pixelFormatBGR8);
                                        std::cout << "Pixel format set to " << ptrPixelFormat->GetCurrentEntry()->GetSymbolic() << std::endl;
                                }
                        }                        
                    }

                    

                    // begin acquisition
                    pcam->BeginAcquisition();
                    
                    //Create OpenCV Window
                    char window_name[] = "FLIR";
                    cv::namedWindow(window_name, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);// | WINDOW_AUTOSIZE);
                    
                    // fps timer
                    tc::stopwatch sw;
                    sw.start();
                    uint32_t fcount = 0;
                    uint32_t refresh_fps = 0;

                    tc::stopwatch sw2;
                    sw2.start();
                            
                    //camera loop..
                    for ( ;; )
                    {
                        //grab image from FLIR stack..
                        sw2.reset();
                        flir::ImagePtr image_ptr = pcam->GetNextImage();
                        uint32_t get_img_ms = sw2.get_elapsed_ms();
                        
                        //check if image was complete on grab..
                        if (image_ptr->GetImageStatus() > 0)
                        {
                            std::cout << "Image incomplete with image status " << image_ptr->GetImageStatus() << std::endl;
                        }
                        else
                        {
                            uint32_t xpad = image_ptr->GetXPadding();
                            uint32_t ypad = image_ptr->GetYPadding();
                            uint32_t width = image_ptr->GetWidth();
                            uint32_t height = image_ptr->GetHeight();

                            //image data contains padding. When allocating Mat container size, you need to account for the X,Y image data padding.

                            cv::Mat cvMat = cv::Mat(height + ypad, width + xpad, CV_8UC3, image_ptr->GetData(), image_ptr->GetStride());

                            fcount ++;
                            int64_t ms = sw.get_elapsed_ms();            
                            if (ms > 1000)
                            {
                                refresh_fps = fcount;                
                                fcount = 0;
                                sw.reset();
                            }
                            
                            //construct & paint fps and ms delay text.
                            std::stringstream fpsstr;
                            fpsstr << "fps: " << std::fixed << std::setprecision(0) << refresh_fps << ", " << get_img_ms << "ms";
                            cv::putText(cvMat, fpsstr.str(), cv::Point(10,50), cv::FONT_HERSHEY_PLAIN, 4,  cv::Scalar(0x00, 0x00, 0xff), 4);   
                            
                            //update onscreen img.
                            cv::imshow(window_name, cvMat);
                            
                            //release  buffers
                            image_ptr->Release();
                            
                        }
                        
                        
                        //wait for Key and quit on ESC button
                        int x = cv::waitKey(1);
                        if(x == 27) //ESC = 27
                        {
                            break;       
                        }
                    }                        
                    
                    //send acquisition
                    pcam->EndAcquisition();
                }
            }
            
            
            //spin down camera..
            pcam->DeInit();
        }
        else
        {
            std::cout << "ERROR: No Cameras found." << std::endl;
        }
        
        //clear cameras before releasing system instance..
        flir_cams.Clear();

        //release flir system.
        flir_system->ReleaseInstance();
    }
    catch (flir::Exception &e)
    {
        std::cout << "FLIR Exception: " << e.what() << std::endl;
    }
        
    return 0;
}

