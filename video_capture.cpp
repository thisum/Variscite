//
// Created by Thisum Buddhika on 5/5/17.
//
#include <opencv2/opencv.hpp>
#include <gst/gst.h>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include "video_capture.h"

using namespace cv;

void captureImageFromVideo()
{
    printf("starting video");
//    VideoCapture cap("imxv4l2videosrc device=\"/dev/video0\" ! autovideosink");
//    VideoCapture cap("imxv4l2videosrc ! video/x-raw ! appsink");
//    VideoCapture cap("imxv4l2videosrc device=\"/dev/video0\" ! appsink");
//    VideoCapture cap("mfw_v4lsrc ! ffmpegcolorspace ! video/x-raw-rgb ! appsink");
    VideoCapture cap("imxv4l2videosrc device=\"/dev/video0\" ! videoconvert ! appsink");


//    cv::VideoCapture cap("v4l2src ! video/x-raw, framerate=30/1, width=640, height=480, format=RGB ! videoconvert ! appsink");
//    if (!cap.isOpened()) {
//        printf("=ERR= can't create video capture\n");
//        return;
//    }
//
//    // second part of sender pipeline
//    cv::VideoWriter writer;
//    writer.open("appsrc ! videoconvert ! x264enc noise-reduction=10000 tune=zerolatency byte-stream=true threads=4 ! mpegtsmux ! udpsink host=localhost port=9999"
//            , 0, (double)30, cv::Size(640, 480), true);
//    if (!writer.isOpened()) {
//        printf("=ERR= can't create video writer\n");
//        return;
//    }
//
//    cv::Mat frame;
//    int key;
//
//    while (true) {
//
//        cap >> frame;
//        if (frame.empty())
//            break;
//
//        imwrite("test_1.jpeg", frame);
//
//        writer << frame;
//        key = cv::waitKey( 30 );
//    }
}
