#include <memory>
#include <exception>


#include "gtest/gtest.h"

#include <pistache/client.h>
#include <pistache/endpoint.h>
#include <pistache/http.h>

#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/details/basic_types.h>

#include <curl/curl.h>

using namespace std;

using namespace Pistache;

using namespace web;
using namespace web::http;
using namespace web::http::client;

static std::string getServerUrl(const Http::Endpoint &server) {
  return std::string("https://localhost:") + server.getPort().toString();
}

/*
static size_t write_cb(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}
*/

///////////////////////////////////////////////////////////////////////////////////////
////// Test Https connect
///////////////////////////////////////////////////////////////////////////////////////
struct HelloHandlerBasic : public Http::Handler {
  HTTP_PROTOTYPE(HelloHandlerBasic)

  void onRequest(const Http::Request & request, Http::ResponseWriter writer) override {
    cout<<"------>Receive request with RURI: "<<endl;
    cout<<"------>"<<request.address().host()<<":"<<request.address().port()<<request.resource()<<request.query().as_str()<<endl;

    if (request.method() == Http::Method::Get) {
      cout<<"------>Method: GET"<<endl;
    }

    writer.send(Http::Code::Ok, "Hello, World!");
  }
};

TEST(RestTest, restTlsBasic) {
  Address addr(Ipv4::any(), Port(10000));
  //auto addr = Address("127.0.0.1", Port(10000));
  Http::Endpoint server(addr);

  auto url = getServerUrl(server);
  cout<<"url: "<<url<<endl;
  cout<<"port: "<<server.getPort().toString()<<endl;

  auto flags = Tcp::Options::ReuseAddr;
  auto server_opts = Http::Endpoint::options().flags(flags);

  try {
    server.init(server_opts);
    //server.init();
    server.setHandler(Http::make_handler<HelloHandlerBasic>());
    server.useSSL("./certs/server.crt", "./certs/server.key");
    server.useSSLAuth("./certs/rootCA.crt");
    server.serveThreaded();
  } catch (exception &e) {
    cout<<e.what()<<endl;
  }

  http_client_config config;
  //config.set_credentials(false);
  config.set_validate_certificates(false);
  config.set_ssl_context_callback( [&](boost::asio::ssl::context& ctx) {
    ctx.set_options(boost::asio::ssl::context::default_workarounds);
    boost::system::error_code ec;
    ctx.use_certificate_file("./certs/client.crt", boost::asio::ssl::context_base::file_format::pem, ec);
    cout<<"error code: %s"<<ec.message()<<endl;
    ctx.use_private_key_file("./certs/client.key", boost::asio::ssl::context_base::file_format::pem);
  });
  //config.set_ssl_context_callback();

  cout<<"Client connect to Server"<<endl;
  shared_ptr<http_client> client;
  client = make_shared<http_client>(U("https://127.0.0.1:10000/"), config);
  //http_client client(U("https://127.0.0.1:1000/"), config);

  // Build request URI and start the request.
  uri_builder builder(U("/search"));
  builder.append_query(U("q"), U("cpprestsdk github"));

  auto response = client->request(methods::GET, builder.to_string()).get();
  if (response.status_code() == status_codes::OK) {
    cout<<"Receive status code: OK"<<endl;
  } else {
    cout<<"Receive status code: "<<response.status_code()<<endl;
  }
  /*
  client.request(methods::GET).then([](http_response response) {
      if (response.status_code() == status_codes::OK) {
          cout<<"Receive status code: OK"<<endl;
          
      } else {
          cout<<"Receive status code: "<<response.status_code()<<endl;
      }
  });
  */  


  server.shutdown();
}

///////////////////////////////////////////////////////////////////////////////////////
////// Test Http header process
///////////////////////////////////////////////////////////////////////////////////////
class Wxl : public Http::Header::Header {
public:
    NAME("Wxl")

    Wxl() : value("wangxiaoling")
    { }

    void parse(const std::string& data) {
      value = data;
    }

    void write(std::ostream& os) const {
        os << value;
    }
private:
    string value;
};

struct HelloHandlerHeader : public Http::Handler {
  HTTP_PROTOTYPE(HelloHandlerHeader)

  void onRequest(const Http::Request & request, Http::ResponseWriter writer) override {
    cout<<"------>Receive request with "<<request.query().as_str()<<endl;
    //Read wellkonw header;
    Http::Header::Collection headers = request.headers();
    ostringstream oss;

    auto ct = headers.get<Http::Header::ContentType>();
    ct->write(oss);
    cout<<"------>ContentType: "<<oss.str()<<endl;  
    oss.str(""); 

    auto author = headers.get<Http::Header::Authorization>();
    author->write(oss);
    cout<<"------>Authorization: "<<oss.str()<<endl;  
    oss.str(""); 

    //Read raw header;    
    auto dy = headers.getRaw("dongyin");
    cout<<"------>"<<dy.name()<<": "<<dy.value()<<endl; 

    //Add wellknow header;
    writer.headers().add<Http::Header::Location>(Http::Header::Location("http://www.baidu.com"));

    //Add raw header; 
    writer.headers().add<Wxl>();
    //writer.headers().addRaw(Http::Header::Raw(move("wxl"), move("wangxiaoling")));
    
    writer.send(Http::Code::Ok, "Hello, World!");
  }
};

TEST(RestTest, RestTlsHeader) {
  //Http::Header::Registry::registerHeader<Wxl>();
  Http::Header::Registry::instance().registerHeader<Wxl>();

  auto addr = Address(Ipv4::any(), Port(10000));
  Http::Endpoint server(addr);

  auto url = getServerUrl(server);

  auto flags = Tcp::Options::ReuseAddr;
  auto server_opts = Http::Endpoint::options().flags(flags);

  try {
    server.init(server_opts);
//    server.init();
    server.setHandler(Http::make_handler<HelloHandlerHeader>());
    server.useSSL("./certs/server.crt", "./certs/server.key");
    server.serveThreaded();
  } catch (exception &e) {
    cout<<e.what()<<endl;
  }

  http_client_config config;
  //config.set_credentials(false);
  config.set_validate_certificates(false);
  //config.set_ssl_context_callback();

  cout<<"------>Client connect to Server"<<endl;
  http_client client(U("https://127.0.0.1:10000/"), config);
  http_request req(methods::GET);

  //Add headers.
  utility::string_t m_authorization = string("123456");
  req.headers().add("dongyin", "dongyin");
  req.headers().add("Authorization", m_authorization);
  req.headers().set_content_type(U("application/json"));

  auto response = client.request(req).get();
  if (response.status_code() == status_codes::OK) {
    cout<<"------>Receive status code: OK"<<endl;

    //Read wellknow header;
    auto loc = response.headers().find("Location");
    cout<<"------>Location: "<<loc->second.c_str()<<endl;

    //Read raw header;
    auto wxl = response.headers().find("wxl");
    if (wxl != response.headers().end()) {
        cout<<"------>wxl: "<<wxl->second<<endl;
    }

    for (auto it = response.headers().begin(); it != response.headers().end(); ++it) {
      cout<<"------>Headers: "<<it->second<<endl;
    }

  } else {
    cout<<"------>Receive status code: "<<response.status_code()<<endl;
  }

  server.shutdown();
}


///////////////////////////////////////////////////////////////////////////////////////
////// Test Http json body process
///////////////////////////////////////////////////////////////////////////////////////
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stream.h>

#include "Util.h"
using namespace crane;

struct HelloHandlerBody : public Http::Handler {
  HTTP_PROTOTYPE(HelloHandlerBody)

  void onRequest(const Http::Request & request, Http::ResponseWriter writer) override {
    cout<<"------>Receive request"<<endl;
    //Read wellkonw header;
    Http::Header::Collection headers = request.headers();
    ostringstream oss;

    auto ct = headers.get<Http::Header::ContentType>();
    ct->write(oss);
    cout<<"------>ContentType: "<<oss.str()<<endl;  
    oss.str(""); 

    string reqJsonBody = request.body();

    rapidjson::Document d;
    d.Parse(reqJsonBody.c_str());

    list<string> h = {"indication"};
    rapidjson::Value* v = crane::Util::fatchJsonValue(d, h);
    cout<<"------>Json value: "<<v->GetString()<<endl;
    ASSERT_STREQ(v->GetString(), "READY")<<"indication = "<<v->GetString()<<std::endl;

    const string rspJsonBody = "{\
        \"subscriptionType\": \"SerAvailabilityNotificationSubscription\",\
        \"callbackReference\": \"string\",\
        \"filteringCriteria\": {\
          \"serNames\": [\
            \"ExampleService\"\
          ],\
          \"states\": [\
            \"ACTIVE\"\
          ],\
          \"isLocal\": true\
          }\
    }";

    
    writer.send(Http::Code::Ok, rspJsonBody);
    //writer.send(Http::Code::Ok, "Hello, World!");
  }
};

TEST(RestTest, RestTlsBody) {

  auto addr = Address("127.0.0.1", Port(10000));
  Http::Endpoint server(addr);

  auto url = getServerUrl(server);

  auto flags = Tcp::Options::ReuseAddr;
  auto server_opts = Http::Endpoint::options().flags(flags);

  try {
    server.init(server_opts);
    //server.init();
    server.setHandler(Http::make_handler<HelloHandlerBody>());
    server.useSSL("./certs/server.crt", "./certs/server.key");
    server.serveThreaded();
  } catch (exception &e) {
    cout<<e.what()<<endl;
  }

  http_client_config config;
  config.set_validate_certificates(false);

  cout<<"------>Client connect to Server"<<endl;
  http_client client(U("https://127.0.0.1:10000/"), config);
  http_request req(methods::GET);

  //Add json body.
  string reqJsonBody = "{\
      \"indication\": \"READY\"\
    }";
  req.headers().set_content_type(U("application/json"));
  req.headers().set_content_length(reqJsonBody.length());
  req.set_body(reqJsonBody);

  auto response = client.request(req).get();

  if (response.status_code() == status_codes::OK) {
    cout<<"------>Receive status code: OK"<<endl;

    const string rspJsonBody = response.content_ready().get().extract_utf8string(true).get();
    cout<<"------>Json: "<<rspJsonBody<<endl;

    rapidjson::Document d;
    d.Parse(rspJsonBody.c_str());

    list<string> h = {"filteringCriteria", "states"};
    rapidjson::Value* v = crane::Util::fatchJsonValue(d, h);
    ASSERT_TRUE(v->IsArray());
    for (auto& s : v->GetArray()) {
        cout<<"----->status Json value: "<<s.GetString()<<endl;
    }
    //cout<<"------>Json value: "<<v->GetString()<<endl;
    //ASSERT_STREQ(v->GetString(), "ACTIVE")<<"states = "<<v->GetString()<<std::endl;
    //Read wellknow header;

    //Read raw header;

  } else {
    cout<<"------>Receive status code: "<<response.status_code()<<endl;
  }

  server.shutdown();
}

///////////////////////////////////////////////////////////////////////////////////////
////// Test Https R-URI process
///////////////////////////////////////////////////////////////////////////////////////
struct HelloHandlerRURI : public Http::Handler {
  HTTP_PROTOTYPE(HelloHandlerRURI)

  void onRequest(const Http::Request & request, Http::ResponseWriter writer) override {
    cout<<"------>Receive request with RURI: "<<endl;
    cout<<"------>"<<request.address().host()<<":"<<request.address().port()<<request.resource()<<request.query().as_str()<<endl;

    if (request.method() == Http::Method::Get) {
      cout<<"------>Method: GET"<<endl;
    }

    writer.send(Http::Code::Ok, "Hello, World!");
  }
};

TEST(RestTest, restTlsRURI) {
  auto addr = Address("127.0.0.1", Port(10000));
  Http::Endpoint server(addr);

  auto url = getServerUrl(server);
  cout<<"url: "<<url<<endl;
  cout<<"port: "<<server.getPort().toString()<<endl;

  auto flags = Tcp::Options::ReuseAddr;
  auto server_opts = Http::Endpoint::options().flags(flags);

  try {
    server.init(server_opts);
    //server.init();
    server.setHandler(Http::make_handler<HelloHandlerRURI>());
    server.useSSL("./certs/server.crt", "./certs/server.key");
    server.serveThreaded();
  } catch (exception &e) {
    cout<<e.what()<<endl;
  }

  http_client_config config;
  //config.set_credentials(false);
  config.set_validate_certificates(false);
  //config.set_ssl_context_callback();

  cout<<"Client connect to Server"<<endl;
  shared_ptr<http_client> client;
  //client = make_shared<http_client>(U("https://127.0.0.1:1000/"), config);
  //http_client client(U("https://127.0.0.1:1000/"), config);
  client = make_shared<http_client>(U("https://127.0.0.1:10000/"), config);

  // Build request URI and start the request.
  uri_builder builder1 = uri_builder();
  //builder1.set_scheme(U("https"));
  //builder1.set_path(U("/mep/mp1/v2"));
  builder1.append_path(U("/mep/mp1/v1"));
  //builder1.append(U("/mep/mp1/v1"));
  builder1.append_query(U("query"), U("cpprestsdk github"));

  auto response = client->request(methods::GET, builder1.to_string()).get();
  if (response.status_code() == status_codes::OK) {
    cout<<"Receive status code: OK"<<endl;
  } else {
    cout<<"Receive status code: "<<response.status_code()<<endl;
  }

  uri_builder builder2 = uri_builder();
  std::string s = "dongyin";
  builder2.append(U("/mep/mp1/v1/services"));
  builder2.append_query(U("serName"), U(s));

  response = client->request(methods::GET, builder2.to_string()).get();
  if (response.status_code() == status_codes::OK) {
    cout<<"Receive status code: OK"<<endl;
  } else {
    cout<<"Receive status code: "<<response.status_code()<<endl;
  }
  
  server.shutdown();

}

///////////////////////////////////////////////////////////////////////////////////////
////// Test Https server send file into body
///////////////////////////////////////////////////////////////////////////////////////
struct HelloHandlerFileInBody : public Http::Handler {
  HTTP_PROTOTYPE(HelloHandlerFileInBody)

  void onRequest(const Http::Request & request, Http::ResponseWriter writer) override {
    cout<<"File In Body: ------>Receive request with RURI: "<<endl;
    cout<<"File In Body: ------>"<<request.address().host()<<":"<<request.address().port()<<request.resource()<<request.query().as_str()<<endl;

    if (request.method() == Http::Method::Get) {
      cout<<"File In Body: ------>Method: GET"<<endl;

      Http::serveFile(writer, "./mp3/cbr_stream.mp3", Http::Mime::MediaType(Pistache::Http::Mime::Type::Image, Pistache::Http::Mime::Subtype::Jpeg))
        .then(
            [](ssize_t bytes) {
              std::cout << "Sent " << bytes << " bytes" << std::endl;
            },
            Async::NoExcept);
          
    }
  }
};

TEST(RestTest, restTlsFileInBody) {
  auto addr = Address("127.0.0.1", Port(10000));
  Http::Endpoint server(addr);

  auto url = getServerUrl(server);
  cout<<"url: "<<url<<endl;
  cout<<"port: "<<server.getPort().toString()<<endl;

  auto flags = Tcp::Options::ReuseAddr;
  auto server_opts = Http::Endpoint::options().flags(flags);

  try {
    server.init(server_opts);
    //server.init();
    server.setHandler(Http::make_handler<HelloHandlerFileInBody>());
    server.useSSL("./certs/server.crt", "./certs/server.key");
    server.serveThreaded();
  } catch (exception &e) {
    cout<<e.what()<<endl;
  }

  http_client_config config;
  //config.set_credentials(false);
  config.set_validate_certificates(false);
  //config.set_ssl_context_callback();

  cout<<"Client connect to Server"<<endl;
  shared_ptr<http_client> client;
  client = make_shared<http_client>(U("https://127.0.0.1:10000/"), config);

  // Build request URI and start the request.
  uri_builder builder1 = uri_builder();
  builder1.append_path(U("/mep/mp1/v1"));
  builder1.append_query(U("query"), U("cpprestsdk github"));

  auto response = client->request(methods::GET, builder1.to_string()).get();
  if (response.status_code() == status_codes::OK) {
    cout<<"File In Body: Receive status code: OK"<<endl;
    auto ct = response.headers().find("Content-Type");
    cout<<"------>Content-Type: "<<ct->second.c_str()<<endl;

    auto cl = response.headers().find("Content-Length");
    cout<<"------>Content-Length: "<<cl->second.c_str()<<endl;

    const string rspJsonBody = response.content_ready().get().extract_utf8string(true).get();
    cout<<"File In Body: ------>Body: "<<rspJsonBody<<endl;    
  } else {
    cout<<"Receive status code: "<<response.status_code()<<endl;
  }

  server.shutdown();

}

///////////////////////////////////////////////////////////////////////////////////////
////// Test Https connect
///////////////////////////////////////////////////////////////////////////////////////
struct HelloHandlerThread : public Http::Handler {
  HTTP_PROTOTYPE(HelloHandlerThread)

  void onRequest(const Http::Request & request, Http::ResponseWriter writer) override {
    cout<<"------>Receive request with RURI: "<<endl;
    cout<<"------>"<<request.address().host()<<":"<<request.address().port()<<request.resource()<<request.query().as_str()<<endl;

    if (request.method() == Http::Method::Get) {
      cout<<"------>Method: GET"<<endl;
    }

    writer.send(Http::Code::Ok, "Hello, World!");
  }
};

TEST(RestTest, restTlsThread) {
  auto addr = Address("127.0.0.1", Port(10000));
  Http::Endpoint server(addr);

  auto url = getServerUrl(server);
  cout<<"url: "<<url<<endl;
  cout<<"port: "<<server.getPort().toString()<<endl;

  auto flags = Tcp::Options::ReuseAddr;
  auto server_opts = Http::Endpoint::options().flags(flags);

  try {
    server.init(server_opts);
    //server.init();
    server.setHandler(Http::make_handler<HelloHandlerBasic>());
    server.useSSL("./certs/server.crt", "./certs/server.key");
    server.useSSLAuth("./certs/rootCA.crt");
    server.serveThreaded();
  } catch (exception &e) {
    cout<<e.what()<<endl;
  }

  http_client_config config;
  //config.set_credentials(false);
  config.set_validate_certificates(false);
  config.set_ssl_context_callback( [&](boost::asio::ssl::context& ctx) {
    ctx.set_options(boost::asio::ssl::context::default_workarounds);
    boost::system::error_code ec;
    ctx.use_certificate_file("./certs/client.crt", boost::asio::ssl::context_base::file_format::pem, ec);
    cout<<"error code: %s"<<ec.message()<<endl;
    ctx.use_private_key_file("./certs/client.key", boost::asio::ssl::context_base::file_format::pem);
    

  });
  //config.set_ssl_context_callback();

  cout<<"Client connect to Server"<<endl;
  shared_ptr<http_client> client;
  client = make_shared<http_client>(U("https://127.0.0.1:10000/"), config);
  //http_client client(U("https://127.0.0.1:1000/"), config);

  // Build request URI and start the request.
  uri_builder builder(U("/search"));
  builder.append_query(U("q"), U("cpprestsdk github"));

  auto response = client->request(methods::GET, builder.to_string()).get();
  if (response.status_code() == status_codes::OK) {
    cout<<"Receive status code: OK"<<endl;
  } else {
    cout<<"Receive status code: "<<response.status_code()<<endl;
  }
  /*
  client.request(methods::GET).then([](http_response response) {
      if (response.status_code() == status_codes::OK) {
          cout<<"Receive status code: OK"<<endl;
          
      } else {
          cout<<"Receive status code: "<<response.status_code()<<endl;
      }
  });
  */  


  server.shutdown();
}