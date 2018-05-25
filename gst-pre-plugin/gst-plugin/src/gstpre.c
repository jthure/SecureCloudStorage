/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2018 jonas <<user@hostname.org>>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-pre
 *
 * FIXME:Describe pre here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! pre ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>
//#include <gstreamer-1.0/gst/base/gstbasetransform.h>
#include <gstreamer-1.0/gst/base/gstadapter.h>
#include <gst/base/gstbytewriter.h>
#include <gst/base/gstbytereader.h>

#include "gstpre.h"
#include "charm_embed_api.h"
#include <Python.h>

GST_DEBUG_CATEGORY_STATIC(gst_pre_debug);
#define GST_CAT_DEFAULT gst_pre_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT,
  PROP_PRE_MODE,
  PROP_PRE_PARAMS,
  PROP_PRE_PK,
  PROP_PRE_SK,
  PROP_PRE_RK
};
/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE("sink",
                                                                   GST_PAD_SINK,
                                                                   GST_PAD_ALWAYS,
                                                                   GST_STATIC_CAPS("ANY"));

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE("src",
                                                                  GST_PAD_SRC,
                                                                  GST_PAD_ALWAYS,
                                                                  GST_STATIC_CAPS("ANY"));

#define gst_pre_parent_class parent_class
G_DEFINE_TYPE(GstPre, gst_pre, GST_TYPE_ELEMENT);

static void gst_pre_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gst_pre_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

// static GstFlowReturn gst_pre_transform(GstBaseTransform *trans, GstBuffer *in_buf, GstBuffer *out_buf);
// static GstFlowReturn gst_pre_prepare_output_buffer(GstBaseTransform *trans, GstBuffer *in_buf, GstBuffer **out_buf);
static GstFlowReturn gst_pre_chain(GstPad *pad, GstObject *parent, GstBuffer *buf);
static gboolean gst_pre_sink_event(GstPad *pad, GstObject *parent, GstEvent *event);
/* GObject vmethod implementations */

/* initialize the pre's class */
static void gst_pre_class_init(GstPreClass *klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *)klass;
  gstelement_class = (GstElementClass *)klass;

  gobject_class->set_property = gst_pre_set_property;
  gobject_class->get_property = gst_pre_get_property;

  g_object_class_install_property(gobject_class, PROP_SILENT,
                                  g_param_spec_boolean("silent", "Silent", "Produce verbose output ?",
                                                       FALSE, G_PARAM_READWRITE));
  g_object_class_install_property(gobject_class, PROP_PRE_PARAMS,
                                  g_param_spec_string("params", "Params", "PRE Params",
                                                      NULL, G_PARAM_READWRITE));
  g_object_class_install_property(gobject_class, PROP_PRE_PK,
                                  g_param_spec_string("pk", "PK", "PRE Public key",
                                                      NULL, G_PARAM_READWRITE));
  g_object_class_install_property(gobject_class, PROP_PRE_SK,
                                  g_param_spec_string("sk", "SK", "PRE Secret key",
                                                      NULL, G_PARAM_READWRITE));
  g_object_class_install_property(gobject_class, PROP_PRE_RK,
                                  g_param_spec_string("rk", "RK", "PRE Re-encryption key",
                                                      NULL, G_PARAM_READWRITE));
  g_object_class_install_property(gobject_class, PROP_PRE_MODE,
                                  g_param_spec_int("mode", "Mode", "PRE Mode",
                                                   0, 2, 0, G_PARAM_READWRITE));
  gst_element_class_set_details_simple(gstelement_class,
                                       "Pre",
                                       "FIXME:Generic",
                                       "FIXME:Generic Template Element",
                                       "jonas <<user@hostname.org>>");

  gst_element_class_add_pad_template(gstelement_class, gst_static_pad_template_get(&src_factory));
  gst_element_class_add_pad_template(gstelement_class, gst_static_pad_template_get(&sink_factory));

  // GST_BASE_TRANSFORM_CLASS(klass)->transform = GST_DEBUG_FUNCPTR(gst_pre_transform);
  // GST_BASE_TRANSFORM_CLASS(klass)->prepare_output_buffer = GST_DEBUG_FUNCPTR(gst_pre_prepare_output_buffer);
}

static Charm_t *InitSignatureScheme(const char *class_file, const char *class_name)
{
  PyObject *py_class_name, *module = NULL, *func = NULL, *class_instance = NULL, *args = NULL;
  py_class_name = PyUnicode_FromString(class_file);
  module = PyImport_Import(py_class_name);
  Free(py_class_name);
  if (module != NULL)
  {
    debug("successful import: '%s'\n", module->ob_type->tp_name);
    func = PyObject_GetAttrString(module, class_name);
    debug("got attr string: '%s'\n", func->ob_type->tp_name);
    if (func && PyCallable_Check(func))
    {
      args = PyTuple_New(0);
      debug("calling class init.\n");
      // instantiate class_instance = ClassName()
      class_instance = PyObject_CallObject(func, args);
      debug("success: \n");
      Free(args);
    }
    else
    {
      // call failed
      if (PyErr_Occurred())
        PyErr_Print();
      fprintf(stderr, "Cannot find function.\n");
    }
    Free(func);
    Free(module);
    return (Charm_t *)class_instance;
  }
  else
  {
    if (PyErr_Occurred())
      PyErr_Print();
    fprintf(stderr, "Cannot complete import.\n");
  }
}
/* initialize the new element
 * initialize instance structure
 */
static void
gst_pre_init(GstPre *filter)
{
  GST_INFO_OBJECT(filter, "Initializing plugin");
  filter->sinkpad = gst_pad_new_from_static_template(&sink_factory, "sink");
  gst_pad_set_event_function(filter->sinkpad, GST_DEBUG_FUNCPTR(gst_pre_sink_event));
  gst_pad_set_chain_function(filter->sinkpad, GST_DEBUG_FUNCPTR(gst_pre_chain));
  GST_PAD_SET_PROXY_CAPS(filter->sinkpad);
  gst_element_add_pad(GST_ELEMENT(filter), filter->sinkpad);

  filter->srcpad = gst_pad_new_from_static_template(&src_factory, "src");
  GST_PAD_SET_PROXY_CAPS(filter->srcpad);
  gst_element_add_pad(GST_ELEMENT(filter), filter->srcpad);

  Charm_t *module = NULL, *group = NULL, *scheme = NULL, *sig = NULL, *hyb = NULL;

  InitializeCharm();

  group = InitPairingGroup(module, "SS512");
  if (group == NULL)
  {
    printf("could not import pairing group.\n");
    return;
  }
  // sig = InitSignatureScheme("charm.schemes.pksig.lamport_jm", "Lamport");
  scheme = InitScheme("charm.schemes.prenc.pre_afgh06_temp_jm", "AFGH06Temp", group);
  if (scheme == NULL)
  {
    printf("could not create scheme.\n");
    return;
  }
  hyb = InitAdapter("charm.adapters.pre_hybrid_jm", "HybridUniPREnc", scheme);
  if (hyb == NULL)
  {
    printf("could not create hybrid.\n");
    return;
  }

  // Charm_t *params = CallMethod(scheme, "setup", "");
  // Charm_t *keys_a = CallMethod(scheme, "keygen", "%O", params);
  // Charm_t *pk_a = GetIndex(keys_a, 0);
  // Charm_t *sk_a = GetIndex(keys_a, 1);

  // Charm_t *keys_b = CallMethod(scheme, "keygen", "%O", params);
  // Charm_t *pk_b = GetIndex(keys_b, 0);
  // Charm_t *sk_b = GetIndex(keys_b, 1);

  // Charm_t *rk_ab = CallMethod(scheme, "re_keygen", "%O%O%O%$s", params, sk_a, pk_b, "l", "2018");

  filter->scheme = hyb;
  filter->group = group;
  filter->params = NULL;
  filter->pk = NULL;
  filter->sk = NULL;
  filter->mode = PRE_ENCRYPT;
  filter->silent = FALSE;
  filter->adapter = gst_adapter_new();
  filter->bytes_in_object = -1;
  // g_print("PARAMS: %s\n", PyBytes_AsString(objectToBytes(params, group)));
  // g_print("PK_A: %s\n", PyBytes_AsString(objectToBytes(pk_a, group)));
  // g_print("SK_A: %s\n", PyBytes_AsString(objectToBytes(sk_a, group)));
  // g_print("PK_B: %s\n", PyBytes_AsString(objectToBytes(pk_b, group)));
  // g_print("SK_B: %s\n", PyBytes_AsString(objectToBytes(sk_b, group)));
  // g_print("RK_A->B: %s\n", PyBytes_AsString(objectToBytes(rk_ab, group)));
}

static void
gst_pre_set_property(GObject *object, guint prop_id,
                     const GValue *value, GParamSpec *pspec)
{
  GstPre *filter = GST_PRE(object);
  char *prop_string = NULL;

  switch (prop_id)
  {
  case PROP_SILENT:
    filter->silent = g_value_get_boolean(value);
    break;
  case PROP_PRE_MODE:
    filter->mode = g_value_get_int(value);
    break;
  case PROP_PRE_PARAMS:
    prop_string = g_value_get_string(value);
    filter->params = bytesToObject(PyBytes_FromString(prop_string), filter->group);
    break;
  case PROP_PRE_PK:
    prop_string = g_value_get_string(value);
    filter->pk = bytesToObject(PyBytes_FromString(prop_string), filter->group);
    break;
  case PROP_PRE_SK:
    prop_string = g_value_get_string(value);
    filter->sk = bytesToObject(PyBytes_FromString(prop_string), filter->group);
    break;
  case PROP_PRE_RK:
    prop_string = g_value_get_string(value);
    filter->rk = bytesToObject(PyBytes_FromString(prop_string), filter->group);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
gst_pre_get_property(GObject *object, guint prop_id,
                     GValue *value, GParamSpec *pspec)
{
  GstPre *filter = GST_PRE(object);

  switch (prop_id)
  {
  case PROP_SILENT:
    g_value_set_boolean(value, filter->silent);
    break;
  case PROP_PRE_MODE:
    g_value_set_int(value, filter->mode);
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

/* GstElement vmethod implementations */
/* this function handles sink events */
static gboolean
gst_pre_sink_event(GstPad *pad, GstObject *parent, GstEvent *event)
{
  GstPre *filter;
  gboolean ret;

  filter = GST_PRE(parent);

  GST_LOG_OBJECT(filter, "Received %s event: %" GST_PTR_FORMAT,
                 GST_EVENT_TYPE_NAME(event), event);

  switch (GST_EVENT_TYPE(event))
  {
  case GST_EVENT_CAPS:
  {
    GstCaps *caps;

    gst_event_parse_caps(event, &caps);
    /* do something with the caps */

    /* and forward */
    ret = gst_pad_event_default(pad, parent, event);
    break;
  }
  default:
    ret = gst_pad_event_default(pad, parent, event);
    break;
  }
  return ret;
}

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_pre_chain2(GstPad *pad, GstObject *parent, GstBuffer *buf)
{
  GstPre *filter;
  Charm_t *pre_op_value = NULL, *pre_op_value_bytes = NULL, *ct = NULL, *input_bytes = NULL;
  Py_ssize_t out_data_size;
  GstFlowReturn ret = GST_FLOW_OK;
  filter = GST_PRE(parent);
  gst_adapter_push(filter->adapter, buf);
  gsize extra_encrypted_size = 300000; // TODO: Calculate how much extra space is needed
  GstBuffer *out_buf = gst_buffer_new_allocate(NULL, gst_buffer_get_size(buf) + extra_encrypted_size, NULL);
  out_buf = gst_buffer_make_writable(out_buf);
  GstMapInfo out_map;
  gst_buffer_map(out_buf, &out_map, GST_MAP_WRITE);
  volatile written_bytes = 0;
  while (gst_adapter_available(filter->adapter) >= 50000 && ret == GST_FLOW_OK)
  {
    const gchar *data = gst_adapter_map(filter->adapter, 50000);
    input_bytes = PyBytes_FromStringAndSize(data, 50000);
    pre_op_value = CallMethod(filter->scheme, "encrypt_lvl2", "%O%O%O%$s", filter->params, filter->pk, input_bytes, "l", "2018"); //PRE encrypt lvl2
    pre_op_value_bytes = objectToBytes(pre_op_value, filter->group);                                                              //Serialize lvl 2 ciphertext to bytes
    char *pre_op_value_string;
    char sucessful = PyBytes_AsStringAndSize(pre_op_value_bytes, &pre_op_value_string, &out_data_size) != -1;
    for (int i = 0; i < out_data_size; ++i)
    {
      out_map.data[written_bytes] = pre_op_value_string[i];
      ++written_bytes;
    }
    gst_adapter_unmap(filter->adapter);
    gst_adapter_flush(filter->adapter, 50000);
    ret = GST_FLOW_OK;
  }
  gst_buffer_set_size(out_buf, written_bytes);
  // gst_buffer_unmap(in_buf, &in_map);
  gst_buffer_unmap(out_buf, &out_map);
  if (filter->silent == FALSE)
    g_print("I'm plugged, therefore I'm in.\n");

  /* just push out the incoming buffer without touching it */
  return gst_pad_push(filter->srcpad, out_buf);
}
static GstFlowReturn
gst_pre_chain(GstPad *pad, GstObject *parent, GstBuffer *buf)
{
  GstPre *filter;
  Charm_t *pre_op_value = NULL, *pre_op_value_bytes = NULL, *ct = NULL, *input_bytes = NULL;
  Py_ssize_t out_data_size;
  GstMapInfo in_map, out_map;
  filter = GST_PRE(parent);
  gsize extra_encrypted_size = 300000; // TODO: Calculate how much extra space is needed
  GstBuffer *out_buf = gst_buffer_new_allocate(NULL, gst_buffer_get_size(buf) + extra_encrypted_size, NULL);
  out_buf = gst_buffer_make_writable(out_buf);

  if (filter->mode == PRE_ENCRYPT)
  {
    gst_buffer_map(buf, &in_map, GST_MAP_READ);
    gst_buffer_map(out_buf, &out_map, GST_MAP_WRITE);
    input_bytes = PyBytes_FromStringAndSize(in_map.data, in_map.size);
    pre_op_value = CallMethod(filter->scheme, "encrypt_lvl2", "%O%O%O%$s", filter->params, filter->pk, input_bytes, "l", "2018"); //PRE encrypt lvl2
    pre_op_value_bytes = objectToBytes(pre_op_value, filter->group);                                                              //Serialize lvl 2 ciphertext to bytes
    char *pre_op_value_string;
    char sucessful = PyBytes_AsStringAndSize(pre_op_value_bytes, &pre_op_value_string, &out_data_size) != -1;
    // volatile gint64 x = (gint64) out_data_size;
    // g_print("Sizes: %d, %d\n", sizeof(Py_ssize_t), sizeof(gint64));
    // g_print("Num: %d, %d\n", out_data_size, x);
    GstByteWriter *byte_writer = gst_byte_writer_new();
    if (!gst_byte_writer_put_int64_be(byte_writer, out_data_size))
    {
      g_printerr("Failed to write size bytes.\n");
    }
    guint8 *size_bytes = gst_byte_writer_free_and_get_data(byte_writer);
    g_print("size_bytes: %s", size_bytes);
    for (int i = 0; i < 8; ++i)
    {
      out_map.data[i] = size_bytes[i];
    }
    for (int i = 0; i < out_data_size; ++i)
    {
      out_map.data[i + 8] = pre_op_value_string[i];
    }
    gst_buffer_set_size(out_buf, out_data_size + 8);
    gst_buffer_unmap(out_buf, &out_map);
    gst_buffer_unmap(buf, &in_map);
    return gst_pad_push(filter->srcpad, out_buf);
  }
  else if (filter->mode == PRE_RE_ENCRYPT)
  {
    gst_adapter_push(filter->adapter, buf);
    if (filter->bytes_in_object == -1)
    {
      // gst_buffer_map(buf, &in_map, GST_MAP_READ);
      guint8 *size_of_object_bytes = gst_adapter_take(filter->adapter, 8);
      GstByteReader *byte_reader = gst_byte_reader_new(size_of_object_bytes, 8);
      gint64 size_of_object;
      gst_byte_reader_get_int64_be(byte_reader, &size_of_object);
      g_print("Size of object: %d\n", size_of_object);
      filter->bytes_in_object = size_of_object;
      // gst_buffer_unmap(buf, &in_map);
      // gst_adapter_push(filter->adapter, buf);
      // gst_adapter_flush(filter->adapter, 8);
    }
    gsize bytes_available = gst_adapter_available(filter->adapter);
    if(bytes_available >= filter->bytes_in_object) {
      gchar *data = gst_adapter_take(filter->adapter, filter->bytes_in_object);
      input_bytes = PyBytes_FromStringAndSize(data, filter->bytes_in_object);
      ct = bytesToObject(input_bytes, filter->group); //De-serialize bytes into lvl 2 ciphertext
      pre_op_value = CallMethod(filter->scheme, "re_encrypt", "%O%O%O", filter->params, filter->rk, ct); //PRE re-encrypt
      pre_op_value_bytes = objectToBytes(pre_op_value, filter->group); //Serialize lvl 1 ciphertext to bytes
      // Extract string from the python bytes object (result from the PRE operation)
      char *pre_op_value_string;
      char sucessful = PyBytes_AsStringAndSize(pre_op_value_bytes, &pre_op_value_string, &out_data_size) != -1;

      gst_buffer_map(out_buf, &out_map, GST_MAP_WRITE);
      GstByteWriter *byte_writer = gst_byte_writer_new();
      if (!gst_byte_writer_put_int64_be(byte_writer, out_data_size))
      {
        g_printerr("Failed to write size bytes.\n");
      }
      guint8 *size_bytes = gst_byte_writer_free_and_get_data(byte_writer);
      g_print("size_bytes: %s", size_bytes);
      for (int i = 0; i < 8; ++i)
      {
        out_map.data[i] = size_bytes[i];
      }
      // Copy the bytes into the output buffer
      for(int i = 0; i < out_data_size; ++i){
        out_map.data[i+8] = pre_op_value_string[i];
      }
      gst_buffer_set_size(out_buf, out_data_size + 8);
      gst_buffer_unmap(out_buf, &out_map);
      filter->bytes_in_object = -1;
      return gst_pad_push(filter->srcpad, out_buf);
    }
  }else if(filter->mode == PRE_DECRYPT) {
      gst_adapter_push(filter->adapter, buf);
      if (filter->bytes_in_object == -1)
      {
        gst_buffer_map(buf, &in_map, GST_MAP_READ);
        guint8 *size_of_object_bytes = gst_adapter_take(filter->adapter, 8);
        GstByteReader *byte_reader = gst_byte_reader_new(size_of_object_bytes, 8);
        gint64 size_of_object;
        gst_byte_reader_get_int64_be(byte_reader, &size_of_object);
        g_print("Size of object: %d\n", size_of_object);
        filter->bytes_in_object = size_of_object;
        // gst_buffer_unmap(buf, &in_map);
        // gst_adapter_push(filter->adapter, buf);
        // gst_adapter_flush(filter->adapter, 8);
      }
      gsize bytes_available = gst_adapter_available(filter->adapter);
      if(bytes_available >= filter->bytes_in_object) {
        gchar *data = gst_adapter_take(filter->adapter, filter->bytes_in_object);
        input_bytes = PyBytes_FromStringAndSize(data, filter->bytes_in_object);
        ct = bytesToObject(input_bytes, filter->group); //De-serialize bytes into lvl 2 ciphertext
        pre_op_value = CallMethod(filter->scheme, "decrypt_lvl1", "%O%O%O", filter->params, filter->sk, ct); //PRE decrypt
        char *pre_op_value_string;
        char sucessful = PyBytes_AsStringAndSize(pre_op_value, &pre_op_value_string, &out_data_size) != -1;
        gst_buffer_map(out_buf, &out_map, GST_MAP_WRITE);

        // Copy the bytes into the output buffer
        for(int i = 0; i < out_data_size; ++i){
          out_map.data[i] = pre_op_value_string[i];
        }
        gst_buffer_set_size(out_buf, out_data_size);
        gst_buffer_unmap(out_buf, &out_map);
        filter->bytes_in_object = -1;
        return gst_pad_push(filter->srcpad, out_buf);
      }
    }

  // GstFlowReturn ret = GST_FLOW_OK;
  // gst_adapter_push(filter->adapter, buf);
  // GstMapInfo out_map;
  // gst_buffer_map(out_buf, &out_map, GST_MAP_WRITE);
  // volatile written_bytes = 0;
  // while(gst_adapter_available(filter->adapter) >= 50000 && ret == GST_FLOW_OK){
  //   const gchar *data = gst_adapter_map(filter->adapter, 50000);
  //   input_bytes = PyBytes_FromStringAndSize(data, 50000);
  //   pre_op_value = CallMethod(filter->scheme, "encrypt_lvl2", "%O%O%O%$s", filter->params, filter->pk, input_bytes, "l", "2018"); //PRE encrypt lvl2
  //   pre_op_value_bytes = objectToBytes(pre_op_value, filter->group); //Serialize lvl 2 ciphertext to bytes
  //   char *pre_op_value_string;
  //   char sucessful = PyBytes_AsStringAndSize(pre_op_value_bytes, &pre_op_value_string, &out_data_size) != -1;
  //   for(int i = 0;i < out_data_size; ++i){
  //     out_map.data[written_bytes] = pre_op_value_string[i];
  //     ++written_bytes;
  //   }
  //   gst_adapter_unmap(filter->adapter);
  //   gst_adapter_flush(filter->adapter, 50000);
  //   ret = GST_FLOW_OK;
  // }
  // gst_buffer_set_size(out_buf, written_bytes);
  // // gst_buffer_unmap(in_buf, &in_map);
  // gst_buffer_unmap(out_buf, &out_map);
  if (filter->silent == FALSE)
    g_print("I'm plugged, therefore I'm in.\n");

  /* just push out the incoming buffer without touching it */
  return GST_FLOW_OK;
}
// static GstFlowReturn gst_pre_prepare_output_buffer(GstBaseTransform *trans, GstBuffer *in_buf, GstBuffer **out_buf)
// {
//   GstPre *filter = GST_PRE(trans);
//   gsize extra_encrypted_size = 300000; // TODO: Calculate how much extra space is needed
//   *out_buf = gst_buffer_new_allocate(NULL, gst_buffer_get_size(in_buf) + extra_encrypted_size , NULL);
//   *out_buf = gst_buffer_make_writable(*out_buf);
//   return GST_FLOW_OK;
// }
// static GstFlowReturn
// gst_pre_transform(GstBaseTransform *trans, GstBuffer *in_buf, GstBuffer *out_buf)
// {
//   Charm_t *pre_op_value = NULL, *pre_op_value_bytes = NULL, *ct = NULL, *input_bytes = NULL;
//   Py_ssize_t out_data_size;
//   GstFlowReturn ret = GST_FLOW_OK;
//   GstPre *filter = GST_PRE(trans);
//   gst_adapter_push(filter->adapter, in_buf);

//   GstMapInfo in_map, out_map;
//   // gst_buffer_map(in_buf, &in_map, GST_MAP_READ);
//   gst_buffer_map(out_buf, &out_map, GST_MAP_WRITE);

//   // Create python bytes from the input
//   // input_bytes = PyBytes_FromStringAndSize(in_map.data, in_map.size);
//   volatile int written_bytes = 0;
//   if (filter->mode == PRE_ENCRYPT)
//   {
//     while(gst_adapter_available(filter->adapter) >= 50000 && ret == GST_FLOW_OK){
//       const gchar *data = gst_adapter_map(filter->adapter, 50000);
//       input_bytes = PyBytes_FromStringAndSize(data, 50000);
//       pre_op_value = CallMethod(filter->scheme, "encrypt_lvl2", "%O%O%O%$s", filter->params, filter->pk, input_bytes, "l", "2018"); //PRE encrypt lvl2
//       pre_op_value_bytes = objectToBytes(pre_op_value, filter->group); //Serialize lvl 2 ciphertext to bytes
//       char *pre_op_value_string;
//       char sucessful = PyBytes_AsStringAndSize(pre_op_value_bytes, &pre_op_value_string, &out_data_size) != -1;
//       for(int i = 0;i < out_data_size; ++i){
//         out_map.data[written_bytes] = pre_op_value_string[i];
//         ++written_bytes;
//       }
//       gst_adapter_unmap(filter->adapter);
//       gst_adapter_flush(filter->adapter, 50000);
//       ret = GST_FLOW_OK;
//     }
//   }
//   else if (filter->mode == PRE_DECRYPT)
//   {
//     ct = bytesToObject(input_bytes, filter->group); //De-serialize bytes into lvl 1 ciphertext
//     pre_op_value_bytes = CallMethod(filter->scheme, "decrypt_lvl1", "%O%O%O", filter->params, filter->sk, ct); //PRE decrypt lvl1
//   }
//   else if(filter->mode == PRE_RE_ENCRYPT)
//   {
//     ct = bytesToObject(input_bytes, filter->group); //De-serialize bytes into lvl 2 ciphertext
//     pre_op_value = CallMethod(filter->scheme, "re_encrypt", "%O%O%O", filter->params, filter->rk, ct); //PRE re-encrypt
//     pre_op_value_bytes = objectToBytes(pre_op_value, filter->group); //Serialize lvl 1 ciphertext to bytes
//   }
//   else{
//     g_printerr("Invalid PRE mode.\n");
//     return GST_FLOW_ERROR;
//   }
//   // Extract string from the python bytes object (result from the PRE operation)
//   // char *pre_op_value_string;
//   // char sucessful = PyBytes_AsStringAndSize(pre_op_value_bytes, &pre_op_value_string, &out_data_size) != -1;

//   // Copy the bytes into the output buffer
//   // for(int i = 0; i < out_data_size; ++i){
//   //   out_map.data[i] = pre_op_value_string[i];
//   // }
//   gst_buffer_set_size(out_buf, written_bytes);
//   // gst_buffer_unmap(in_buf, &in_map);
//   gst_buffer_unmap(out_buf, &out_map);
//   return GST_FLOW_OK;
// }

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
pre_init(GstPlugin *pre)
{
  /* debug category for fltering log messages
   *
   * exchange the string 'Template pre' with your description
   */
  GST_DEBUG_CATEGORY_INIT(gst_pre_debug, "pre",
                          0, "Proxy re-encryption");

  return gst_element_register(pre, "pre", GST_RANK_NONE,
                              GST_TYPE_PRE);
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "axis.pre"
#endif

/* gstreamer looks for this structure to register myfilters
 *
 * exchange the string 'Template pre' with your pre description
 */
GST_PLUGIN_DEFINE(
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    pre,
    "Proxy re-encryption",
    pre_init,
    VERSION,
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/")
