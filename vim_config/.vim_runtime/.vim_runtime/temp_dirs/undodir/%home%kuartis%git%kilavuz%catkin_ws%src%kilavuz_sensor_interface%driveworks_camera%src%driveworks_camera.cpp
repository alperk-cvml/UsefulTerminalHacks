Vim�UnDo� ��F��ذ�_�8Յ�@�g�Y2t.}i��  �                                   ^� �    _�                     �        ����                                                                                                                                                                                                                                                                                                                                                             ^� b     �              �       #include "json.hpp"       2#include <driveworks_camera/driveworks_camera.hpp>           #ifdef ROS_LOGGING   #include <ros/console.h>   #endif           #ifndef NO_OPENCV_NODE      #include <opencv2/imgproc.hpp>     #ifdef SHOW_FRAMES_DEBUG      #include <opencv2/highgui.hpp>     #endif   #endif       #define STREAM_TIMEOUT 30 // ms   ##define READ_TIMEOUT   500000 // us       using namespace nlohmann;       (dwLogCallback getConsoleLoggerCallback()   {       K        return [](dwContextHandle_t, dwLoggerVerbosity, const char *msg) {    #ifdef ROS_LOGGING   (        ROS_DEBUG("DriveWorks: %s",msg);   #else   '        printf("DriveWorks: %s", msg);    #endif   
        };   }       %DriveWorksCamera::DriveWorksCamera(){       initializeSdk();   ,    _previous_time_point = myclock_t::now();   }       &DriveWorksCamera::~DriveWorksCamera(){       dwSAL_release(_sal);       dwRelease(_sdk);   }       'void DriveWorksCamera::initializeSdk(){   4    dwLogger_initialize(getConsoleLoggerCallback());   )    dwLogger_setLogLevel(DW_LOG_VERBOSE);          0    // initialize SDK context, using data folder   '    dwContextParameters sdkParams = {};   7    memset(&sdkParams, 0, sizeof(dwContextParameters));          B    dwStatus status = dwInitialize(&_sdk, DW_VERSION, &sdkParams);   *    logIfError(status,0,"initialize sal");       +    status = dwSAL_initialize(&_sal, _sdk);   *    logIfError(status,0,"initialize sal");   }       Abool DriveWorksCamera::parseParams(std::string config_file_name){       json config;   !    if (config_file_name.empty())           return false;       try{   4        std::ifstream file_stream(config_file_name);           file_stream >> config;       }       catch(...)       {   #ifdef ROS_LOGGING   4       ROS_ERROR("Error cannot open config_file !");   #else   D       std::cout << "Error cannot open config_file !" << std::endl;    #endif           return false;       }       ,    int num_cameras = config["num_cameras"];       *    for (int i = 1; i<= num_cameras; i++){   +        std::string si = std::to_string(i);   '        Camera* camera = new Camera() ;   <        std::string camera_type = config[si]["camera_type"];           bool is_raw;    &        dwSensorParams sensorParams{};       1        if (camera_type.compare("virtual") == 0){       <            std::string file_name = config[si]["file_name"];       q            if ( (file_name.find(".raw") != std::string::npos) || (file_name.find(".lraw") != std::string::npos)    r                                                               || (file_name.find(".xraw") != std::string::npos)){   &                camera->is_raw = true;   *                // TODO: handle raw sensor               }   "            if (_usePinnedMemory){   2                file_name += ",usePinnedMemory=1";               }   7            sensorParams.protocol   = "camera.virtual";   6            std::string params = "video=" + file_name;   7            char *cstr = new char[params.length() + 1];   )            strcpy(cstr, params.c_str());   +            sensorParams.parameters = cstr;   	        }   2        else if (camera_type.compare("usb") == 0){   5            bool pointgrey = config[si]["pointgrey"];   .            int device = config[si]["device"];                   if (pointgrey)   =                sensorParams.protocol   = "camera.pointgrey";               else   7                sensorParams.protocol   = "camera.usb";       8            std::string parameters = "device=" + device;   9            sensorParams.parameters = parameters.c_str();   4            // TODO: is "mode=" parameter necessary?   	        }   3        else if (camera_type.compare("gmsl") == 0){   W            std::string parameterString = std::string("output-format=yuv,fifo-size=3");   :            std::string isp_mode = config[si]["isp-mode"];   4            std::string model = config[si]["model"];   B            std::string camera_group = config[si]["camera-group"];   3            bool slave = config[si]["tegra-slave"];   ;            bool custom_board = config[si]["custom-board"];   D            std::string custom_config = config[si]["custom-config"];   D            std::string custom_plugin = config[si]["custom-plugin"];           c            parameterString             += std::string(",isp-mode=") + "yuv420-uint8"; // isp_mode;   m            parameterString             += std::string(",camera-type=") + "ar0231-rccb-bae-sf3324" ;// model;   _            parameterString             += std::string(",camera-group=") + "a";// camera_group;   e            parameterString             += std::string(",slave=") +"0";// (slave? std::string("1") :    n//            parameterString             += std::string(",custom-board=") +"0";// (slave? std::string("1") :    (//                    std::string("0"));                   if (custom_board)               {   S                // it's a custom board, use the board specific extra configurations   A                parameterString             += ",custom-board=1";       B                // pass an extra set of parameter in custom-config   M                sensorParams.auxiliarydata           = custom_config.c_str();               }       '            if (!custom_plugin.empty())               {   \//                parameterString += std::string(",custom-plugin=") + custom_plugin.c_str();               }       @            char *cstr = new char[parameterString.length() + 1];   2            strcpy(cstr, parameterString.c_str());   +            sensorParams.parameters = cstr;   $                                       >            sensorParams.protocol             = "camera.gmsl";   	        }   1        else if (camera_type.compare("ip") == 0){   '            // TODO: ip camera settings   	        }       R        dwStatus status = dwSAL_createSensor(&camera->sensor, sensorParams, _sal);   $        logIfError(status,i,"open");   #        if (status != DW_SUCCESS) {               return false;   	        }       P        dwSensorCamera_getSensorProperties(&camera->properties, camera->sensor);       ~        status = dwSensorCamera_getImageProperties(&camera->imageProperties,DW_CAMERA_OUTPUT_NATIVE_PROCESSED,camera->sensor);   2        logIfError(status,i,"getImageProperties");       �        status = dwSensorCamera_getImageProperties(&camera->targetImageProperties,DW_CAMERA_OUTPUT_NATIVE_PROCESSED,camera->sensor);   3        logIfError(status,i,"getImageProperties2");       �        status = dwSensorCamera_getImageProperties(&camera->cpuImageProperties,DW_CAMERA_OUTPUT_NATIVE_PROCESSED,camera->sensor);   3        logIfError(status,i,"getImageProperties3");       9        camera->framerate = camera->properties.framerate;       ;        camera->targetImageProperties.type = DW_IMAGE_CUDA;   I        camera->targetImageProperties.format = DW_IMAGE_FORMAT_RGB_UINT8;   O        dwImage_create(&camera->imageRGB, camera->targetImageProperties, _sdk);       7        camera->cpuImageProperties.type = DW_IMAGE_CPU;   F        camera->cpuImageProperties.format = DW_IMAGE_FORMAT_RGB_UINT8;   L        dwImage_create(&camera->imageCPU, camera->cpuImageProperties, _sdk);   #ifdef VIBRANTE   v        status = dwImageStreamer_initialize(&camera->streamerNVM2CUDA, &camera->imageProperties, DW_IMAGE_CUDA, _sdk);   :        logIfError(status,i,"dwImageStreamer_initialize");   #endif   {        status = dwImageStreamer_initialize(&camera->streamerCUDA2CPU, &camera->targetImageProperties, DW_IMAGE_CPU, _sdk);   :        logIfError(status,i,"dwImageStreamer_initialize");       #ifdef NO_OPENCV_NODE   �        camera->cv_buffer = (unsigned char*) malloc (camera->properties.resolution.y* camera->properties.resolution.x*3*sizeof(unsigned char) );   #else   j        camera->cv_mat= cv::Mat(camera->properties.resolution.y, camera->properties.resolution.x,CV_8UC3);   #endif   4        camera->publisher = config[si]["publisher"];   #ifdef ROS_LOGGING   ^        ROS_INFO("camera image %dx%d at %f fps, format %d\n", camera->properties.resolution.x,   g                   camera->properties.resolution.y, camera->framerate, camera->imageProperties.format);   #endif   W        camera->processPeriod = convertFrecuencyToPeriod(camera->properties.framerate);   #        _cameras.push_back(camera);       }       return true;   }       #ifdef NO_OPENCV_NODE   vvoid DriveWorksCamera::registerFrameReceivedHandler( void (* cameraFrameReceivedHandler)(unsigned char*,int,int,int)){   #else   hvoid DriveWorksCamera::registerFrameReceivedHandler( void (* cameraFrameReceivedHandler)(cv::Mat&,int)){   #endif   8    _onCameraFrameReceived = cameraFrameReceivedHandler;   }       \auto DriveWorksCamera::convertFrecuencyToPeriod(int loopsPerSecond) -> myclock_t::duration {   (    using ns = std::chrono::nanoseconds;           if (loopsPerSecond <= 0)   ,        return std::chrono::milliseconds(0);       else   >        return ns(static_cast<ns::rep>(1e9 / loopsPerSecond));   }              void DriveWorksCamera::start(){       _killed = false;   -    for (int i = 0; i< _cameras.size(); i++){   d        _capture_threads.push_back(std::thread(&DriveWorksCamera::runCamera, this, _cameras[i],i ));       }   }       void DriveWorksCamera::kill(){       _killed = true;   }       7void DriveWorksCamera::runCamera(Camera* camera,int i){       dwStatus status;       startCamera(camera,i);   !    // Periodically capture image       while (!_killed){   )            uint32_t cameraSiblingID = 0;   )            // TODO: calculate sibling ID   m            status = dwSensorCamera_readFrame(&camera->frame, cameraSiblingID, READ_TIMEOUT, camera->sensor);   .            logIfError(status,i,"read frame");       #ifdef VIBRANTE   r            status = dwSensorCamera_getImage(&camera->imageNV, DW_CAMERA_OUTPUT_NATIVE_PROCESSED , camera->frame);   -            logIfError(status,i,"get image");       \            status = dwImageStreamer_producerSend(camera->imageNV,camera->streamerNVM2CUDA);   9            logIfError(status,i,"nvm2cuda producerSend");       q            status = dwImageStreamer_consumerReceive(&camera->imageCUDA,STREAM_TIMEOUT,camera->streamerNVM2CUDA);   <            logIfError(status,i,"nvm2cuda consumerReceive");       n            status = dwImageStreamer_producerReturn(&camera->imageNV,STREAM_TIMEOUT,camera->streamerNVM2CUDA);   #else   t            status = dwSensorCamera_getImage(&camera->imageCUDA, DW_CAMERA_OUTPUT_NATIVE_PROCESSED , camera->frame);   -            logIfError(status,i,"get image");   #endif       @            status = dwSensorCamera_returnFrame(&camera->frame);   0            logIfError(status,i,"return frame");           U            status = dwImage_copyConvert(camera->imageRGB, camera->imageCUDA, _sdk );   /            logIfError(status,i,"copyConvert");       #ifdef VIBRANTE   a            status = dwImageStreamer_consumerReturn(&camera->imageCUDA,camera->streamerNVM2CUDA);   #endif       #ifdef PROFILING   S            std::cout << "--------------- New Frame ----------------" << std::endl;   C            auto begin = std::chrono::high_resolution_clock::now();   #endif       ]            status = dwImageStreamer_producerSend(camera->imageRGB,camera->streamerCUDA2CPU);   9            logIfError(status,i,"cuda2cpu producerSend");           #ifdef PROFILING   A            auto end = std::chrono::high_resolution_clock::now();   d            auto time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin);   d            std::cout << "producerSend:" << time_span.count()*1000 << " milliseconds." << std::endl;   #endif       p            status = dwImageStreamer_consumerReceive(&camera->imageCPU,STREAM_TIMEOUT,camera->streamerCUDA2CPU);   <            logIfError(status,i,"cuda2cpu consumerReceive");       #ifdef PROFILING   >            begin = std::chrono::high_resolution_clock::now();   #endif       #ifdef NO_OPENCV_NODE   [            dwImageToBuffer(camera->imageCPU,camera->cv_buffer,camera->cpuImageProperties);   #else   W            dwImageToCvMat(camera->imageCPU,camera->cv_mat,camera->cpuImageProperties);   #endif       #ifdef PROFILING   <            end = std::chrono::high_resolution_clock::now();   _            time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin);   g            std::cout << "dwImageToCvMat: " << time_span.count()*1000 << " milliseconds." << std::endl;   #endif       `            status = dwImageStreamer_consumerReturn(&camera->imageCPU,camera->streamerCUDA2CPU);   2            logIfError(status,i,"consumerReturn");           o            status = dwImageStreamer_producerReturn(&camera->imageRGB,STREAM_TIMEOUT,camera->streamerCUDA2CPU);   2            logIfError(status,i,"producerReturn");       #ifdef PROFILING   >            begin = std::chrono::high_resolution_clock::now();   #endif       (            if (_onCameraFrameReceived){   #ifdef NO_OPENCV_NODE   �                _onCameraFrameReceived(camera->cv_buffer,i,camera->cpuImageProperties.width, camera->cpuImageProperties.height);   #else   9                _onCameraFrameReceived(camera->cv_mat,i);   #endif               }       #ifdef PROFILING   <            end = std::chrono::high_resolution_clock::now();   _            time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin);   o            std::cout << "_onCameraFrameReceived: " << time_span.count()*1000 << " milliseconds." << std::endl;   >            begin = std::chrono::high_resolution_clock::now();   #endif           -            //TODO call frame receive handler   v            auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(myclock_t::now() - _previous_time_point);   4            _previous_time_point = myclock_t::now();   .            if (diff < camera->processPeriod){   H                std::this_thread::sleep_for(camera->processPeriod-diff);               }   #ifdef PROFILING   h            std::cout << "time of one cycle :" << diff.count()/1000000 << " milliseconds." << std::endl;   #endif       }       finalizeCamera(camera,i);   }       Rvoid DriveWorksCamera::logIfError(dwStatus status, int device,std::string action){       if (status != DW_SUCCESS) {   #ifdef ROS_LOGGING   R       ROS_ERROR("Error cannot %s camera %d. %d", action.c_str(), device, status);   #else   `       std::cout << "Error cannot "<< action<< " camera "<<device<< ". " << status << std::endl;   #endif       }   (    if (status == DW_SAL_SENSOR_ERROR) {   %	    ROS_INFO("DW_SAL_SENSOR_ERROR");       }       if (status == DW_FAILURE) {   	    ROS_INFO("DW_FAILURE");       }   }           9void DriveWorksCamera::startCamera(Camera* camera,int i){   5    dwStatus status = dwSensor_start(camera->sensor);   !    logIfError(status,i,"start");       status = DW_NOT_READY;       do {   [        status = dwSensorCamera_readFrame(&camera->frame, 0, READ_TIMEOUT, camera->sensor);   %    } while (status == DW_NOT_READY);   !    logIfError(status,i,"start");       if (status != DW_SUCCESS) {   D        throw std::runtime_error("Cameras did not start correctly");       }   8    status = dwSensorCamera_returnFrame(&camera->frame);   (    logIfError(status,i,"return frame");   }       <void DriveWorksCamera::finalizeCamera(Camera* camera,int i){   4    dwStatus status = dwSensor_stop(camera->sensor);        logIfError(status,i,"stop");   1    status = dwSAL_releaseSensor(camera->sensor);   #    logIfError(status,i,"release");   #ifdef NO_OPENCV_NODE       free(camera->cv_buffer);   #else       camera->cv_mat.release();   #endif   6    dwImageStreamer_release(camera->streamerCUDA2CPU);   #ifdef VIBRANTE   6    dwImageStreamer_release(camera->streamerNVM2CUDA);   #endif   (    dwSAL_releaseSensor(camera->sensor);       delete camera;   }       kvoid DriveWorksCamera::getPublishers(std::vector<std::string>& publishers, std::vector<float>& framerates){   "    for (auto camera :  _cameras){   0        publishers.push_back(camera->publisher);   0        framerates.push_back(camera->framerate);       }   }       #ifdef NO_OPENCV_NODE   xvoid DriveWorksCamera::dwImageToBuffer(dwImageHandle_t source, unsigned char* array, dwImageProperties imageProperties){       // See example at:   V    // DRIVE_SW_10.0_References/DRIVE_Software/DRIVE_AV_doxy_files/image_usecase5.html       dwImageCPU *imageCPU;   8    dwStatus status = dwImage_getCPU(&imageCPU, source);   #    logIfError(status,0,"get_cpu");       size_t planeCount;   >    dwImage_getPlaneCount(&planeCount, imageCPU->prop.format);           ;    for (uint32_t plane = 0; plane < planeCount; ++plane) {   D        uint32_t pitch = imageCPU->pitch[0] / imageProperties.width;   C        memcpy(array ,imageCPU->data[plane], imageProperties.height   E                    *imageProperties.width*3*sizeof(unsigned char) );       }       }   #else   wvoid DriveWorksCamera::dwImageToCvMat(dwImageHandle_t source, cv::Mat& destination, dwImageProperties imageProperties){       // See example at:   V    // DRIVE_SW_10.0_References/DRIVE_Software/DRIVE_AV_doxy_files/image_usecase5.html       dwImageCPU *imageCPU;   8    dwStatus status = dwImage_getCPU(&imageCPU, source);   #    logIfError(status,0,"get_cpu");       size_t planeCount;   >    dwImage_getPlaneCount(&planeCount, imageCPU->prop.format);   =    unsigned char* array = (unsigned char*) destination.data;       ;    for (uint32_t plane = 0; plane < planeCount; ++plane) {   D        uint32_t pitch = imageCPU->pitch[0] / imageProperties.width;   ?        for (uint32_t i = 0; i < imageProperties.height; ++i) {   �            memcpy(array+(destination.step*i),imageCPU->data[plane]+(i * imageCPU->pitch[plane]), imageProperties.width*pitch*sizeof(unsigned char) );   	        }       }       #ifdef SHOW_FRAMES_DEBUG       cv::Mat display   :    cv::cvtColor(destination, display, cv::COLOR_BGR2RGB);   .    cv::imshow("converted_image",display);           cv::waitKey(1);   #endif   }   #endif5�_�                    �        ����                                                                                                                                                                                                                                                                                                                            �          �          V       ^� �     �   �   �          8    _onCameraFrameReceived = cameraFrameReceivedHandler;   }5�_�                    �        ����                                                                                                                                                                                                                                                                                                                            �          �          V       ^� �     �   �   �  �    �   �   �  �    5�_�                     �       ����                                                                                                                                                                                                                                                                                                                            �          �          V       ^� �    �   �   �  �    �   �   �  �    5��