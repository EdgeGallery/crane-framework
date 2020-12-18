#ifndef __CRANE_RESTSERVERPROCESS_H__
#define __CRANE_RESTSERVERPROCESS_H__

#include <time.h>
#include <chrono> 

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "craneMacro.h"
#include "Config.h"
#include "Data.h"
#include "Util.h"

using namespace std;
using namespace Pistache;
using namespace rapidjson;

namespace NS_CRANE {

    void postProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    void getProcess(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
}

#endif