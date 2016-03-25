#ifndef CLIENT_LINUX_WEBRTCSTACK_HPP
#define CLIENT_LINUX_WEBRTCSTACK_HPP

#include "webRTCSession.hpp"
#include "SignalHandler.hpp"

//#include <backward/hash_map>
#include <stdio.h>

using namespace std;
using namespace __gnu_cxx;

class webRTCSession;
class SessionObserver;
class SignalHandler;

struct streamOption{
	bool broadcast;
	bool one_way;
	string video_resolution;// = "hd";
};

//Observer class has to inherited in class which creates instance of webRTCStack
class StackObserver{
public:
    virtual void onSession(webRTCSession *session){};
    virtual void onReady(sdpData email){};
};

//class webRTCStack: public SessionCallback{
class webRTCStack {
public:
    webRTCStack();                                                      // connect to server through signalHandler
    webRTCStack(string url, string emailAddress, StackObserver *stackObserver);   // connect to server through signalHandler
    ~webRTCStack();
    webRTCSession* createSession(string target, string fromId, Json::Value Options, SessionObserver *sessionobserver); // start session for any activity that user connects to server.
    void onRegServerMessage(sdpData message);
    void onRTCServerMessage(sdpData message);    // if incoming call, create session or pass the message to webrtcsession class.

    //outgoing
    void sendRTCMessage(Json::Value message);    // to send any outgoing message string to the signalHandler
    void dispose();                         // delete peerconnection factory and close the connection

    //override from SignalHandler
    virtual void onSignalingMessage(string event,sdpData message);    // parse the type of incoming message from signalHandler and call related APIs.


private:
    webRTCSession *session;
    SignalHandler *sHandler;
    std::string clientSessionId;
//gk
    typedef std::map<string, webRTCSession*> sessions_;
    sessions_ sessions;
    StackObserver *stackobserver;

};

#endif // WEBRTCSTACK_HPP
