/* socket_io_client.cpp
 * Evan Shimizu, June 2012
 * websocket++ handler implementing the socket.io protocol.
 * https://github.com/LearnBoost/socket.io-spec 
 *
 * This implementation uses the rapidjson library.
 * See examples at https://github.com/kennytm/rapidjson.
 */
//#include "talk/examples/peerconnection/client/peer_connection_client.h"
//#include "talk/examples/peerconnection/client/defaults.h"
#include "socket_io_client.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include <sstream>
#include "rapidjson/stringbuffer.h"

// Comment this out to disable handshake logging to stdout
#define LOG(x) std::cout << x
//#define LOG(x)
#define JSEP_SIP_GATEWAY_SUPPORT
#ifdef JSEP_SIP_GATEWAY_SUPPORT
#define HARDCODED_PEERID 1
#endif

using socketio::socketio_client_handler;
using socketio::socketio_events;
using websocketpp::client;
using namespace rapidjson;

// Event handlers
//connection_ptr m_conTemp;//Shayo
bool conductor_OneWayCall = false;
void socketio_events::example(const Value& args)
{
   // Event handlers are responsible for knowing what's in args.
   // This function expects a string as the first argument, and doesn't care about the rest.
   std::cout << "Hello! You've just successfully tested this event. Args[0]: " << args[SizeType(0)].GetString() << std::endl;
}
#ifdef JSEP_SIP_GATEWAY_SUPPORT

void socketio_events::serverMsg(const Value& args)
{
#if 0
   // Event handlers are responsible for knowing what's in args.
   // This function expects a string as the first argument, and doesn't care about the rest.
   std::cout << "Server message GetType: " << args.GetType() << std::endl;
   
   // Check if the type of the response is an array
   if (args.GetType() == kArrayType)
   {
     std::cout << "Server message args.Size(): " << args.Size() << std::endl;
     // Check how many elements are in the array
     for (int icount=0; icount < args.Size(); icount ++)
     {
       // Take out an element from an array
       std::cout << "Elements received: Id: " << icount << "Type: " << args[icount].GetType() << std::endl;
       switch(args[icount].GetType())
       {
	 case kObjectType:
	 {
	   	// member iterator
		Value::ConstMemberIterator itr = args[icount].MemberBegin(); 
		while (itr != args[icount].MemberEnd())
		{
		  std::cout << "Elements received: Name: " << itr->name.GetString() << std::endl;
		  std::cout << "Elements received: Value: " << itr->value.GetString() << std::endl;
		  ++itr;
		}
		std::cout << "Type: " << args[icount]["type"].GetString() << std::endl;
		
		// Check if the response from the server is a success
		if (!std::strcmp(args[icount]["type"].GetString(), "regsuccess"))
		{
		  // Login success
		  std::cout << "Login success" << std::endl;
		}
	 }
	 break;
	 case kStringType:
	 break;
	 case kNumberType:
	 break;
	 default:
	   std::cout << "Unknown type !!!" << std::endl;
	 break;
       }
     }
   }
#endif
}

void socketio_events::rtcMsg( const Value& args)
{
std::cout << "Server message GetType: " << args.GetType() << std::endl;
#if 0   
  // Event handlers are responsible for knowing what's in args.
   // This function expects a string as the first argument, and doesn't care about the rest.
   std::cout << "Server message GetType: " << args.GetType() << std::endl;
   
   // Check if the type of the response is an array
   if (args.GetType() == kArrayType)
   {
     std::cout << "Server message args.Size(): " << args.Size() << std::endl;
     // Check how many elements are in the array
     for (int icount=0; icount < args.Size(); icount ++)
     {
       // Take out an element from an array
       std::cout << "Elements received: Id: " << icount << "Type: " << args[icount].GetType() << std::endl;
       switch(args[icount].GetType())
       {
	 case kObjectType:
	 {
	   	// member iterator
		Value::ConstMemberIterator itr = args[icount].MemberBegin(); 
		while (itr != args[icount].MemberEnd())
		{
		  std::cout << "Elements received: Name: " << itr->name.GetString() << std::endl;
		  std::cout << "Elements received: Value: " << itr->value.GetString() << std::endl;
		  ++itr;
		}
		std::cout << "Type: " << args[icount]["type"].GetString() << std::endl;
		
		// Check if the response from the server is a success
		if (!std::strcmp(args[icount]["type"].GetString(), "offer"))
		{
		  // Incoming call
		  std::cout << "Incoming call from address:" << args[icount]["fromAlias"].GetString() << std::endl;
		  //PeerConnectionClient::GetCallback()->OnPeerConnected(HARDCODED_PEERID, args[icount]["fromAlias"].GetString());
		  //handler->appEventCb(HARDCODED_PEERID, args[icount]["fromAlias"].GetString());
		}
	 }
	 break;
	 case kStringType:
	 break;
	 case kNumberType:
	 break;
	 default:
	   std::cout << "Unknown type !!!" << std::endl;
	 break;
       }
     }
   }
#endif   
}


#endif
// Websocket++ client handler


void socketio_client_handler::on_fail(connection_ptr con)
{
   stop_heartbeat();
   m_con = connection_ptr();
   m_connected = false;

   LOG("Connection failed." << std::endl);
}

void socketio_client_handler::on_open(connection_ptr con)
{
   std::cout << "socketio_client_handler::on_open" << std::endl;
   m_con = con;
  // m_conTemp = con;//Shayo
 //  std::cout << "m_conTemp:" << m_con << std::endl;
   // Create the heartbeat timer and use the same io_service as the main event loop.
#ifndef BOOST_NO_CXX11_SMART_PTR
   m_heartbeatTimer = std::unique_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(con->get_io_service(), boost::posix_time::seconds(0)));
#else
   m_heartbeatTimer = boost::shared_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(con->get_io_service(), boost::posix_time::seconds(0)));
#endif
   start_heartbeat();
   m_connected = true;

   LOG("Connected." << std::endl);
}

void socketio_client_handler::on_close(connection_ptr con)
{  
   stop_heartbeat();
   m_connected = false;
   m_con = connection_ptr();

   LOG("Client Disconnected." << std::endl);
}

void socketio_client_handler::on_message(connection_ptr con, message_ptr msg)
{
 std::cout << "socketio_client_handler::on_message() "<<msg->get_payload ().c_str();
   // Parse the incoming message according to socket.IO rules
   parse_message(msg->get_payload());
}

// Client Functions
// Note from websocketpp code: methods (except for perform_handshake) will be called
// from outside the io_service.run thread and need to be careful to not touch unsynchronized
// member variables.

std::string socketio_client_handler::perform_handshake(std::string url, std::string socketIoResource)
{
   // Log currently not accessible from this function, outputting to std::cout
   LOG("Parsing websocket uri..." << std::endl);
   websocketpp::uri uo(url);
   m_resource = uo.get_resource();

   // Declare boost io_service
   boost::asio::io_service io_service;

   LOG("Connecting to Server..." << std::endl);

   // Resolve query
   tcp::resolver r(io_service);
   tcp::resolver::query q(uo.get_host(), uo.get_port_str());
   tcp::socket socket(io_service);
   boost::asio::connect(socket, r.resolve(q));

   // Form initial post request.
   boost::asio::streambuf request;
   std::ostream reqstream(&request);

   reqstream << "POST " << socketIoResource << "/1/ HTTP/1.0\r\n";
   reqstream << "Host: " << uo.get_host() << "\r\n";
   reqstream << "Accept: */*\r\n";
   reqstream << "Connection: close\r\n\r\n";

   LOG("Sending Handshake Post Request..." << std::endl);

   // Write request.
   boost::asio::write(socket, request);

   // Receive response
   boost::asio::streambuf response;
   boost::asio::read_until(socket, response, "\r\n");

   // Parse response
   std::istream resp_stream(&response);

   // Extract HTTP version, status, and message.
   std::string httpver;
   unsigned int status;
   std::string status_msg;

   resp_stream >> httpver >> status;
   std::getline(resp_stream, status_msg);

   // Log response
   LOG("Received Response:" << std::endl);
   LOG(httpver << " " << status << std::endl);
   LOG(status_msg << std::endl);

   // Read response headers. Terminated with double newline.
   boost::asio::read_until(socket, response, "\r\n\r\n");

   // Log headers.
   std::string header;

   while (std::getline(resp_stream, header) && header[0] != '\r')
   {
      LOG(header << std::endl);
   }

   // Handle errors
   if (!resp_stream || httpver.substr(0, 5) != "HTTP/")
   {
      std::cerr << "Invalid HTTP protocol: " << httpver << std::endl;
      throw websocketpp::exception("Socket.IO Handshake: Invalid HTTP protocol: " + httpver, websocketpp::error::GENERIC);
   }
   switch (status)
   {
   case(200):
      LOG("Server accepted connection." << std::endl);
      break;
   case(401):
   case(503):
      std::cerr << "Server rejected client connection" << std::endl;
      throw websocketpp::exception("Socket.IO Handshake: Server rejected connection with code " + status, websocketpp::error::GENERIC);
      break;
   default:
      std::cerr << "Server returned unknown status code: " << status << std::endl;
      throw websocketpp::exception("Socket.IO Handshake: Server responded with unknown code " + status, websocketpp::error::GENERIC);
      break;
   }

   // Get the body components.
   std::string body;

   std::getline(resp_stream, body, '\0');
   boost::cmatch matches;
   const boost::regex expression("([0-9a-zA-Z_-]*):([0-9]*):([0-9]*):(.*)");

   if (boost::regex_match(body.c_str(), matches, expression))
   {
      m_sid = matches[1];
      
      m_heartbeatTimeout = atoi(matches[2].str().c_str());
      if (m_heartbeatTimeout <= 0) m_heartbeatTimeout = 0;
      
      m_disconnectTimeout = atoi(matches[3].str().c_str());
      
      m_transports = matches[4];
      if (m_transports.find("websocket") == std::string::npos)
      {
         std::cerr << "Server does not support websocket transport: " << m_transports << std::endl;
         websocketpp::exception("Socket.IO Handshake: Server does not support websocket transport", websocketpp::error::GENERIC);
      }
   }

   // Log socket.IO info
   LOG(std::endl << "Session ID: " << m_sid << std::endl);
   LOG("Heartbeat Timeout: " << m_heartbeatTimeout << std::endl);
   LOG("Disconnect Timeout: " << m_disconnectTimeout << std::endl);
   LOG("Allowed Transports: " << m_transports << std::endl);

   // Form the complete connection uri. Default transport method is websocket (since we are using websocketpp).
   // If secure websocket connection is desired, replace ws with wss.
   std::stringstream iouri;
   iouri << "ws://" << uo.get_host() << ":" << uo.get_port() << socketIoResource << "/1/websocket/" << m_sid;
   m_socketIoUri = iouri.str();
   return m_socketIoUri;
}

void socketio_client_handler::send(const std::string &msg)
{
   std::cout << "socketio_client_handler::send" << std::endl;
   if (!m_con)
   {
      std::cout << "Error:::: No active session" << std::endl;
      return;
   }
   std::cout << "socketio_client_handler::send--cp1" << std::endl;
   m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Sent: " << msg << websocketpp::log::endl;
   m_con->send(msg);
}

void socketio_client_handler::send(unsigned int type, std::string endpoint, std::string msg, unsigned int id)
{
   // Construct the message.
   // Format: [type]:[id]:[endpoint]:[msg]
   std::cout << "socketio_client_handler::send+++++++" << std::endl;
   std::stringstream package;
   package << type << ":";
   if (id > 0) package << id;
   package << ":" << endpoint << ":" << msg;

   send(package.str());
}

void socketio_client_handler::connect_endpoint(std::string endpoint)
{
    send("1::" + endpoint);
}

void socketio_client_handler::disconnect_endpoint(std::string endpoint)
{
    send("0::" + endpoint);
}

void socketio_client_handler::emitt(std::string name, Document& args, std::string endpoint, unsigned int id)
{
   // Add the name to the data being sent.
   Value n;
   n.SetString(name.c_str(), name.length(), args.GetAllocator());
   printf("n.flags_ 0x%x\n", n.GetType());

   args.AddMember("name", n, args.GetAllocator()); 


   
   // Stringify json
   std::ostringstream outStream;
   StreamWriter<std::ostringstream> writer(outStream);
   args.Accept(writer);

   std::cout << "outstream:: \n" << outStream.str();
   
   // Extract the message from the stream and format it.
   std::string package(outStream.str());
   send(5, endpoint, package.substr(0, package.find('\0')), id);
}

void socketio_client_handler::emitt(std::string name, std::string arg0, std::string endpoint, unsigned int id) {
   Document d;
   Document d1;
   d.SetObject();
   Value addr("address", 7);
   Value addrValue("1@1.com", 7);
   Value addrFull;
   Value args;
   Value value1;
   Value objVal(kObjectType);        
   objVal.AddMember("address", "1@1.com", d.GetAllocator());       

   //args.SetArray();

#if 0
   // AddMember()
    Value name("A", 1);
    Value value("Apple", 5);
    x.AddMember(name, value, allocator);
    //name.SetString("B");
    name.SetString("B", 1);
    //value.SetString("Banana");
    value.SetString("Banana", 6);
    x.AddMember(name, value, allocator);
	
   //args.PushBack(arg0.c_str(), d.GetAllocator());
#endif   
   //args.SetString(arg0.c_str());
   args.SetString(arg0.c_str());
   //args.SetObject();
   std::cout << "ARGS::\n" << args.GetString() << std::endl;
   value1.SetString("args");
   printf("args.flags_ 0x%x\n", args.GetType());
   printf("value1.flags_ 0x%x\n", value1.GetType());
   //d.AddMember(value1, addr, d.GetAllocator());
   //addrFull.AddMember(addr, addrValue, d.GetAllocator());
   d.AddMember(value1, args, d.GetAllocator());
   std::cout << "D:: " << d.IsObject() << std::endl;
   std::string argument_ = "{\"name\":\""+name+"\",\"args\":["+arg0+"]}";
   std::cout << "Message to POST:: " << argument_.c_str() << std::endl;
    
//   emitt(name, d, endpoint, id);
   d.AddMember("name",name.c_str(), d.GetAllocator());
   send(5, endpoint, argument_, id);

}

void socketio_client_handler::message(std::string msg, std::string endpoint, unsigned int id)
{
   send(3, endpoint, msg, id);
}

void socketio_client_handler::json_message(Document& json, std::string endpoint, unsigned int id)
{
   // Stringify json
   std::ostringstream outStream;
   StreamWriter<std::ostringstream> writer(outStream);
   json.Accept(writer);

   // Extract the message from the stream and format it.
   std::string package(outStream.str());
   send(4, endpoint, package.substr(0, package.find('\0')), id);
}

bool socketio_client_handler::bind_event(std::string name, eventFunc func)
{
   // If the name is already in use, don't replace it.
   if (m_events.count(name) == 0)
   {
      m_events[name] = func;
      return true;
   }

   return false;
}

bool socketio_client_handler::unbind_event(std::string name)
{
   // Delete from map if the key exists.
   if (m_events.count(name) > 0)
   {
      m_events.erase(m_events.find(name));
      return true;
   }
   return false;
}

void socketio_client_handler::close()
{
   if (!m_con)
   {
      std::cerr << "Error: No active session" << std::endl;
      return;
   }

   send(3, "disconnect", "");
   m_con->close(websocketpp::close::status::GOING_AWAY, "");
}

void socketio_client_handler::start_heartbeat()
{
   // Heartbeat is already active so don't do anything.
   if (m_heartbeatActive) return;
   
   // Check valid heartbeat wait time.
   if (m_heartbeatTimeout > 0)
   {
      m_heartbeatTimer->expires_at(m_heartbeatTimer->expires_at() + boost::posix_time::seconds(m_heartbeatTimeout));
      m_heartbeatActive = true;
      m_heartbeatTimer->async_wait(boost::bind(&socketio_client_handler::heartbeat, this));

      m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Sending heartbeats. Timeout: " << m_heartbeatTimeout << websocketpp::log::endl;
   }
}

void socketio_client_handler::stop_heartbeat()
{
   // Timer is already stopped.
   if (!m_heartbeatActive) return;

   // Stop the heartbeats.
   m_heartbeatActive = false;
   m_heartbeatTimer->cancel();

   if (m_con)
      m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Stopped sending heartbeats." << websocketpp::log::endl;
}

void socketio_client_handler::send_heartbeat()
{
   if (m_con)
   {
      m_con->send("2::");
      m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Sent Heartbeat" << websocketpp::log::endl;
   }
}

void socketio_client_handler::heartbeat()
{
   send_heartbeat();
   m_heartbeatTimer->expires_at(m_heartbeatTimer->expires_at() + boost::posix_time::seconds(m_heartbeatTimeout));
   m_heartbeatTimer->async_wait(boost::bind(&socketio_client_handler::heartbeat, this));
}

void socketio_client_handler::parse_message(const std::string &msg)
{
   // Parse response according to socket.IO rules.
   // https://github.com/LearnBoost/socket.io-spec

   boost::cmatch matches;
   const boost::regex expression("([0-8]):([0-9]*):([^:]*)[:]?(.*)");
   
   if(boost::regex_match(msg.c_str(), matches, expression))
   {
      int type;
      int msgId;
      Document json;

      // Attempt to parse the first match as an int.
      std::stringstream convert(matches[1]);
      if (!(convert >> type)) type = -1;

      // Store second param for parsing as message id. Not every type has this, so if it's missing we just use 0 as the ID.
      std::stringstream convertId(matches[2]);
      if (!(convertId >> msgId)) msgId = 0;

      switch (type)
      {
      // Disconnect
      case (0):
         m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received message type 0 (Disconnect)" << websocketpp::log::endl;
         close();
         break;
      // Connection Acknowledgement
      case (1):
         m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received Message type 1 (Connect): " << msg << websocketpp::log::endl;
         break;
      // Heartbeat
      case (2):
         m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received Message type 2 (Heartbeat)" << websocketpp::log::endl;
         send_heartbeat();
         break;
      // Message
      case (3):
         m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received Message type 3 (Message): " << msg << websocketpp::log::endl;
         on_socketio_message(msgId, matches[3], matches[4]);
         break;
      // JSON Message
      case (4):
         m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received Message type 4 (JSON Message): " << msg << websocketpp::log::endl;
         // Parse JSON
         if (json.Parse<0>(matches[4].str().c_str()).HasParseError())
         {
            m_con->elog()->at(websocketpp::log::elevel::WARN) << "JSON Parse Error: " << matches[4] << websocketpp::log::endl;
            return;
         }
         on_socketio_json(msgId, matches[3], json);
         break;
      // Event
      case (5):
         m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received Message type 5 (Event): " << msg << websocketpp::log::endl;
#ifdef JSEP_SIP_GATEWAY_SUPPORT
   // Our event handler needs access to the whole message
   msgCopy = matches[4].str().c_str();
#endif   	 
         // Parse JSON
         if (json.Parse<0>(matches[4].str().c_str()).HasParseError())
         {
            m_con->elog()->at(websocketpp::log::elevel::WARN) << "JSON Parse Error: " << matches[4] << websocketpp::log::endl;
            return;
         }
         if (!json["name"].IsString())
         {
            m_con->elog()->at(websocketpp::log::elevel::WARN) << "Invalid socket.IO Event" << websocketpp::log::endl;
            return;
         }

         on_socketio_event(msgId, matches[3], json["name"].GetString(), json["args"]);
         break;
      // Ack
      case (6):
         m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received Message type 6 (ACK)" << websocketpp::log::endl;
         on_socketio_ack(matches[4]);
         break;
      // Error
      case (7):
         m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received Message type 7 (Error): " << msg << websocketpp::log::endl;
         on_socketio_error(matches[3], matches[4].str().substr(0, matches[4].str().find("+")), matches[4].str().substr(matches[4].str().find("+")+1));
         break;
      // Noop
      case (8):
         m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received Message type 8 (Noop)" << websocketpp::log::endl;
         break;
      default:
         m_con->elog()->at(websocketpp::log::elevel::WARN) << "Invalid Socket.IO message type: " << type << websocketpp::log::endl;
         break;
      }
   }
   else
   {
      m_con->elog()->at(websocketpp::log::elevel::WARN) << "Non-Socket.IO message: " << msg << websocketpp::log::endl;
   }
}

// This is where you'd add in behavior to handle the message data for your own app.
void socketio_client_handler::on_socketio_message(int msgId, std::string msgEndpoint, std::string data)
{
   m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received message (" << msgId << ") " << data << websocketpp::log::endl;
}

// This is where you'd add in behavior to handle json messages.
void socketio_client_handler::on_socketio_json(int msgId, std::string msgEndpoint, Document& json)
{
   m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received JSON Data (" << msgId << ")" << websocketpp::log::endl;
}

// This is where you'd add in behavior to handle events.
// By default, nothing is done with the endpoint or ID params.
void socketio_client_handler::on_socketio_event(int msgId, std::string msgEndpoint, std::string name, const Value& args)
{
   m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received event (" << msgId << ") " << websocketpp::log::endl;

   if (m_events.count(name) > 0)
   {
      socketio_events events;
      m_events[name](events, args);      
#ifdef JSEP_SIP_GATEWAY_SUPPORT         
      msgCb(name, args);
#endif      
   }
   else m_con->elog()->at(websocketpp::log::elevel::WARN) << "No bound event with name: " << name << websocketpp::log::endl;
}
#ifdef JSEP_SIP_GATEWAY_SUPPORT   

void socketio_client_handler::msgCb(std::string name, const Value& args)
{
   // Event handlers are responsible for knowing what's in args.
   // This function expects a string as the first argument, and doesn't care about the rest.
   std::cout << "Server message1 GetType: " << args.GetType() << std::endl;

   
   
   // Check if the type of the response is an array
   if (args.GetType() == kArrayType)
   {
     std::cout << "Server message args.Size(): " << args.Size() << std::endl;
     // Check how many elements are in the array
     for (int icount=0; icount < args.Size(); icount ++)
     {
       // Take out an element from an array
       //std::cout << "Elements received: Id: " << icount << "Type: " << args[icount].GetType() << std::endl;
       switch(args[icount].GetType())
       {
	 case kObjectType:
	 {
	   	// member iterator
		Value::ConstMemberIterator itr = args[icount].MemberBegin(); 
		while (itr != args[icount].MemberEnd())
		{
		  //if (itr->name.IsString())
		    //std::cout << "Elements received: Name: " << itr->name.GetString() << std::endl;
		  //if (itr->value.IsString())
		  //std::cout << "Elements received: Value: " << itr->value.GetString() << std::endl;
		  ++itr;
		}
		std::cout << "****** Type: " << args[icount][ktypeStr].GetString() << std::endl;
		
		// Check if the response from the server is a success
		if (!std::strcmp(args[icount][ktypeStr].GetString(), kofferStr))
		{
			// Incoming call
			std::cout << "Incoming call from address:" << args[icount][kfromAliasIdStr].GetString() << std::endl;

			if (!std::strcmp(args[icount][kfromAliasIdStr].GetString(),"bill.saylor@comcast.net"))
				//          if (!std::strcmp(args[icount]["streamOptions"]["one_way"].IsFalse()))
			{
				std::cout << " \n Bill - Screen sharing request !!!" <<  std::endl;
				conductor_OneWayCall = true;
			}else
			{
				std::cout << " \n Normal video call !!!" <<  std::endl;
				conductor_OneWayCall = false;
			}
		  //PeerConnectionClient::GetCallback()->OnPeerConnected(HARDCODED_PEERID, args[icount]["fromAlias"].GetString());
		  //appEventCb(HARDCODED_PEERID, kOfferType, args[icount]["fromAlias"].GetString(), args[icount]["sdp"].GetString());
		  //appEventCb(HARDCODED_PEERID, kOfferType, args[icount]["fromAlias"].GetString(), msgCopy);
		  //appEventCb(HARDCODED_PEERID, kOfferType, args[icount]["fromAlias"].GetString(), args.GetString()); 
		  //StringBuffer s(0, 1024 * 1024);
		  //Writer<StringBuffer> writer(s);
		  //PrettyWriter<StringBuffer> writer(s);
		  //writer.SetIndent(' ', 1);
		  //Value val = args;
		  //val.SetString();
		  //val.Accept(writer);
		  
		  //Document doc1;
		  
		  //doc1.SetArray();
		  
		  //doc1.Accept(writer);
		  //const char* str = s.GetString();
		  
		  //std::ostringstream os;
		  //rapidjson::Writer<std::ostringstream> writer(os);
		  //args.Accept(writer);
		  sdpData sdp;
		  sdp.id = HARDCODED_PEERID;
  		  sdp.From = args[icount][kfromStr].GetString();
		  sdp.Target = args[icount][ktargetStr].GetString();
		  sdp.FromAlias = args[icount][kfromAliasIdStr].GetString();
		  sdp.ToAlias = args[icount][ktoAliasIdStr].GetString();
		  sdp.Sdp = args[icount][ksdpStr].GetString();
		  sdp.Msg = msgCopy;
		  sdp.MsgType = kOfferType;
		  sdp.msgName = name;
		  sdp.PingData.clientSessionId = args[icount]["clientSessionId"].GetString();
		  appEventCb(sdp);

		  // Send the offer
		  //appEventCb(HARDCODED_PEERID, args[icount]["fromAlias"].GetString());
		}
		
		// Check if the response from the server is a success
		if (!std::strcmp(args[icount]["type"].GetString(), "answer"))
		{
		  // Incoming call
		  std::cout << "!!!!!Incoming call from address:" << args[icount]["target"].GetString() << std::endl;

		  printf("!!!!!!Inside answer...");
		  sdpData sdp;
		  sdp.id = HARDCODED_PEERID;
  		  sdp.From = args[icount][kfromStr].GetString();
		  sdp.Target = args[icount][ktargetStr].GetString();
		  sdp.Sdp = args[icount][ksdpStr].GetString();
		  sdp.Msg = msgCopy;
          sdp.msgName = name;
          sdp.PingData.clientSessionId = args[icount]["clientSessionId"].GetString();
          sdp.MsgType = kAnswerType;
          appEventCb(sdp);

		  // Send the offer
		  //appEventCb(HARDCODED_PEERID, args[icount]["fromAlias"].GetString());
		}

		//Bye message
		if (!std::strcmp(args[icount]["type"].GetString(), "bye"))
		{
		  // Incoming call
		  std::cout << "!!!!!Bye message received" << std::endl;

		  sdpData sdp;
		  sdp.id = HARDCODED_PEERID;
		  sdp.Msg = msgCopy;
          sdp.MsgType = kByeType;
          sdp.msgName = name;
		  appEventCb(sdp);
		  // Send the offer
		  //appEventCb(HARDCODED_PEERID, args[icount]["fromAlias"].GetString());
		}

        //Ping message

        if (!std::strcmp(args[icount][ktypeStr].GetString(), "ping"))
        {
          // Ping message
          std::cout << "!!!!!Ping message received" << std::endl;

          sdpData sdp;
          sdp.id = HARDCODED_PEERID;
          sdp.Msg = msgCopy;
          sdp.MsgType = kPingMessage;
          sdp.msgName = name;
          sdp.PingData.clientSessionId = args[icount]["clientSessionId"].GetString();
          //Shayo sdp.PingData.legId = args[icount]["legId"].GetString();
          sdp.PingData.socketid = args[icount]["socketid"].GetString();
          appEventCb(sdp);
          // Send the offer
          //appEventCb(HARDCODED_PEERID, args[icount]["fromAlias"].GetString());
        }

		if (!std::strcmp(args[icount][ktypeStr].GetString(), kregSuccess))
		{
		  // Login success
		  std::cout << "Login success" << std::endl;
                  sdpData sdp;
                  sdp.MsgType = kLoginType;
                  sdp.msgName = name;
                  appEventCb(sdp);
		}

		if (!std::strcmp(args[icount][ktypeStr].GetString(), "regfailure"))
		{
		  // Login success
		  std::cout << "Login Failed" << std::endl;
                  sdpData sdp;
                  sdp.MsgType = kLoginFailure;
                  sdp.msgName = name;
                  appEventCb(sdp);
		}

		if (!std::strcmp(args[icount]["type"].GetString(), kIceServers))
		{
			// Got IceServer message
			std::cout << "Ice servers received" << std::endl;
			sdpData sdp;
            sdp.MsgType = kIceServer;
            sdp.msgName = name;
            sdp.PingData.clientSessionId = args[icount]["clientSessionId"].GetString();
			sdp.iceServersData.username = args[icount][kIceServers]["username"].GetString();
			std::cout << "sdp.iceServersData.username " << sdp.iceServersData.username << std::endl;
			sdp.iceServersData.credential = args[icount][kIceServers]["credential"].GetString();
			std::cout << "sdp.iceServersData.credential " << sdp.iceServersData.credential << std::endl;
			/*if(args[icount][kIceServers]["uris"].GetType()==kArrayType)
			{
				int uriSize = args[icount][kIceServers]["uris"].Size();
				std::cout << "URI SIZE " << uriSize << std::endl;
				for(int i=0; i<uriSize; i++)
				{
					sdp.iceServersData.uri[i] = args[icount][kIceServers]["uris"][i].GetString();
					std::cout << "URI:: " << sdp.iceServersData.uri[i] << std::endl;
				}
			}*/
			sdp.iceServersData.uri = args[icount][kIceServers]["uris"].GetString();
			std::cout << "sdp.iceServersData.uri " << sdp.iceServersData.uri << std::endl;
			std::cout << "clientSessionId from server:: " << sdp.PingData.clientSessionId << std::endl;
			appEventCb(sdp);
		}

		//video mail
		if (!std::strcmp(args[icount][ktypeStr].GetString(), kNotification))
		{
		  // video mail notification
          std::cout << "Notification received for video mail  " << icount << " keventStr " << keventStr << std::endl;
          std::cout << "TYPE :: " << args[icount][ktypeStr].GetString() << std::endl;
          sdpData sdp;
          sdp.id = HARDCODED_PEERID;
  		  sdp.NotifData.event = args[icount][keventStr].GetString();
          std::cout << "Notification received for video mail 3" << std::endl;
  		  sdp.NotifData.videourl = args[icount][kvideourlStr].GetString();
          std::cout << "Notification received for video mail 4" << std::endl;
  		  sdp.NotifData.thumburl = args[icount][kthumburlStr].GetString();
          std::cout << "Notification received for video mail 5" << std::endl;
  		  sdp.NotifData.title = args[icount][ktitleStr].GetString();
          std::cout << "Notification received for video mail 6" << std::endl;
          sdp.MsgType = kNotificationType;
           std::cout << "Notification received for video mail !!!!!!!!!!!!" << std::endl;
           sdp.msgName = name;
		  appEventCb(sdp);
		}		
		// Check if the response from the server is a success
		if (!std::strcmp(args[icount][ktypeStr].GetString(), kcandidateStr))
		{
		  // Candidates
		  std::cout << "Candidates received" << std::endl;
		  std::cout << "args:1" << args.GetString() << std::endl;
		  std::cout << "kcandidateStr:" << kcandidateStr << std::endl;
		  //std::cout << "args:2" << args[icount]["id"].GetString() << std::endl;
		  //std::cout << "args:" << args[icount]["label"].GetInt() << std::endl;
		//  std::cout << "args:3" << args[icount][kcandidateStr].GetString() << std::endl;
		  std::cout << "args:3" << std::endl;
		  sdpData sdp;
		  sdp.lastTemp = false;
		  sdp.id = HARDCODED_PEERID;
  		  sdp.Msg = msgCopy;
          sdp.MsgType = kCandidateType;
          sdp.msgName = name;
          std::cout << "args:4" << std::endl;
          if(!(args[icount].HasMember("last")))
          {
          std::cout << "Last Message NOT Received!!!!!!!!" << std::endl;
          sdp.sdpMid = args[icount]["id"].GetString();
          sdp.sdpMlineIndex = args[icount]["label"].GetInt();
          sdp.Sdp = args[icount][kcandidateStr].GetString();
          }
          else
          {
        	  sdp.lastTemp = true;
        	  std::cout << "Candidate exhausted.last message received" << std::endl;
          }
		  appEventCb(sdp);
		}

		// Photo notification
		if (!std::strcmp(args[icount][ktypeStr].GetString(), kSessionMessage))
		{
			  std::cout << "Session message received" << std::endl;
			  std::cout << "Session message received url:==>" << args[icount][kSessionMessage]["url"].GetString() << std::endl;
			  sdpData sdp;
			  sdp.id = HARDCODED_PEERID;
	  		  sdp.SessionMsgData.type = args[icount][kSessionMessage][ktypeStr].GetString();
	  		  //if(!std::strcmp(sdp.SessionMsgData.type, "photo"))
	  		  if(sdp.SessionMsgData.type == "photo")
	  		  {
	  			std::cout << "Notification type is photo" << std::endl;
		  		sdp.SessionMsgData.url = args[icount][kSessionMessage][kUrl].GetString();
                sdp.msgName = name;

	  		  }
	  		  //else if(std::strcmp(sdp.SessionMsgData.type, "chat"))
	  		  else if(sdp.SessionMsgData.type == "chat")
	  		  {
	  			std::cout << "Notification type is chat" << std::endl;
	  			sdp.SessionMsgData.msg = args[icount][kSessionMessage][kMsg].GetString();
	  			std::cout << "Notification type is chat" << sdp.SessionMsgData.msg << std::endl;
	  		  }
	          sdp.MsgType = kSessionMsgType;
              sdp.msgName = name;
			  appEventCb(sdp);
		}
	 }
	 break;
	 case kStringType:
	 break;
	 case kNumberType:
	 break;
	 default:
	   std::cout << "Unknown type !!!" << std::endl;
	 break;
       }
     }
   }  

}

// Vitual fn which wil be invoked upon event
void socketio_client_handler::appEventCb(sdpData sdp)
{
  //printf("socketio_client_handler::appEventCb called");
  std::cout << "socketio_client_handler::appEventCb id " << std::endl;
}
   
#endif  
// This is where you'd add in behavior to handle ack
void socketio_client_handler::on_socketio_ack(std::string data)
{
   m_con->alog()->at(websocketpp::log::alevel::DEVEL) << "Received ACK: " << data << websocketpp::log::endl;
}

// This is where you'd add in behavior to handle errors
void socketio_client_handler::on_socketio_error(std::string endpoint, std::string reason, std::string advice)
{
   m_con->alog()->at(websocketpp::log::elevel::RERROR) << "Received Error: " << reason << " Advice: " << advice << websocketpp::log::endl;
}
