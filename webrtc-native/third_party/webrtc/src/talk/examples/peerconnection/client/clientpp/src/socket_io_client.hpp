/* socket_io_client.hpp
 * Evan Shimizu, June 2012
 * websocket++ handler implementing (most of) the socket.io protocol.
 * https://github.com/LearnBoost/socket.io-spec 
 *
 * This implementation uses the rapidjson library.
 * See examples at https://github.com/kennytm/rapidjson.
 */

#ifndef SOCKET_IO_CLIENT_HPP
#define SOCKET_IO_CLIENT_HPP

#pragma warning(disable:4355) // C4355: this used in base member initializer list

#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <rapidjson/document.h>		// rapidjson's DOM-style API
#include <rapidjson/prettywriter.h>	// for stringify JSON
#include <rapidjson/filestream.h>
#include <rapidjson/stringwriter.h>
#include "webrtc/base/json.h"

#include <roles/client.hpp>
#include <websocketpp.hpp>

#include <map>
#include <string>
#include <queue>
#include "talk/examples/peerconnection/client/defaults.h"

#define JSON_BUFFER_SIZE 20000

class s;
using websocketpp::client;
using namespace rapidjson;
using namespace rapidjson;
#define JSEP_SIP_GATEWAY_SUPPORT 

namespace socketio {

// Class for event callbacks.
// Class is automatically created  on the stack to handle calling the function when an event callback is triggered.
// Class is broken out from the main handler class to allow for easier editing of functions
// and modularity of code.
// If you don't want to use this class, you can modify the callback type to whatever suits your purpose.
#ifdef JSEP_SIP_GATEWAY_SUPPORT
//! Type of message
typedef enum Type {
	kOfferType 	= 0,		//!< Offer message from peer
	kAnswerType = 1,		//!< Answer from peer
	kLoginType 	= 2,		//!< Login success
    kCandidateType  = 3,
    kHeartBeatType  = 4,
    kNotificationType =5,
    kByeType =6,
    kSessionMsgType =7,
    kPingMessage=8,
    kIceServer=9,
    kLoginFailure =10
}Type_e;
#endif

class socketio_events 
{
public:
  
   void example(const Value& args);
#ifdef JSEP_SIP_GATEWAY_SUPPORT   
   void serverMsg(const Value& args);
   void rtcMsg(const Value& args);
   //struct PeerConnectionClientObserver* callback_;
   //void SetCallback(struct PeerConnectionClientObserver* callback_); 
#endif   
};
#ifdef JSEP_SIP_GATEWAY_SUPPORT

typedef struct Notif_data_t_{
	std::string event;
	std::string videourl;
	std::string thumburl;
	std::string title;

}Notif_data_t;


typedef struct Ping_data_t_{
    std::string clientSessionId;
    std::string legId;
    std::string socketid;
}Ping_data_t;

typedef struct SessionMsg_data_t_{
	std::string url;
	std::string msg;
	std::string type;
}SessionMsg_data_t;

typedef struct IceServer_data_t_{
	Json::Value uri;
	std::string username;
	std::string credential;
}IceServer_data_t;

//struct sdpData{
typedef struct {
  int id;
  Notif_data_t NotifData;
  SessionMsg_data_t SessionMsgData;
  std::string Msg;
  std::string Sdp;
  std::string From;
  std::string Target;
  std::string FromAlias;
  std::string ToAlias;
  Type_e MsgType;
  Ping_data_t  PingData;
  std::string msgName;
  std::string sdpMid;
  bool lastTemp;
  int sdpMlineIndex;
  IceServer_data_t iceServersData;
}sdpData;
//sdpData sdpData_;
#endif

class socketio_client_handler : public client::handler {
public:
   socketio_client_handler() :
      m_heartbeatActive(false),
      m_connected(false)
   { }

   ~socketio_client_handler() {
      if (m_con)
         m_con->close(websocketpp::close::status::GOING_AWAY, "Client Initiated Disconnect");
   }

   // Callbacks
   void on_fail(connection_ptr con);
   void on_open(connection_ptr con);
   void on_close(connection_ptr con);
   void on_message(connection_ptr con, message_ptr msg);

   // Client Functions - such as send, etc.

   // Function pointer to a event handler.
   // Args is an array, managed by rapidjson, and could be null
   // Can change to whatever signature you want, just make sure to change the call in on_socketio_event too.
   
#ifndef BOOST_NO_CXX11_HDR_FUNCTIONAL
   // If you're using C++11 and have the proper functional header in the standard lib, we'll use that
   typedef std::function<void (socketio_events&, const Value&)> eventFunc;
#else
   // Otherwise we'll let boost fill in the gaps
   typedef boost::function<void (socketio_events&, const Value&)> eventFunc;
   //typedef void eventFunc(const Value&) ;
   //typedef void (*PeerConnectionClient::eventFunc)(const Value&) ;
   //PeerConnectionClient pptr;
#endif

   // Performs a socket.IO handshake
   // https://github.com/LearnBoost/socket.io-spec
   // param - url takes a ws:// address with port number
   // param - socketIoResource is the resource where the server is listening. Defaults to "/socket.io".
   // Returns a socket.IO url for performing the actual connection.
   std::string perform_handshake(std::string url, std::string socketIoResource = "/socket.io");

   // Sends a plain string to the endpoint. No special formatting performed to the string.
   void send(const std::string &msg);

   // Allows user to send a custom socket.IO message
   void send(unsigned int type, std::string endpoint, std::string msg, unsigned int id = 0);

   // Signal connection to the desired endpoint. Allows the use of the endpoint once message is successfully sent.
   void connect_endpoint(std::string endpoint);

   // Signal disconnect from specified endpoint.
   void disconnect_endpoint(std::string endpoint);

   // Emulates the emit function from socketIO (type 5)
   void emitt(std::string name, Document& args, std::string endpoint = "", unsigned int id = 0);
   void emitt(std::string name, std::string arg0, std::string endpoint = "", unsigned int id = 0);

   // Sends a plain message (type 3)
   void message(std::string msg, std::string endpoint = "", unsigned int id = 0);

   // Sends a JSON message (type 4)
   void json_message(Document& json, std::string endpoint = "", unsigned int id = 0);

   // Binds a function to a name. Function will be passed a a Value ref as the only argument.
   // If the function already exists, this function returns false. You must call unbind_event
   // on the name of the function first to re-bind a name.
   bool bind_event(std::string name, eventFunc func);
   void SetCallback();

   // Removes the binding between event [name] and the function associated with it.
   bool unbind_event(std::string name);

   // Closes the connection
   void close();

   // Heartbeat operations.
   void start_heartbeat();
   void stop_heartbeat();
   
   std::string getSid() { return m_sid; }
   std::string getResource() { return m_resource; }
   bool connected() { return m_connected; }

#ifdef JSEP_SIP_GATEWAY_SUPPORT   
  void msgCb(std::string name, const Value& args);
   // Vitual fn which wil be invoked upon event
   virtual void appEventCb(sdpData sdp);
   
#endif  
private:
   // Sends a heartbeat to the server.
   void send_heartbeat();

   // Called when the heartbeat timer fires.
   void heartbeat();

   // Parses a socket.IO message received
   void parse_message(const std::string &msg);

   // Message Parsing callbacks.
   void on_socketio_message(int msgId, std::string msgEndpoint, std::string data);
   void on_socketio_json(int msgId, std::string msgEndpoint, Document& json);
   void on_socketio_event(int msgId, std::string msgEndpoint, std::string name, const Value& args);
   void on_socketio_ack(std::string data);
   void on_socketio_error(std::string endppoint, std::string reason, std::string advice);

   // Connection pointer for client functions.
   connection_ptr m_con;
   // Socket.IO server settings
   std::string m_sid;
   unsigned int m_heartbeatTimeout;
   unsigned int m_disconnectTimeout;
   std::string m_socketIoUri;
   std::string m_resource;
   bool m_connected;

   // Currently we assume websocket as the transport, though you can find others in this string
   std::string m_transports;

   // Heartbeat variabes.
#ifndef BOOST_NO_CXX11_SMART_PTR
   // If you're using C++11 use the standar library smart pointer
   std::unique_ptr<boost::asio::deadline_timer> m_heartbeatTimer;
#else
   // Otherwise let boost provideo the smart pointer
   boost::shared_ptr<boost::asio::deadline_timer> m_heartbeatTimer;
#endif

   bool m_heartbeatActive;

   // Event bindings
   std::map<std::string, eventFunc> m_events;
#ifdef JSEP_SIP_GATEWAY_SUPPORT   
   std::string msgCopy;
#endif   

};
typedef boost::shared_ptr<socketio_client_handler> socketio_client_handler_ptr;

}

#endif // SOCKET_IO_CLIENT_HPP