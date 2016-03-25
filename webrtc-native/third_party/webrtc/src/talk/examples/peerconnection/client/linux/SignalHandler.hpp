#ifndef CLIENT_LINUX_SIGNALHANDLER_HPP
#define CLIENT_LINUX_SIGNALHANDLER_HPP


//#include "webRTCStack.hpp"
#include "signalingwrapper.h"
#include "webRTCStack.hpp"

using namespace std;
using namespace socketio;

class webRTCStack;


class HandlerCallBack {
public:
	virtual void onSignalingMessage(string event, sdpData message){};
	// enable the below API on using onConnect() callback from socket
	//virtual void onConnected();
};

class SignalHandler: public SignalingWrapper{

public:
    SignalHandler();
    SignalHandler(string server_url, webRTCStack *stack);
    ~SignalHandler();
    void connectToSignalingServer(string emailId);   // call socketIOClient.perform_handshake to connect to the server
    void sendClientRegMessage(string message);  // send reg_client_message with user details to socketio_client
    void sendClientRTCMessage(Json::Value message);  // send rtc_client_message to socketio_client
    void sendClientAuthMessage(string message); // send rtc_auth_message to socketio_client
    void disconnect();                          // close the client session

    void onMessage(sdpData message);             // Receive incoming messages from socketio_client and send to upper layer.
    virtual void onSignalingMessage(string event, sdpData message){};
    //webRTCStack stack;

private:
    string GATEWAY_SERVER_URL;
    SignalingWrapper *signaling;
    HandlerCallBack *hcb_;
    webRTCStack *lStack;
};

#endif // SIGNALHANDLER_HPP
