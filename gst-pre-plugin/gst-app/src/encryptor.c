#include <gst/gst.h>

const char *params = "eJyNVDFuHDEM/Mpi6y1EnShSeUoM43AOjDTuLgkQGP67NUNqbXcu9iBRFDkzHN3r/nP/sb3u1+uvl9v9fr3O3f70/8/zfT+2Gf13e/n7zOiDyrGpH5vVY/O5tnZsUvqx9fmJzJ0pIjbT5kJnQJF6QfCSKTpPh0VUe65L1MAtFGaJUbBo0WbMtt3ikzJT2rzu2EieGE9QyKOwyCXusfe5EAFGObNHdHJd9YBxBoy5OBkZJQYgngGbEDqu9CghsrKUoiCF+Ounc8OilmgEzqjUPPqhDeQjGI/+0EZqjQsgqSsI1o0IwbR4/ozkH2xrgIB+nhSlkH1NnCxjGWlj1WRbScEAxlsMDJOHC5AatDxRW87aT3HrR2f6A1PTlI9DAHOvURx6qiUfdEKwpVVgiyVNEIYmATWL0oY4p8bUAwRQnpQpWmlpLZ5pyR1Hn0TDX1BbHt+m/39/921AEfX1FDzkASPz9EPQw7PpH+4nQLiMbpHL+aNxE9iQZ0tSTSNymF/GR2dhB+dofnx7taTZMQvLcbIFqozTuWkw9LK+3N9Xr7JecgkPUHJqCqrg76e+oElJa8AnhRZZFGMkkk+T0ISx/lnC51mEc6EoWYV+8HiE/Oa83t4B3ZrnlQ==";
const char *pk_a = "eJyNVMtOw0AM/JUo5xzWyb7MryBUFcQJDkgFJIT4dzxjO1w5tN04foxnZvu9vr3Ierd8r5fL0+v1drtc7Gl9/Hp/vq3bYtHP6+vHM6P3TbalzW0ZY1uk6LZMO/RmAfuIlG2pFpgTD90OljGsZHSkVws0T2/IYlCQemyLlqgrI4rVDq1Ha+aV3SI8MHQkhpbda3QOQBOjLavOGNJ2ZNok2THOImMPlHbuACSYEUjxoltt5SR720rM5cposUeJSHVsjU0MxFDfm794rj6Ro9lIcicRzX3EGiufbK7OiKCxb1myxLeQk5rE3oO5cW7ZfUuCYlug6CM7TodZ47flGBKQeAEeQvIhgKsLhgLsyKGh9dSAyREywxvUFgJVDchwC0SZ+RaSgXKgBpXkQkJ3ZkkPBFCFpMpJ3oyvU00SID6jazCKlWXfz4KeriuaX1CThLE55Ony8GO34e1l/+9dwZK4KyRmngKUvA7FDUtjhofUl4Y7QCXuBHQEIT3jceFYgT3TXygEJVgR5ocuOmI1zocgTsDhPSlV8fQ60run8Wb6dMRdSfeTVvQmqGQIr72DZhF0O5Jx2pb+KSlqn8H1n0kPv36ck38uejq3p6UywgJUgu2p+R9yuNwQ7ecXhYPrsw==";
const char *sk_a = "eJyVT7sOwjAM/JUoc4a45MmvIBQV1KndAkio6r9zlwR2Bjv2+c4577qukz6rXZdy3+ZaS0Gnb+/HUrVRQF/z9lwaenHJKI9I0aiYjQqoHd5k0Z/woheLwjMCQBApyhyIgAG2iz0yVMGDCDwAj36oRRw6hB+MINcDXuoqfzsdmxLXTjJciG3p+8O4gBqZbD+LA7r+gT3RWTuGYopyQwORzCr2w+n4+AAB3Us6";

int main(int argc, char *argv[])
{
    GstElement *pipeline, *source, *sink, *pre_enc;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    if(argc < 2){
        g_printerr("Specify output file.\n");
        return -1;
    }
    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    /* Create the elements */
    source = gst_element_factory_make("videotestsrc", "source");
    sink = gst_element_factory_make("filesink", "sink");
    pre_enc = gst_element_factory_make("pre", "pre_enc");

    /* Create the empty pipeline */
    pipeline = gst_pipeline_new("pipeline");

    if (!pipeline || !source || !sink || !pre_enc)
    {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    /* Build the pipeline */
    gst_bin_add_many(GST_BIN(pipeline), source, pre_enc, sink, NULL);
    if (gst_element_link_many(source, pre_enc, sink, NULL) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    /* Modify the source's properties */
    g_object_set(source, "pattern", 0, NULL);
    g_object_set(sink, "location", argv[1], NULL);
    g_object_set(pre_enc, "params", params, NULL);
    g_object_set(pre_enc, "mode", 0, NULL);
    g_object_set(pre_enc, "pk", pk_a, NULL);
    g_object_set(pre_enc, "sk", sk_a, NULL);

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