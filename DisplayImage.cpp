#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <gst/gst.h>

using namespace cv;

int main(int argc, char** argv )
{
    GstElement *pipeline, *source, *sink, *encode;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    
      gst_init (&argc, &argv);

  source = gst_element_factory_make ("imxv4l2videosrc", "source");
  encode = gst_element_factory_make("jpegenc", "encode");
  sink = gst_element_factory_make ("filesink", "sink");
    
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);

    return 0;
}
