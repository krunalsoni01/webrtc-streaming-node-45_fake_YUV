#ifndef CLIENT_LINUX_SIGNALINGWRAPPER_H
#define CLIENT_LINUX_SIGNALINGWRAPPER_H

#include "talk/examples/peerconnection/client/clientpp/src/socket_io_client.hpp"
//#include "talk/base/scoped_ptr.h"
#include <iostream>
#include "json/json.h"

using namespace socketio;
using namespace std;

class WrapperCallback{
public:
	virtual void onMessage(sdpData message){};
};

class SignalingWrapper: public socketio_client_handler 
{
public:
    SignalingWrapper();
    void connectAndLogin(string serverAddr, string emailAddr);
    void sendOffer(Json::Value message);
    void appEventCb(sdpData sdp);
//    SignalHandler sHandler;
//    WrapperCallback *wcb_;
//    socketio_client_handler *handler;
    virtual void onMessage(sdpData message){};
    int temp;

};
typedef boost::shared_ptr<socketio_client_handler> socketio_client_handler_ptr;

#endif // SIGNALINGWRAPPER_H
