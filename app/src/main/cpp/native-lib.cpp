#include "com_example_wma_opencvtest_MainActivity.h"
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>

#include <fcntl.h>


#include <android/log.h>
#include <stdio.h>
#include <sys/types.h>


#include <sys/stat.h>
#include <time.h>




#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <opencv2/imgcodecs.hpp>
#define SERVER_PORT 9234
#define BUFF_LEN 64000
#define SERVER_IP "127.0.0.1"
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <dlib/dnn.h>
#include <dlib/gui_widgets.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <iostream>
#include <fstream>


#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>



#include <stdlib.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>





#define TAG "JNITEST"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)


using namespace dlib;
using namespace std;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using ares      = relu<residual<block,N,affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block,N,affine,SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128,avg_pool_everything<
        alevel0<
                alevel1<
                        alevel2<
                                alevel3<
                                        alevel4<
                                                max_pool<3,3,2,2,relu<affine<con<32,7,7,2,2,
                                                        input_rgb_image_sized<150>
                                                >>>>>>>>>>>>;



using namespace cv;



frontal_face_detector detector;
shape_predictor sp;
anet_type net;
double knowDes[128] = {};


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {


    LOGI("---------------INIT ---------");

    detector = get_frontal_face_detector();
    deserialize("/sdcard/shape_predictor_5_face_landmarks.dat") >> sp;
    deserialize("/sdcard/dlib_face_recognition_resnet_model_v1.dat") >> net;
    return JNI_VERSION_1_4;
}


void startUDP()
{

    LOGI("---------------UDP start it ---------");
    int server_fd, ret;
    struct sockaddr_in ser_addr;

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    LOGI("---------------socket it ----%d-----",server_fd);
    if(server_fd < 0)
    {

        LOGI("---------------create socket fail ---------");

    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(SERVER_PORT);

    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));

    if(ret < 0)
    {
        LOGI("---------------socket bind fail ---------");

    } else{
        LOGI("---------------socket bind %d ---------",ret);

    }



    char buf[BUFF_LEN];
    socklen_t len;
    int count;
    struct sockaddr_in clent_addr;
    memset(&clent_addr, 0, sizeof(clent_addr));
    socklen_t addrlen = sizeof(clent_addr);
    memset(buf, 0, BUFF_LEN);
    for (;;) {


        LOGI("---------------receive data ---------");

          recvfrom(server_fd, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr, &addrlen);
         int recvlen = recvfrom(server_fd, buf, BUFF_LEN, 0, (struct sockaddr *)&clent_addr, &addrlen);
       // count = recvfrom(server_fd, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr, &addrlen);


        LOGI("---------------UDP 666 ---------");
        if(count == -1)
        {

            LOGI("---------------recieve data fail! ---------");

        }

        LOGI("---------------client:%d\n",sizeof(buf));

        int image_ptr = 0;
        cv::Mat image = cv::Mat::zeros(120,160, CV_8UC3);
        for (int i = 0;  i < 120; ++i) {
            for (int j = 0; j < 160; ++j) {

                image.at<cv::Vec3b>(i,j) = cv::Vec3b(buf[image_ptr+0],
                                                     buf[image_ptr+1],
                                                     buf[image_ptr+2]);

                image_ptr += 3;
            }
        }



        std::vector<matrix<rgb_pixel>> faces;
        cv_image<bgr_pixel> cimg(image);


        for (auto face : detector(cimg))
        {

            cout << "face descriptor for one face: " << endl;
            auto shape = sp(cimg, face);
            matrix<rgb_pixel> face_chip;
            extract_image_chip(cimg, get_face_chip_details(shape,150,1), face_chip);
            faces.push_back(move(face_chip));
        }

        if (faces.size() == 0)
        {
            LOGI("---------------no detect face---------");

        }else{

            LOGI("---------------detect face---------");
            std::vector<matrix<float,0,1>> face_descriptors = net(faces);

            float res[128] = {};
            int index = 0;
            cout << "---------------------" << endl;
            for (auto item : trans(face_descriptors[0]))
            {
                res[index++] =item;
                cout << item << endl;
                LOGI("item is %f \n",item);
            }

            sendto(server_fd, res, sizeof(res), 0, (struct sockaddr*)&clent_addr, addrlen);


        }






    }




}





extern "C" JNIEXPORT void
JNICALL Java_com_example_wma_opencvtest_MainActivity_getEdge
        (JNIEnv *env, jobject obj, jobject bitmap) {
    AndroidBitmapInfo info;
    void *pixels;

    /**
     * let get detect
     *
     */
//    std::thread myThread (startUDP);


    startUDP();

















//    LOGI("----------------------------------------- start");
//
//    using namespace std::chrono;
//    high_resolution_clock::time_point t1 = high_resolution_clock::now();
//
//    matrix<rgb_pixel> img;
//    load_image(img, "/sdcard/face2.jpg");
//
//    std::vector<matrix<rgb_pixel>> faces;
//    for (auto face : detector(img))
//    {
//        auto shape = sp(img, face);
//        matrix<rgb_pixel> face_chip;
//        extract_image_chip(img, get_face_chip_details(shape,150,0.25), face_chip);
//        faces.push_back(move(face_chip));
//
//    }
//
//    if (faces.size() == 0)
//    {
//        LOGI("---------------no detect face---------");
//
//    }else{
//        LOGI("---------------detect face---------");
//
//    }
//
//
//    std::vector<matrix<float,0,1>> know_face_descriptors = net(faces);
//
//
//    int index = 0;
//    for (auto item : trans(know_face_descriptors[0]))
//    {
//        knowDes[index++] =item;
//        LOGI("item is %f \n",item);
//
//    }
//
//    high_resolution_clock::time_point t2 = high_resolution_clock::now();
//
//    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
//
//    LOGI("It took me  %f  second. ",time_span.count());
//
//
//
//    LOGI("----------------------------------------- end");





    CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
    CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
              info.format == ANDROID_BITMAP_FORMAT_RGB_565);
    CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
    CV_Assert(pixels);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat temp(info.height, info.width, CV_8UC4, pixels);
        Mat gray;
        cvtColor(temp, gray, COLOR_RGBA2GRAY);
        Canny(gray, gray, 125, 225);
        cvtColor(gray, temp, COLOR_GRAY2RGBA);
    } else {
        Mat temp(info.height, info.width, CV_8UC2, pixels);
        Mat gray;
        cvtColor(temp, gray, COLOR_RGB2GRAY);
        Canny(gray, gray, 125, 225);
        cvtColor(gray, temp, COLOR_GRAY2RGB);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}




