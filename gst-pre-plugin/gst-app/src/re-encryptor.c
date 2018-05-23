#include <gst/gst.h>

const char *params = "eJyNVDFuHDEM/Mpi6y1EnShSeUoM43AOjDTuLgkQGP67NUNqbXcu9iBRFDkzHN3r/nP/sb3u1+uvl9v9fr3O3f70/8/zfT+2Gf13e/n7zOiDyrGpH5vVY/O5tnZsUvqx9fmJzJ0pIjbT5kJnQJF6QfCSKTpPh0VUe65L1MAtFGaJUbBo0WbMtt3ikzJT2rzu2EieGE9QyKOwyCXusfe5EAFGObNHdHJd9YBxBoy5OBkZJQYgngGbEDqu9CghsrKUoiCF+Ounc8OilmgEzqjUPPqhDeQjGI/+0EZqjQsgqSsI1o0IwbR4/ozkH2xrgIB+nhSlkH1NnCxjGWlj1WRbScEAxlsMDJOHC5AatDxRW87aT3HrR2f6A1PTlI9DAHOvURx6qiUfdEKwpVVgiyVNEIYmATWL0oY4p8bUAwRQnpQpWmlpLZ5pyR1Hn0TDX1BbHt+m/39/921AEfX1FDzkASPz9EPQw7PpH+4nQLiMbpHL+aNxE9iQZ0tSTSNymF/GR2dhB+dofnx7taTZMQvLcbIFqozTuWkw9LK+3N9Xr7JecgkPUHJqCqrg76e+oElJa8AnhRZZFGMkkk+T0ISx/lnC51mEc6EoWYV+8HiE/Oa83t4B3ZrnlQ==";
const char *rk_ab_2018 = "eJw9kEEOwiAQRa9Cuu6CKR0GvIoxTTXduauaGOPd/Z+hLqCTB/Pm08+wLLf7uu/LMpzCcH0/tn0YA+hrvT+3Rs9zHYOWMVQsmaYxGEBWwAwQURRBIVPfcmURgY9zA61sgqEkF6ihNl6oDnlIOJsrDAajODYxqsIQbJGZGCRjguqfFFdaN2R8Vfo42Gb2R3+IMRa6C+WRY+vxGKYW9VE6eaDaw1DBlqY115bWGV3hGqaR0iM1WVOLJE9lqd+iU5Ov5jh+kcTUHyGX7w8vllFk";

int main(int argc, char *argv[])
{
    GstElement *pipeline, *source, *sink, *pre_re_enc;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    if(argc < 3){
        g_printerr("Specify input and output file.\n");
        return -1;
    }
    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    /* Create the elements */
    source = gst_element_factory_make("filesrc", "source");
    sink = gst_element_factory_make("filesink", "sink");
    pre_re_enc = gst_element_factory_make("pre", "pre_re_enc");

    /* Create the empty pipeline */
    pipeline = gst_pipeline_new("pipeline");

    if (!pipeline || !source || !sink || !pre_re_enc)
    {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    /* Build the pipeline */
    gst_bin_add_many(GST_BIN(pipeline), source, pre_re_enc, sink, NULL);
    if (gst_element_link_many(source, pre_re_enc, sink, NULL) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* Modify the source's properties */
    g_object_set(source, "location", argv[1], NULL);
    g_object_set(sink, "location", argv[2], NULL);
    g_object_set(pre_re_enc, "params", params, NULL);
    g_object_set(pre_re_enc, "mode", 1, NULL);
    g_object_set(pre_re_enc, "rk", rk_ab_2018, NULL);

    GST_DEBUG_BIN_TO_DOT_FILE(pipeline, GST_DEBUG_GRAPH_SHOW_ALL, "re-encryptor_pipeline");

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