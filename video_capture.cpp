//
// Created by Thisum Buddhika on 5/5/17.
//
#include <opencv2/opencv.hpp>
#include <gst/gst.h>

void captureImageFromVideo()
{
    VideoCapture cap("mfw_v4lsrc ! ffmpegcolorspace ! video/x-raw-rgb ! appsink");

    if(!cap.isOpened())  // check if we succeeded
        printf("camera cannot be opened \n");

    else
        printf("camera opened");
}
