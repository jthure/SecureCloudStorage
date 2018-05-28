#include <gst/gst.h>

const char *params = "eJyNVDFuHDEM/Mpi6y1EnShSeUoM43AOjDTuLgkQGP67NUNqbXcu9iBRFDkzHN3r/nP/sb3u1+uvl9v9fr3O3f70/8/zfT+2Gf13e/n7zOiDyrGpH5vVY/O5tnZsUvqx9fmJzJ0pIjbT5kJnQJF6QfCSKTpPh0VUe65L1MAtFGaJUbBo0WbMtt3ikzJT2rzu2EieGE9QyKOwyCXusfe5EAFGObNHdHJd9YBxBoy5OBkZJQYgngGbEDqu9CghsrKUoiCF+Ounc8OilmgEzqjUPPqhDeQjGI/+0EZqjQsgqSsI1o0IwbR4/ozkH2xrgIB+nhSlkH1NnCxjGWlj1WRbScEAxlsMDJOHC5AatDxRW87aT3HrR2f6A1PTlI9DAHOvURx6qiUfdEKwpVVgiyVNEIYmATWL0oY4p8bUAwRQnpQpWmlpLZ5pyR1Hn0TDX1BbHt+m/39/921AEfX1FDzkASPz9EPQw7PpH+4nQLiMbpHL+aNxE9iQZ0tSTSNymF/GR2dhB+dofnx7taTZMQvLcbIFqozTuWkw9LK+3N9Xr7JecgkPUHJqCqrg76e+oElJa8AnhRZZFGMkkk+T0ISx/lnC51mEc6EoWYV+8HiE/Oa83t4B3ZrnlQ==";
const char *pk_b = "eJyNU8tu3DAM/BXDZx9EWRKl/kpRLNIip+QQYNsCRZB/D2dIOnvsYb2S+J4Zvu9vL7J/29732+3X69P9frvZbf/57/fzfT82e/379Prnma/fuxxbn8cmpR3bGHaoEjcpxWznsam4aZpf78e2lhkFQQIrvKr9I7jw1fyb+SiS1Bqfab8xL58zInCQajmmeatFLTurVRn2m2q/0+8i/NRw7wU3tmlphuIAb0SJ50bFWdzYh2dUiQgeWLzYp0UhEcQN74Z1fNALDynVWwQQnAtZpYhnRB6URndfpU7HAtMEbkBSHbc1cywMOFfc0E0OwxYdsjR7MxITwRGIgyGOwkqJEJGPkRPNGQ0rnZYTTWrAEV69rxK0a2DTJV6/8HOGJVBwqCiB+ZB65oDeUUJ1RsAif/1SR/Ldw5mSo4p6wlGCUfRJvaij7DrM7tp8gIlTgECgPOTHh23D20v9310Bwr4ryWwfiZHGrC7EFogALTJalsuKK8ERywrFkk591Au51dQJq1EFcm0PIWfOfkmqhhHS8w09g0QkgFybBnbUVkvZnoGdxs61GIVLhw4B24jmgSdRVk+OBjTsrlu55NxCXphpBUhLk4aYEYEtF4PqKkGqA9euNU+lokmQ9/EJA0zs1A==";
const char *sk_b = "eJydT0kOwyAM/ArizAGzOv1KFaG0yim50Vaqovy9YyAf6AEv47GZOXTdnL6pQ5fy3JdaS0GnH9/XWrVRQD/L/l4beg9sVMTLHjkYRTZdBVAia9SUgfie8ySTKMEBRcFeaChSG/nOSbhCjiTY3gkvYMIXLckOzScU1Y3+0pu6BLmUaSggGiFCLrvhiK0UU+eSzUOLeHDdXZuy7MA887DZ/IoF+U/Unj9lF0ug";

int main(int argc, char *argv[])
{
    GstElement *pipeline, *source, *sink, *pre_dec, *videoparser;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    if(argc < 2){
        g_printerr("Specify input file.\n");
        return -1;
    }
    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    /* Create the elements */
    source = gst_element_factory_make("filesrc", "source");
    sink = gst_element_factory_make("autovideosink", "sink");
    videoparser = gst_element_factory_make("rawvideoparse", "videoparser");
    pre_dec = gst_element_factory_make("pre", "pre_dec");

    /* Create the empty pipeline */
    pipeline = gst_pipeline_new("pipeline");

    if (!pipeline || !source || !sink || !pre_dec || !videoparser)
    {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    /* Build the pipeline */
    gst_bin_add_many(GST_BIN(pipeline), source, pre_dec, videoparser, sink, NULL);
    if (gst_element_link_many(source, pre_dec, videoparser, sink, NULL) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* Modify the source's properties */
    g_object_set(source, "location", argv[1], NULL);
    g_object_set(pre_dec, "params", params, NULL);
    g_object_set(pre_dec, "mode", 2, NULL);
    g_object_set(pre_dec, "pk", pk_b, NULL);
    g_object_set(pre_dec, "sk", sk_b, NULL);

    GST_DEBUG_BIN_TO_DOT_FILE(pipeline, GST_DEBUG_GRAPH_SHOW_ALL, "deccryptor_pipeline");

    /* Start playing */
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* Wait until error or EOS */
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* Parse message */
    if (msg != NULL)
    {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE(msg))
        {
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
            g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
            g_clear_error(&err);
            g_free(debug_info);
            break;
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");
            break;
        default:
            /* We should not reach here because we only asked for ERRORs and EOS */
            g_printerr("Unexpected message received.\n");
            break;
        }
        gst_message_unref(msg);
    }

    /* Free resources */
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    return 0;
}