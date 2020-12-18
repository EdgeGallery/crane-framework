/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2020 root <<user@hostname.org>>
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
 * SECTION:element-myfilter
 *
 * FIXME:Describe myfilter here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! myfilter ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>

#include "gstmyfilter.h"

GST_DEBUG_CATEGORY_STATIC (gst_my_filter_debug);
#define GST_CAT_DEFAULT gst_my_filter_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (
    "audio/x-raw, "
    //"format = (string) " GST_AUDIO_NE (S16) ", "
    "format = (string) " "S16LE" ", "
    "channels = (int) { 1, 2 }, "
    "rate = (int) [ 8000, 96000 ]"
    )
  );

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

#define gst_my_filter_parent_class parent_class
G_DEFINE_TYPE (GstMyFilter, gst_my_filter, GST_TYPE_ELEMENT);

static void gst_my_filter_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_my_filter_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static gboolean gst_my_filter_sink_event (GstPad * pad, GstObject * parent, GstEvent * event);
static GstFlowReturn gst_my_filter_chain (GstPad * pad, GstObject * parent, GstBuffer * buf);

//add by dongyin
static GstStateChangeReturn
    gst_my_filter_change_state (GstElement *element, GstStateChange transition);
static gboolean
gst_my_filter_src_query (GstPad    *pad,
                         GstObject *parent,
                         GstQuery  *query);
/////////////////////////////////////////////////////

/* GObject vmethod implementations */

/* initialize the myfilter's class */
static void
gst_my_filter_class_init (GstMyFilterClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;

  gobject_class->set_property = gst_my_filter_set_property;
  gobject_class->get_property = gst_my_filter_get_property;

  g_object_class_install_property (gobject_class, PROP_SILENT,
      g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
          FALSE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  gst_element_class_set_details_simple(gstelement_class,
    "MyFilter",
    "Dongyin/MyFirstPlugin",
    "Just try gst plugin",
    "dongyin <<dongyin@huawei.com>>");

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&src_factory));
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_factory));

   gstelement_class->set_state = gst_my_filter_change_state;
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_my_filter_init (GstMyFilter * filter)
{
  filter->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink");
  //add by dongyin
  gst_pad_set_query_function(filter->srcpad,
                              GST_DEBUG_FUNCPTR(gst_my_filter_src_query));
  /////////////////
  gst_pad_set_event_function (filter->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_my_filter_sink_event));
  gst_pad_set_chain_function (filter->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_my_filter_chain));
  GST_PAD_SET_PROXY_CAPS (filter->sinkpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->sinkpad);

  filter->srcpad = gst_pad_new_from_static_template (&src_factory, "src");
  GST_PAD_SET_PROXY_CAPS (filter->srcpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->srcpad);

  filter->silent = FALSE;
}

static void
gst_my_filter_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMyFilter *filter = GST_MY_FILTER (object);

  switch (prop_id) {
    case PROP_SILENT:
      filter->silent = g_value_get_boolean (value);
      g_print ("Silent argument was changed to %s\n",
               filter->silent ? "true" : "false");
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_my_filter_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstMyFilter *filter = GST_MY_FILTER (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_value_set_boolean (value, filter->silent);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* GstElement vmethod implementations */

/* this function handles sink events */
static gboolean
gst_my_filter_sink_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
  GstMyFilter *filter;
  gboolean ret;

  filter = GST_MY_FILTER (parent);

  GST_LOG_OBJECT (filter, "Received %s event: %" GST_PTR_FORMAT,
      GST_EVENT_TYPE_NAME (event), event);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS:
    {
      GstCaps * caps;

      gst_event_parse_caps (event, &caps);
      /* do something with the caps */
      g_print("Cap type: %lu", caps->mini_object.type);

      /* and forward */
      ret = gst_pad_event_default (pad, parent, event);
      //ret = gst_pad_push_event(filter->srcpad, event);
      break;
    }

    case GST_EVENT_EOS:
    {
        GST_LOG_OBJECT(filter, "Received EOS EVENT");
        ret = gst_pad_event_default(pad, parent, event);
        break;
    }

    default:
      ret = gst_pad_event_default (pad, parent, event);
      break;
  }
  return ret;
}

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_my_filter_chain (GstPad * pad, GstObject * parent, GstBuffer * buf)
{
  GstMyFilter *filter;

  filter = GST_MY_FILTER (parent);

  if (filter->silent == FALSE) {
      g_print("I'm plugged, therefore I'm in here.\n");
  } else {
      g_print("slient == TRUE. \n");
  }

  /* just push out the incoming buffer without touching it */
  return gst_pad_push (filter->srcpad, buf);
}


/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
myfilter_init (GstPlugin * myfilter)
{
  /* debug category for fltering log messages
   *
   * exchange the string 'Template myfilter' with your description
   */
  GST_DEBUG_CATEGORY_INIT (gst_my_filter_debug, "myfilter",
      0, "Template myfilter");

  return gst_element_register (myfilter, "myfilter", GST_RANK_NONE,
      GST_TYPE_MY_FILTER);
}

static GstStateChangeReturn
gst_my_filter_change_state (GstElement *element, GstStateChange transition)
{
    GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
    GstMyFilter *filter = GST_MY_FILTER (element);

    switch (transition) {
        case GST_STATE_CHANGE_NULL_TO_READY:
            g_print("Receive: GST_STATE_CHANGE_NULL_TO_READY \n");
            break;
        default:
            break;
    }

    ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);
    if (ret == GST_STATE_CHANGE_FAILURE)
        return ret;

    switch (transition) {
        case GST_STATE_CHANGE_READY_TO_NULL:
            g_print("Receive: GST_STATE_CHANGE_READY_TO_NULL \r\n");
            break;
        default:
            break;
    }

    return ret;
}

static gboolean
gst_my_filter_src_query (GstPad    *pad,
                         GstObject *parent,
                         GstQuery  *query)
{
    gboolean ret;
    GstMyFilter *filter = GST_MY_FILTER (parent);

    switch (GST_QUERY_TYPE (query)) {
        case GST_QUERY_POSITION:
            /* we should report the current position */
            break;
        case GST_QUERY_DURATION:
            /* we should report the duration here */
            break;
        case GST_QUERY_CAPS:
            /* we should report the supported caps here */
            break;
        default:
            /* just call the default handler */
            ret = gst_pad_query_default (pad, parent, query);
            break;
    }
    return ret;
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "myfirstmyfilter"
#endif

/* gstreamer looks for this structure to register myfilters
 *
 * exchange the string 'Template myfilter' with your myfilter description
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    myfilter,
    "Template myfilter",
    myfilter_init,
    VERSION,
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
)
