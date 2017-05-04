#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <gst/gst.h>

using namespace cv;

struct ROW
{
    int minx;
    int miny;
    int maxx;
    int maxy;
};

int takeImage(int argc, char** argv )
{
    
    GstElement *pipeline, *source, *sink, *encode;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;


    gst_init (&argc, &argv);

    source = gst_element_factory_make ("imxv4l2videosrc", "source");
    encode = gst_element_factory_make("jpegenc", "encode");
    sink = gst_element_factory_make ("filesink", "sink");


    pipeline = gst_pipeline_new ("test-pipeline");

    if (!pipeline || !source || !sink || !encode) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    gst_bin_add_many (GST_BIN (pipeline), source, encode, sink, NULL);
    if (gst_element_link_many (source, encode, sink, NULL) != TRUE) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (pipeline);
        return -1;
    }


    g_object_set (source, "device", "/dev/video0", NULL);
    g_object_set (source, "num-buffers", 1, NULL);
    g_object_set (sink, "location", "test.jpeg", NULL);


    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return -1;
    }


    bus = gst_element_get_bus (pipeline);
    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,(GstMessageType)( GST_MESSAGE_ERROR | GST_MESSAGE_EOS));


    if (msg != NULL) {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE (msg)) {
          case GST_MESSAGE_ERROR:
            gst_message_parse_error (msg, &err, &debug_info);
            g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
            g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
            g_clear_error (&err);
            g_free (debug_info);
            break;
          case GST_MESSAGE_EOS:
            g_print ("End-Of-Stream reached.\n");
            break;
          default:
            /* We should not reach here because we only asked for ERRORs and EOS */
            g_printerr ("Unexpected message received.\n");
            break;
        }
        gst_message_unref (msg);
    }

    /* Free resources */
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

    return 0;
}

int processImage()
{
    Mat image = imread( "test.jpeg", 1 );

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

int main(int argc, char** argv )
{

    int g = takeImage(argc, argv);
    if(g!=-1)
    {
        processImage();
    }
    else{
        printf("Taking image failed \n");
    }
}
