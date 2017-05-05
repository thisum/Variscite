#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <gst/gst.h>
#include "gstreamer_file.h"
#include "video_capture.h"

using namespace cv;

int main(int argc, char** argv )
{
//    captureImage(argc, argv);
    captureImageFromVideo();
}
