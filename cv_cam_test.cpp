#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <gst/gst.h>


int takeImage(int argc, argv )
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
    Mat image;
    image = imread( "test.jpeg", 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
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
