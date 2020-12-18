#include <string>
#include <memory>

#include <iostream>
       
#include <pistache/http.h>
#include <pistache/router.h>
//#include <pistache/endpoint.h>


//Gstreamer H files
#include <gst/gst.h>

//Crane H files
#include "crane.h"
#include "Util.h"

//Crane app plugin interface H file, which is created by APP developer.
#include "playerInterface.h"

using namespace NS_CRANE;

static void testPostProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  unused(request);
  g_print("Enter postProcess function.\n");
  response.send(Pistache::Http::Code::Ok);
  return;
}

static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data);
static void cb_newpad (GstElement *decodebin, GstPad *pad, gpointer data);

typedef struct UserDataBusCall {
    GMainLoop*      loop;
    Itf_Player*     player;
}UserDataBusCall;

int main(int argc, char** argv) {
    //*** Create a singleton object of plugin frame.
    AbstractPluginFrame* pPluginFrame = AbstractPluginFrame::getPluginFrame();

    //*** Initialize app's server restful process function.
    pPluginFrame->setServerRoutes(HttpMethod::POST, string("/api/v1/img/dongyin"), &testPostProcess);

    //*** Initialize Crane and Gstreamer system.
    pPluginFrame->init(argc, argv, CRANE_ALL);

    //*** Create "PlayerImplCD" instance of crane plugin.
    Itf_Player* playerCD = dynamic_cast<Itf_Player*>(pPluginFrame->create("Itf_Player", "PlayerImplCD", "my first plugin instance"));
    
    //*** Using the "PlayerImplCD" plugin instance.
    playerCD->play("......");

    //*** Gstreamer app's code.
    GstStateChangeReturn ret;
    GstElement *pipeline, *filesrc, *decoder, *plugin, *filesink;
    GstElement *convert1, *convert2;
    GMainLoop *loop;
    GstBus *bus;
    guint watch_id;

    loop = g_main_loop_new (NULL, FALSE);
    if (argc != 3) {
      g_print ("Usage: %s <mp3 input filename> and <mp3 output filename>\n", argv[0]);
      return -1;
    }

    pipeline = gst_pipeline_new("my_pipeline");

    /* watch for messages on the pipeline's bus (note that this will only
     * work like this when a GLib main loop is running) */
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    UserDataBusCall userData = {loop, playerCD};
    //watch_id = gst_bus_add_watch (bus, bus_call, loop);
    watch_id = gst_bus_add_watch (bus, bus_call, &userData);
    gst_object_unref (bus);

    filesrc  = (GstElement*)pPluginFrame->create("filesrc", "my_filesource");
    decoder = (GstElement*)pPluginFrame->create("decodebin", "decoderbin");

    /* putting an audioconvert element here to convert the output of the
     * decoder into a format that my_filter can handle (we are assuming it
     * will handle any sample rate here though) */
    convert1 = (GstElement*)pPluginFrame->create("audioconvert", "audioconvert1");
 
    /* use "identity" here for a filter that does nothing */
    plugin = (GstElement*)pPluginFrame->create("plugin", "plugin");
 
    /* there should always be audioconvert and audioresample elements before
     * the audio sink, since the capabilities of the audio sink usually vary
     * depending on the environment (output used, sound card, driver etc.) */
    convert2 = (GstElement*)pPluginFrame->create("audioconvert", "audioconvert2");
    filesink     = (GstElement*)pPluginFrame->create("filesink", "filesink");

    if (!filesink || !decoder) {
      g_print ("Decoder or output could not be found - check your install\n");
      return -1;
    } else if (!convert1 || !convert2 /*|| !resample*/) {
      g_print ("Could not create audioconvert or audioresample element, "
               "check your installation\n");
      return -1;
    } else if (!plugin) {
      g_print ("Your self-written filter could not be found. Make sure it "
               "is installed correctly in $(libdir)/gstreamer-1.0/ or "
               "~/.gstreamer-1.0/plugins/ and that gst-inspect-1.0 lists it. "
               "If it doesn't, check with 'GST_DEBUG=*:2 gst-inspect-1.0' for "
               "the reason why it is not being loaded.");
      return -1;
    }
 
    g_object_set (G_OBJECT (filesrc), "location", argv[1], NULL);
    g_object_set (G_OBJECT (filesink), "location", argv[2], NULL);
 
    gst_bin_add_many(GST_BIN(pipeline), filesrc, decoder, NULL);
    gst_element_link(filesrc, decoder);
 
    GstElement *audio = gst_bin_new("audiobin");
    GstPad *audiopad = gst_element_get_static_pad(convert1, "sink");
    gst_bin_add_many(GST_BIN(audio), convert1, plugin, convert2, filesink, NULL);
    gst_element_link_many(convert1, plugin, convert2, filesink, NULL);
    gst_element_add_pad(audio, gst_ghost_pad_new("sink", audiopad));
 
    g_signal_connect(decoder, "pad-added", G_CALLBACK(cb_newpad), audio);
    gst_object_unref(audiopad);
 
    gst_bin_add(GST_BIN(pipeline), audio);
 
    /* run */
    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
      GstMessage *msg;
 
      g_print ("Failed to start up pipeline!\n");
 
      /* check if there is an error message with details on the bus */
      msg = gst_bus_poll (bus, GST_MESSAGE_ERROR, 0);
      if (msg) {
        GError *err = NULL;
 
        gst_message_parse_error (msg, &err, NULL);
        g_print ("ERROR: %s\n", err->message);
        g_error_free (err);
        gst_message_unref (msg);
      }
      return -1;
    }
 
    g_main_loop_run (loop);

    std::this_thread::sleep_for(std::chrono::seconds(20));
 
    /* clean up */
    gst_element_set_state (pipeline, GST_STATE_NULL);
    //gst_object_unref (pipeline); 
    //*** Destory Gstreamer plugins.
    pPluginFrame->destory(pipeline);
    g_source_remove (watch_id);
    g_main_loop_unref (loop);

    /* GStreamer-CRITICAL **: gst_object_unref: assertion '((GObject *) object)->ref_count > 0' failed*/
    //so I should not release the gstreamer plugin again.
    
    //*** Destory Crane plugin.
    pPluginFrame->destory(playerCD);
    //delete pPluginFrame;

    return CRANE_SUCC;
}

static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data) {
  //GMainLoop *loop = data;
  //GMainLoop *loop = static_cast<GMainLoop*>(static_cast<UserDataBusCall*>(data)->loop);

  NS_CRANE::unused(bus);

  GMainLoop* loop = static_cast<UserDataBusCall*>(data)->loop;
  Itf_Player* player = static_cast<UserDataBusCall*>(data)->player;
 
  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_EOS:
      g_print ("End-of-stream\n");
      g_main_loop_quit (loop);
      break;
    case GST_MESSAGE_ERROR: {
      gchar *debug = NULL;
      GError *err = NULL;
 
      gst_message_parse_error (msg, &err, &debug);
 
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
 
      if (debug) {
        g_print ("Debug details: %s\n", debug);
        g_free (debug);
      }
 
      g_main_loop_quit (loop);
      break;
    }
    //Receive and process the message from plugin.
    case GST_MESSAGE_INFO: {
        const GstStructure* s = gst_message_get_structure(msg);
        const gchar* info = gst_structure_get_string(s, "info");
        if (info != nullptr) {
            //g_print("Info from plugin: %s\n", info);
            std::string str(info);
            player->play(str);
        }
        break;
    }
    
    default:
      break;
  }
 
  return TRUE;
}

static void cb_newpad (GstElement *decodebin, GstPad *pad, gpointer data) {
    NS_CRANE::unused(decodebin);

	GstCaps *caps;
	GstStructure *str;
	GstPad *audiopad;
 
	/* only link once */
	//audiopad = gst_element_get_static_pad (audio, "sink");
	audiopad = gst_element_get_static_pad (GST_ELEMENT_CAST(data), "sink");
	if (GST_PAD_IS_LINKED (audiopad)) {
		g_object_unref (audiopad);
		return;
	}
 
	/* check media type */
	caps = gst_pad_query_caps (pad, NULL);
	str = gst_caps_get_structure (caps, 0);
	if (!g_strrstr (gst_structure_get_name (str), "audio")) {
		gst_caps_unref (caps);
		gst_object_unref (audiopad);
		return;
	}
 
	gst_caps_unref (caps);
 
	/* link’n’play */
	gst_pad_link (pad, audiopad);
	g_object_unref (audiopad);
}