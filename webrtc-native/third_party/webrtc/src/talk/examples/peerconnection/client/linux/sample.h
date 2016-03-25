#ifndef CLIENT_LINUX_SAMPLE_H
#define CLIENT_LINUX_SAMPLE_H

#include "stdio.h"
#include <iostream>

#include "webRTCStack.hpp"
#include "webRTCSession.hpp"

using namespace std;

class Sample: public StackObserver, public SessionObserver{

public:
	//webRTCStack stack; //gk
	webRTCStack *stack;
	webRTCSession *session;
	Sample(string serverip, string fromAddr, string toAddr, string callOption);
	void createObserver(SessionObserver *sessionObserver);
	//void createStack(webRTCStack stack); //gk
	void createStack(webRTCStack *stack);
	virtual void onSession(webRTCSession *session);
	virtual void onReady(sdpData email);

	virtual void onSessionConnect();
	virtual void onSessionConnecting();
	virtual void onSessionRenegotiation();
	virtual void onSessionEnd();
	virtual void onLocalStreamAdded(const long stream, long localVideoTrack, string streamid);
	virtual void onRemoteStreamAdded(const long stream, string streamId);
	virtual void onRemoteStreamRemoved(const long stream, string streamId);

	string serverip;
	string FromId;
	string ToId;
	string callOption;
	Json::Value options;

private:
//	streamOption sOption;
	socketio::sdpData sdpOut;
	SessionObserver *sessionObserver;
};

#endif // SAMPLE_H
