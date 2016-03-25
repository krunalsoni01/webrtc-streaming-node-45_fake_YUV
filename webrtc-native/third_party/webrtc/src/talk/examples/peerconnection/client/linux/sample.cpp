
#include "sample.h"
#include <iostream>

#include "webRTCSession.hpp"
//#include "talk/base/logging.h"
#include "webrtc/base/logging.h"
#include "talk/examples/peerconnection/client/defaults.h"

Sample::Sample(string serverip, string fromAddr, string toAddr, string callOption){
	//string streamoptions = "{}";
	//webRTCStack stack_ = new webRTCStack();
	Json::Value sOption;
	sOption["video_resolution"] = "hd";
	sOption["broadcast"] = true;
	sOption["one_way"] = true;
	options["streamOptions"] = sOption;
	this->serverip = serverip;
	FromId = fromAddr;
	ToId = toAddr;
	this->callOption = callOption;
}

void Sample::createObserver(SessionObserver *sessionObserver)
{
	this->sessionObserver = sessionObserver;
}

void Sample::createStack(webRTCStack *stack)
{
	cout << "createStack" << endl;
	this->stack = stack;
//	stack = new webRTCStack(this->serverip, FromId, this);
}

/*    sdpOut.FromAlias = FromId;
    sdpOut.ToAlias = ToId;*/
void Sample::onReady(sdpData email){
	//if(callOption != NULL)
	cout << "Sample::onReady" << endl;
//	session = stack.createSession(ToId, FromId, options, new SessionObserver());
	//session = stack.createSession(ToId, FromId, options, sessionObserver);
	session = stack->createSession(ToId, FromId, options, sessionObserver);

}
void Sample::onSession(webRTCSession *session){
	this->session = session;
	this->sessionObserver = new SessionObserver();
//	this->session->on(new SessionObserver());
	this->session->on(sessionObserver);
}

//SessionObserver class implementation
void Sample::onSessionConnect(){
	LOG(INFO) <<  "session connected offer/answer exchanged.";
}

void Sample::onSessionConnecting(){
	LOG(INFO) <<  "session is trying to connect";
}

void Sample::onSessionRenegotiation(){
	LOG(INFO) <<  "session requires renegotiation reoffer received.";
}

void Sample::onSessionEnd(){
	LOG(INFO) <<  "session has ended";
}

void Sample::onLocalStreamAdded(const long stream, long localVideoTrack, string streamid){
	LOG(INFO) <<  "local stream added";
}

void Sample::onRemoteStreamAdded(const long stream, string streamId){
	LOG(INFO) <<  "remote stream added";
}

void Sample::onRemoteStreamRemoved(const long stream, string streamId){
	LOG(INFO) <<  "remote stream removed";
}
