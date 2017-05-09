//
// Created by Thisum Buddhika on 5/5/17.
//
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include "video_capture.h"
#include <time.h>

using namespace cv;

int processImage(Mat image);

struct ROW
{
    int minx;
    int miny;
    int maxx;
    int maxy;
};

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

//        time_t now = time(0);
        cap >> frame;
        if (frame.empty())
            break;

        processImage(frame);
	//time_t now = time(0);
//        char buf[100];
//        sprintf(buf, "test_%d.jpeg", now);
//        imwrite(buf, frame);

//        key = cv::waitKey( 30 );
    }
}


int processImage(Mat image)
{
//    Mat image = imread( "image.jpeg", 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    int elementSize = 3;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<std::vector<cv::Point> > contours2;
    std::vector<std::vector<cv::Point> > contours3;
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    int cols = image.cols;
    int rows = image.rows;
    Mat M = getRotationMatrix2D(Point(cols/2, rows/2), -90, 1);
    Mat frame = Mat::zeros( rows, cols, image.type() );
    Mat frame2 = Mat::zeros( rows, cols, image.type() );
    Mat frame3 = Mat::zeros( rows, cols, image.type() );
    warpAffine(image, frame, M, frame.size());
    Mat original = frame.clone();

    cvtColor(frame, frame, COLOR_BGR2HSV);
    inRange(frame, Scalar(90, 100, 30), Scalar(130, 255, 255), frame);
    medianBlur(frame, frame, 3);
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size( 2*elementSize + 1, 2*elementSize+1 ), Point(elementSize, elementSize));
    dilate(frame, frame, element);
    findContours(frame, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if(!contours.empty())
    {
        int i = 0;
        while(i<contours.size())
        {
            if( contourArea(contours[i]) < (cols * rows / 1000) )
            {
                contours.erase(contours.begin()+i);
            }
            else
            {
                i += 1;
            }
        }

        std::vector<ROW> data;
        for(i=0;i<contours.size();i++)
        {
            ROW newRow;
            newRow.minx = 10000; newRow.miny = 10000; newRow.maxx = 0; newRow.maxy = 0;
            for(int j =0; j< contours[i].size(); j++)
            {
                Point p = contours[i][j];
                if(p.x < newRow.minx)
                {
                    newRow.minx = p.x;
                }
                if(p.x > newRow.maxx)
                {
                    newRow.maxx = p.x;
                }
                if(p.y < newRow.miny)
                {
                    newRow.miny = p.y;
                }
                if(p.y > newRow.maxy)
                {
                    newRow.maxy = p.y;
                }
            }
            data.push_back(newRow);
        }

        cvtColor(original, frame2, COLOR_BGR2HSV);
        inRange(frame2, Scalar(140, 70, 100), Scalar(179, 255, 255), frame2);
        medianBlur(frame2, frame2, 3);
        dilate(frame2, frame2, element);
        findContours(frame2, contours2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        std::vector<ROW> data2;
        for(i=0;i<contours2.size();i++)
        {
            ROW newRow;
            newRow.minx = 10000; newRow.miny = 10000; newRow.maxx = 0; newRow.maxy = 0;
            for(int j =0; j< contours2[i].size(); j++)
            {
                Point p = contours2[i][j];
                if(p.x < newRow.minx)
                {
                    newRow.minx = p.x;
                }
                if(p.x > newRow.maxx)
                {
                    newRow.maxx = p.x;
                }
                if(p.y < newRow.miny)
                {
                    newRow.miny = p.y;
                }
                if(p.y > newRow.maxy)
                {
                    newRow.maxy = p.y;
                }
            }
            data2.push_back(newRow);
        }

        cvtColor(original, frame3, COLOR_BGR2HSV);
        inRange(frame3, Scalar(0, 100, 100), Scalar(10, 255, 255), frame3);
        medianBlur(frame3, frame3, 3);
        dilate(frame3, frame3, element);
        findContours(frame3, contours3, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        std::vector<ROW> data3;
        for(i=0;i<contours3.size();i++)
        {
            ROW newRow;
            newRow.minx = 10000; newRow.miny = 10000; newRow.maxx = 0; newRow.maxy = 0;
            for(int j =0; j< contours3[i].size(); j++)
            {
                Point p = contours3[i][j];
                if(p.x < newRow.minx)
                {
                    newRow.minx = p.x;
                }
                if(p.x > newRow.maxx)
                {
                    newRow.maxx = p.x;
                }
                if(p.y < newRow.miny)
                {
                    newRow.miny = p.y;
                }
                if(p.y > newRow.maxy)
                {
                    newRow.maxy = p.y;
                }
            }
            data3.push_back(newRow);
        }

        i = 0;
        while( i<contours.size())
        {
            bool f = false;
            for(int j=0; j<data2.size(); j++)
            {
                if( ( (data[i].minx <= data2[j].minx) and (data[i].maxx >= data2[j].maxx)
                      and (data[i].miny <= data2[j].miny) and (data[i].maxy >= data2[j].maxy))
                    or ( (data[i].minx <= data3[j].minx) and (data[i].maxx >= data3[j].maxx)
                         and (data[i].miny <= data3[j].miny) and data[i].maxy >= data3[j].maxy))
                {
                    f = true;
                    break;
                }
            }
            if(f)
            {
                i += 1;
            }
            else
            {
                contours.erase(contours.begin() + i);
                data.erase(data.begin() + i);
            }

        }

        if (contours.size()==1)
        {
            printf("Ring Found: \n");
            printf("Location: Top Left Corner: (%d, %d), Bottom Right Corner: (%d, %d)", data[0].minx, data[0].miny, data[0].maxx, data[0].maxy);
            rectangle(original, cv::Point(data[0].minx, data[0].miny), cv::Point(data[0].maxx, data[0].maxy), cv::Scalar(255, 255, 255) );
            line(original, cv::Point(data[0].minx, data[0].miny), cv::Point(data[0].maxx, data[0].maxy), cv::Scalar(255, 255, 255) );
           
            time_t now = time(0);
            char img_name[50];
            sprintf(img_name, "image_%d.jpeg", now);
	    imwrite(img_name, original);
        }
        else if( contours.empty() )
        {
            printf("No Ring Found");
        }
        else
        {
            printf("Multiple Rings Found");
        }
    }
    else
    {
        printf("No Rings Found");
    }

    return 0;
}
