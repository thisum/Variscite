//
// Created by Thisum Buddhika on 5/5/17.
//
#include <opencv2/opencv.hpp>
#include <gst/gst.h>
#include "video_capture.h"

using namespace cv;

void captureImageFromVideo()
{
    printf("starting video");
    VideoCapture cap("v4l2src ! video/x-raw, framerate=30/1, width=640, height=480, format=RGB ! videoconvert ! appsink");

    if(!cap.isOpened())  // check if we succeeded
        printf("camera cannot be opened \n");

    else
        printf("camera opened");
}
