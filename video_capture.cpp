//
// Created by Thisum Buddhika on 5/5/17.
//
#include <opencv2/opencv.hpp>
#include <gst/gst.h>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include "video_capture.h"
#include <time.h>

using namespace cv;

void captureImageFromVideo()
{
    printf("starting video");
//    VideoCapture cap("imxv4l2videosrc device=\"/dev/video0\" ! autovideosink");
//    VideoCapture cap("imxv4l2videosrc ! video/x-raw ! appsink");
//    VideoCapture cap("imxv4l2videosrc device=\"/dev/video0\" ! appsink");
//    VideoCapture cap("mfw_v4lsrc ! ffmpegcolorspace ! video/x-raw-rgb ! appsink");
    VideoCapture cap("imxv4l2videosrc device=\"/dev/video0\" ! videoconvert ! appsink");


    if (!cap.isOpened()) {
        printf("=ERR= can't create video capture\n");
        return;
    }

    cv::Mat frame;
    int key;

    while (true) {

        time_t now = time(0);
        cap >> frame;
        if (frame.empty())
            break;

        char buf[100];
        sprintf(buf, "test_%d.jpeg", now);
        imwrite(buf, frame);

        key = cv::waitKey( 30 );
    }
}
