#include <iostream>
#include <stdio.h>
#include "socket_io_client.hpp"
using namespace socketio;

void eventcb(socketio_events&, const Value&)
{
  printf("event cb called \n");
}

int main ()
{

	printf("Sockets io program launched\n"); 
#if 0
	socketio_client_handler_ptr handler(new socketio_client_handler());
	client endpoint(handler);
	client::connection_ptr con = endpoint.get_connection(handler->perform_handshake("ws://localhost:3000"));
	
	handler->start_heartbeat();
	while(1)
	{
	}
#endif
// websocket++ expects urls to begin with "ws://" not "http://"
   std::string uri = "ws://127.0.0.1:3000/";

   try {
      // Create and link handler to websocket++ callbacks.
      socketio_client_handler_ptr handler(new socketio_client_handler());
      client::connection_ptr con;
      client endpoint(handler);

      // Set log level. Leave these unset for no logging, or only set a few for selective logging.
      endpoint.elog().set_level(websocketpp::log::elevel::RERROR);
      endpoint.elog().set_level(websocketpp::log::elevel::FATAL);
      endpoint.elog().set_level(websocketpp::log::elevel::WARN);
      endpoint.alog().set_level(websocketpp::log::alevel::DEVEL);

      std::string socket_io_uri = handler->perform_handshake(uri);
      con = endpoint.get_connection(socket_io_uri);

      // The previous two lines can be combined:
      // con = endpoint.get_connection(handler->perform_handshake(uri));

      endpoint.connect(con);

      boost::thread t(boost::bind(&client::run, &endpoint, false));

      // Wait for a sec before sending stuff
      while (!handler->connected()) {
         sleep(1);
      }

      handler->bind_event("reg_server_message", &eventcb);

      // After connecting, send a connect message if using an endpoint
      //handler->connect_endpoint("{\"address\":\"2@2.com\"}");

      //std::getchar();

      // Then to connect to another endpoint with the same socket, we call connect again.
      //handler->connect_endpoint("/news");

      //std::getchar();
      //handler->emit("regevent","{ type: 'regsuccess', alias: data.address, contact: data.contact}");
      rapidjson::Document doc1;
      doc1.SetObject();
      rapidjson::Value objVal(kObjectType);
      rapidjson::Value objArgs;        
      objVal.AddMember("address", "1@1.com", doc1.GetAllocator());       

      objArgs.SetString("args");
      doc1.AddMember(objArgs, objVal, doc1.GetAllocator());
      handler->emitt("reg_client_message",doc1);
      
      //handler->emit("reg_client_message","{address:2@2.com}");
      //handler->json_message(doc1);
      
    //handler->emit("test", "hello!");

      std::getchar();

      endpoint.stop(false);
   }
   catch (std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      std::getchar();
   }
}
