//
// Created by Thisum Buddhika on 5/5/17.
//
#include <opencv2/opencv.hpp>
#include <gst/gst.h>
#include "video_capture.h"

void captureImageFromVideo()
{
    VideoCapture cap("imxv4l2videosrc device=\"/dev/video0\" ! autovideosink ");

    if(!cap.isOpened())  // check if we succeeded
        printf("camera cannot be opened \n");

    else
        printf("camera opened");
}
