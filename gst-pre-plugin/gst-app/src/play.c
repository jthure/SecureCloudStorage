/* Copyright (C) 2006 Tim-Philipp Müller <tim centricular net>
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

#include "play.h"
#include "gst/base/gstbasetransform.h"

const char *params = "eJyNVDFuHDEM/Mpi6y1EnShSeUoM43AOjDTuLgkQGP67NUNqbXcu9iBRFDkzHN3r/nP/sb3u1+uvl9v9fr3O3f70/8/zfT+2Gf13e/n7zOiDyrGpH5vVY/O5tnZsUvqx9fmJzJ0pIjbT5kJnQJF6QfCSKTpPh0VUe65L1MAtFGaJUbBo0WbMtt3ikzJT2rzu2EieGE9QyKOwyCXusfe5EAFGObNHdHJd9YBxBoy5OBkZJQYgngGbEDqu9CghsrKUoiCF+Ounc8OilmgEzqjUPPqhDeQjGI/+0EZqjQsgqSsI1o0IwbR4/ozkH2xrgIB+nhSlkH1NnCxjGWlj1WRbScEAxlsMDJOHC5AatDxRW87aT3HrR2f6A1PTlI9DAHOvURx6qiUfdEKwpVVgiyVNEIYmATWL0oY4p8bUAwRQnpQpWmlpLZ5pyR1Hn0TDX1BbHt+m/39/921AEfX1FDzkASPz9EPQw7PpH+4nQLiMbpHL+aNxE9iQZ0tSTSNymF/GR2dhB+dofnx7taTZMQvLcbIFqozTuWkw9LK+3N9Xr7JecgkPUHJqCqrg76e+oElJa8AnhRZZFGMkkk+T0ISx/lnC51mEc6EoWYV+8HiE/Oa83t4B3ZrnlQ==";
const char *pk_a = "eJyNVMtOw0AM/JUo5xzWyb7MryBUFcQJDkgFJIT4dzxjO1w5tN04foxnZvu9vr3Ierd8r5fL0+v1drtc7Gl9/Hp/vq3bYtHP6+vHM6P3TbalzW0ZY1uk6LZMO/RmAfuIlG2pFpgTD90OljGsZHSkVws0T2/IYlCQemyLlqgrI4rVDq1Ha+aV3SI8MHQkhpbda3QOQBOjLavOGNJ2ZNok2THOImMPlHbuACSYEUjxoltt5SR720rM5cposUeJSHVsjU0MxFDfm794rj6Ro9lIcicRzX3EGiufbK7OiKCxb1myxLeQk5rE3oO5cW7ZfUuCYlug6CM7TodZ47flGBKQeAEeQvIhgKsLhgLsyKGh9dSAyREywxvUFgJVDchwC0SZ+RaSgXKgBpXkQkJ3ZkkPBFCFpMpJ3oyvU00SID6jazCKlWXfz4KeriuaX1CThLE55Ony8GO34e1l/+9dwZK4KyRmngKUvA7FDUtjhofUl4Y7QCXuBHQEIT3jceFYgT3TXygEJVgR5ocuOmI1zocgTsDhPSlV8fQ60run8Wb6dMRdSfeTVvQmqGQIr72DZhF0O5Jx2pb+KSlqn8H1n0kPv36ck38uejq3p6UywgJUgu2p+R9yuNwQ7ecXhYPrsw==";
const char *sk_a = "eJyVT7sOwjAM/JUoc4a45MmvIBQV1KndAkio6r9zlwR2Bjv2+c4577qukz6rXZdy3+ZaS0Gnb+/HUrVRQF/z9lwaenHJKI9I0aiYjQqoHd5k0Z/woheLwjMCQBApyhyIgAG2iz0yVMGDCDwAj36oRRw6hB+MINcDXuoqfzsdmxLXTjJciG3p+8O4gBqZbD+LA7r+gT3RWTuGYopyQwORzCr2w+n4+AAB3Us6";
const char *pk_b = "eJyNU8tu3DAM/BXDZx9EWRKl/kpRLNIip+QQYNsCRZB/D2dIOnvsYb2S+J4Zvu9vL7J/29732+3X69P9frvZbf/57/fzfT82e/379Prnma/fuxxbn8cmpR3bGHaoEjcpxWznsam4aZpf78e2lhkFQQIrvKr9I7jw1fyb+SiS1Bqfab8xL58zInCQajmmeatFLTurVRn2m2q/0+8i/NRw7wU3tmlphuIAb0SJ50bFWdzYh2dUiQgeWLzYp0UhEcQN74Z1fNALDynVWwQQnAtZpYhnRB6URndfpU7HAtMEbkBSHbc1cywMOFfc0E0OwxYdsjR7MxITwRGIgyGOwkqJEJGPkRPNGQ0rnZYTTWrAEV69rxK0a2DTJV6/8HOGJVBwqCiB+ZB65oDeUUJ1RsAif/1SR/Ldw5mSo4p6wlGCUfRJvaij7DrM7tp8gIlTgECgPOTHh23D20v9310Bwr4ryWwfiZHGrC7EFogALTJalsuKK8ERywrFkk591Au51dQJq1EFcm0PIWfOfkmqhhHS8w09g0QkgFybBnbUVkvZnoGdxs61GIVLhw4B24jmgSdRVk+OBjTsrlu55NxCXphpBUhLk4aYEYEtF4PqKkGqA9euNU+lokmQ9/EJA0zs1A==";
const char *sk_b = "eJydT0kOwyAM/ArizAGzOv1KFaG0yim50Vaqovy9YyAf6AEv47GZOXTdnL6pQ5fy3JdaS0GnH9/XWrVRQD/L/l4beg9sVMTLHjkYRTZdBVAia9SUgfie8ySTKMEBRcFeaChSG/nOSbhCjiTY3gkvYMIXLckOzScU1Y3+0pu6BLmUaSggGiFCLrvhiK0UU+eSzUOLeHDdXZuy7MA887DZ/IoF+U/Unj9lF0ug";
const char *pk_c = "eJyNU7tOBDEM/JXV1lvEm4cdfgWh04GooEA6QEKIf8fjR0RJcbls4tgz4/H3/vZC+932vV8uT6/X2+1y0a/98ev9+bYfm55+Xl8/nu30vtOxdTk2ohPL1KXYQmupx8Z6xBrA7dhG1/+h/zNeSdEUetE0jVgqPRA9mPrRNRo1iMjTDNZI/TGCNE3X/Yy9lUY6Zg/Ea0E9ZDh1EY6i0zYNb2wBhhrZxDPiMUByEqj+3UckIwLa8wx0VPSiBUQgQFVewcDGYBN0kdJuUJdKRxb2a7tycGfoMoNlX2XJ1UIxKuICWDU8BHYjhUfekVTVseILZCDlEN+bhGQVhgMRWgQ5Elr1mW0CXesZWg2BJOAjL4hxi8glBACjDVJdD1DrgRTyS/jDXp5liVu9wFKZI06SKov3RiwqpLSNA2jOdJFDNteeXBlkmjNYwyRcogUyA8Gghx8dgbeX878DAqgYELN/Ohye4x6NM3NSDIBB9e7VZND8Zchfl2fpzxCZnwHVCLcasuW0jPC0CV79bEb/OBpiyto895SsBLKRXh0jRx0QekyNJLfMAiYI5dTO5jxHZiYZjl8/PbRxurukWTEX8JO5ogc9bMz3MCs8g8fh0xqQjVUohK79/AKygem0";
const char *sk_c = "eJyVUEkOwjAM/IqVcw522sYpX0EoKqin9hZAQlX/jhfyAA6OPDNeJj5C21K4wBFqfexLa7UKCvfPc20hgrDvZX+txl7HEmGSmDEC4RAhTxGMVIJIAHsYIJwdTSlCyV2RHtYe6WchilRyVmGUgSJkAUUG8vgjiWROptspftpG/7rl5MsIbVbylaoSqimTzNnQFyos7FY0rNASwv6g/mN2734QK6Se6QXU9PkF1+VMWQ==";
const char *pk_d = "eJyNVEFu3DAM/Irhsw+iLJpSvhIUizTIKTkE2DZAEOTvJTkjoccc7LUoaTgcDvdrf3+V/WH72m+357en+/1289X++/PPy30/No9+PL39fcnoo8qxaT+2Yf5bj+1Sf3yt17GZPyLt2Hp++EnzhRTDR6+xKHzFpW5xrmMhpXFrDH8Ksdrp5xaU71jFRY2A+G5QkqLxOkEoyCRcM5yX6nDqjxkXF/l3P94CtZNp6wj2Qm7k5S8bQGTkZOV50LeumVUkwGPbo70zoxRWnHzlQlmZU6ozscKwndTBlGVXAeG40EkeJ2Tei/oMbYkaQzVV1qwQKxEa8HO/AiCrtMk7coQGwR+8ZbVs9dggRB+zmnmbX3XKQv2z/e2/asZq4LIBj0UjgnHgj1niQGLkUiS+dHZDYBnQrygswNNDaRml2JmtsnlKlo1tTXUba0hvy69v9//7a/3pdARUTscA8SSc3lPQCVpdp5/qUtf4ZVPNsM1yLrYutDYn8IKEOWiJX2drpo9hN0EjVamElItTg4mRMdEV0g8OiU7JqlDivga7LbAT9kuvSKV/85WzVXEj7BTY7Vy5TyKnSI0TkOTgm5Mh5XhyrgRAEc1JlqlJ/Gv0OYcDrfv+B7kz6cI=";
const char *sk_d = "eJyVT0EOAiEM/ArhzIEiUPArxpDV7Gn3hpqYzf7dKTXu2UMLnU7bmc32Jdiz2Wxr93XqvTVU9vZ+zN06A/Q1rc95oJdYnEmIzHgTXkQ6OcPeGfKEREESS8rAozOFvz3OWnDVYQpjCgNMipLHrgRaCkqX/wAZc5muOwT1hf6VW3GoylXSTRSwv6CofNwT0miqBXST6COxWdVq+ckRflHfjEYUW+XYJ1L3DzVuSuM=";
const char *rk_ab_2018 = "eJw9kEEOwiAQRa9Cuu6CKR0GvIoxTTXduauaGOPd/Z+hLqCTB/Pm08+wLLf7uu/LMpzCcH0/tn0YA+hrvT+3Rs9zHYOWMVQsmaYxGEBWwAwQURRBIVPfcmURgY9zA61sgqEkF6ihNl6oDnlIOJsrDAajODYxqsIQbJGZGCRjguqfFFdaN2R8Vfo42Gb2R3+IMRa6C+WRY+vxGKYW9VE6eaDaw1DBlqY115bWGV3hGqaR0iM1WVOLJE9lqd+iU5Ov5jh+kcTUHyGX7w8vllFk";
const char *rk_ab_2017 = "eJxFUMtuwzAM+xUj5xysxLbk/sowBN3QW2/ZBhRF/32kJGwHJXpQJOXnchyf9+t5HsdyKcvH4+t2LmtB9+d6/755963NtXRbiw0E/qOvRSt6CG0I1J0z4KTuKJQJO3smIp0Z+rMmSDkTrI+RICYKSN8QjVNiRUJJKigm5CcgJpzsoR9ygBgW1U0gkU3CklNx2TS8+qLzSsWH51kLaZEtfdKEuewM/aEJ8Xtqi44743HDdY2VBef/uTL/mCUZ6Vu2mgfnI+qIC2mUDCr5VnzxIe+vX3yHUqE=";
const char *rk_ac_2018 = "eJw9kEEOwiAQRa9Cuu6CKR0GvIoxTTXduauaGOPd/Z+hLqCTB/Pm08+wLLf7uu/LMpzCcH0/tn0YA+hrvT+3Rs9zHYOWMVQsmaYxGEBWwAwQURRBIVPfcmURgY9zA61sgqEkF6ihNl6oDnlIOJsrDAajODYxqsIQbJGZGCRjguqfFFdaN2R8Vfo42Gb2R3+IMRa6C+WRY+vxGKYW9VE6eaDaw1DBlqY115bWGV3hGqaR0iM1WVOLJE9lqd+iU5Ov5jh+kcTUHyGX7w8vllFk";

void
play_uri (const gchar * uri)
{
  GstStateChangeReturn sret;
  GstElement *playbin, *audiosink, *videosink, *videoflip, *myfilter, *filter_bin, *source, *sink, *pipeline, *pre_enc, *pre_re_enc, *pre_dec;
  GstPad *pad, *ghost_pad;
  GstMessage *msg = NULL;
  GstBus *bus;

  g_print ("Trying to play %s ...\n", uri);

  // source = gst_element_factory_make("urisourcebin", "source");
  source = gst_element_factory_make("videotestsrc", "source");
  sink = gst_element_factory_make("filesink", "sink");
  videosink = gst_element_factory_make ("autovideosink", "videosink");
  pre_enc = gst_element_factory_make("myfilter", "pre_enc");
  pre_re_enc = gst_element_factory_make("myfilter", "pre_re_enc");
  pre_dec = gst_element_factory_make("myfilter", "pre_dec");
  

  pipeline = gst_pipeline_new("pipeline");

  if(!pipeline || !source || !videosink || !pre_enc || !pre_re_enc || !pre_dec){
    g_printerr("Not all elements could be created.\n");
    return;
  }
  gst_bin_add_many(GST_BIN(pipeline), source, pre_enc, pre_re_enc, pre_dec, videosink, NULL);
  if(gst_element_link_many(source, pre_enc, pre_re_enc, pre_dec, videosink, NULL) != TRUE){
    g_printerr("Elements could not be linked.\n");
  }



  // playbin = gst_element_factory_make ("playbin", "playbin");
  // if (playbin == NULL) goto no_playbin;

  // /* get playbin's bus - we'll watch it for messages */
  // bus = gst_pipeline_get_bus (GST_PIPELINE (playbin));
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  g_object_set (source, "pattern", 0, NULL);

  g_object_set (pre_enc, "params", params, NULL);
  g_object_set (pre_enc, "mode", 0, NULL);
  g_object_set (pre_enc, "pk", pk_a, NULL);
  g_object_set (pre_enc, "sk", sk_a, NULL);

  g_object_set (pre_re_enc, "params", params, NULL);
  g_object_set (pre_re_enc, "mode", 1, NULL);
  g_object_set (pre_re_enc, "rk", rk_ab_2018, NULL);

  g_object_set (pre_dec, "params", params, NULL);
  g_object_set (pre_dec, "mode", 2, NULL);
  g_object_set (pre_dec, "pk", pk_b, NULL);
  g_object_set (pre_dec, "sk", sk_b, NULL);

  // /* set audio sink */
  // audiosink = gst_element_factory_make ("autoaudiosink", "audiosink");
  // if (audiosink == NULL) goto no_autoaudiosink;
  // g_object_set (playbin, "audio-sink", audiosink, NULL);

  // /* set video sink */
  // videosink = gst_element_factory_make ("autovideosink", "videosink");
  // if (videosink == NULL) goto no_autovideosink;
  // g_object_set (playbin, "video-sink", videosink, NULL);

  
  // myfilter = gst_element_factory_make("myfilter", "myfilter");
  // if(myfilter == NULL) goto no_myfilter;



  // g_object_set(videoflip, "video-direction", 2, NULL);
  // g_object_set(playbin, "video-filter", myfilter, NULL);

  /* set URI to play back */
  // g_object_set (playbin, "uri", uri, NULL);
  // g_object_set (source, "uri", uri, NULL);

  GST_DEBUG_BIN_TO_DOT_FILE(pipeline, GST_DEBUG_GRAPH_SHOW_ALL, "pipeline");

  /* and GO GO GO! */
  // gst_element_set_state (GST_ELEMENT (playbin), GST_STATE_PLAYING);
  gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);

  /* wait (blocks!) until state change either completes or fails */
  // sret = gst_element_get_state (GST_ELEMENT (playbin), NULL, NULL, -1);
  sret = gst_element_get_state (GST_ELEMENT (pipeline), NULL, NULL, -1);

  switch (sret) {
    case GST_STATE_CHANGE_FAILURE:{
      msg = gst_bus_poll (bus, GST_MESSAGE_ERROR, 0);
      goto got_error_message;
    }
    case GST_STATE_CHANGE_SUCCESS:{
      GstMessage *msg;

      g_print ("Playing ...\n");

      while (1) {
        gint64 dur, pos;

        if (gst_element_query_duration (playbin, GST_FORMAT_TIME, &dur) &&
            gst_element_query_position (playbin, GST_FORMAT_TIME, &pos)) {
          g_print ("  %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\n",
              GST_TIME_ARGS (pos), GST_TIME_ARGS (dur));
        }

        /* check if we finished or if there was an error,
         * but don't wait/block if neither is the case */
        msg = gst_bus_poll (bus, GST_MESSAGE_EOS | GST_MESSAGE_ERROR, 0);

        if (msg && GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR)
          goto got_error_message;

        if (msg && GST_MESSAGE_TYPE (msg) == GST_MESSAGE_EOS) {
          g_print ("Finished.\n");
          break;
        }

        /* sleep for one second */
        g_usleep (G_USEC_PER_SEC * 1);
      }
      break;
    }
    default:
      g_assert_not_reached ();
  }

  /* shut down and free everything */
  gst_element_set_state (playbin, GST_STATE_NULL);
  gst_object_unref (playbin);
  gst_object_unref (bus);
  return;

/* ERRORS */
got_error_message:
  {
    if (msg) {
      GError *err = NULL;
      gchar *dbg_str = NULL;

      gst_message_parse_error (msg, &err, &dbg_str);
      g_printerr ("FAILED to play %s: %s\n%s\n", uri, err->message,
          (dbg_str) ? dbg_str : "(no debugging information)");
      g_error_free (err);
      g_free (dbg_str);
      gst_message_unref (msg);
    } else {
      g_printerr ("FAILED to play %s: unknown error\n", uri);
    }

    /* shut down and free everything */
    gst_element_set_state (playbin, GST_STATE_NULL);
    gst_object_unref (playbin);
    gst_object_unref (bus);
    return;
  }

no_playbin:
  {
    g_error ("Could not create GStreamer 'playbin' element. "
        "Please install it");
    /* not reached, g_error aborts */
    return;
  }

no_autoaudiosink:
  {
    g_error ("Could not create GStreamer 'autoaudiosink' element. "
        "Please install it");
    /* not reached, g_error aborts */
    return;
  }

no_autovideosink:
  {
    g_error ("Could not create GStreamer 'autovideosink' element. "
        "Please install it");
    /* not reached, g_error aborts */
    return;
  }
no_videoflip:
  {
    g_error ("Could not create GStreamer 'videoflip' element. "
        "Please install it");
    /* not reached, g_error aborts */
    return;
  }
no_myfilter:
  {
    g_error ("Could not create GStreamer 'myfilter' element. "
        "Please install it");
    /* not reached, g_error aborts */
    return;
  }
}


